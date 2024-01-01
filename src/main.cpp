#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <string.h>
#include "clocklab_types.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Tools.h"

#include <NTPClient.h>   // https://github.com/arduino-libraries/NTPClient

#include "TimeType/Timestamp.h"

#include "Source/RTCSystemTimeHandler.h"
#include "Source/GPSTimeHandler2.h"
#include "Source/ManualTimeHandler.h"

#include "Display/ConsoleViewHandler.h"
#include "Display/LEDClockViewHandler.h"
#include "Display/OLEDDisplayClockViewHandler.h"

#include "Converter/SplitterTimeHandler.h"
#include "Converter/DSTSunriseSunsetTimeHandler.h"

#include "AdjustmentAdvisor.h"

#include "Controller.h"


#include "WifiCred.h"



const  char*   gc_Ssid { SSID };
const  char*   gc_Password{ PASSWD };

//=============================================================================================================

constexpr int gc_XCoreId_0{ 0};
constexpr int gc_XCoreId_1{ 1};

constexpr uint8_t   gc_irqIn_pin{ 15}; // D8 - don't use D4

//constexpr uint8_t   gc_rxd2_pin{17};
constexpr uint8_t   gc_txd2_pin{16};

constexpr uint8_t   gc_sda_pin{21};
constexpr uint8_t   gc_scl_pin{22};

constexpr uint8_t   gc_DIO_pin{ 13}; // 25
constexpr uint8_t   gc_CLK_pin{ 33}; // 26
constexpr uint8_t   gc_STB_pin{ 32}; // 27


constexpr uint8_t   gc_SI_pin{25};
constexpr uint8_t   gc_RCK_pin{26};
constexpr uint8_t   gc_SCK_pin{27};
constexpr uint8_t   gc_NSCLR_pin{14};

constexpr uint8_t   gc_PULS_pin{4};

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0
constexpr auto       gc_GMT_Plus_2h{ 2* 3600};
constexpr auto       gc_period_1000_Millis{ 1000};

// Location Nowy Dworm Mazowiecki
//double  g_Latitude{  52.4465078};  // 52.2507628, 020.4409067
//double  g_Longitude{ 20.6925219};  // 
Coordinates_t g_coordinates{  52.4465078, 20.6925219};

MyTime      g_SunriseTime, g_SunsetTime;
Timestamp   g_LocalTimestamp;


OLEDDisplayClockViewHandler g_OLEDViewHandler( nullptr, g_SunriseTime, g_SunsetTime);
//LEDClockViewHandler         g_LEDViewHandler(  nullptr, g_SunriseTime, g_SunsetTime, gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
LEDClockViewHandler         g_LEDViewHandler( &g_OLEDViewHandler, g_SunriseTime, g_SunsetTime, gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
ConsoleViewHandler          g_ConsoleViewHandler( &g_LEDViewHandler);

SplitterTimeHandler         g_SplitterHandler(nullptr, g_LocalTimestamp);  
DSTSunriseSunsetTimeHandler g_TimeZoneDSTHandler( &g_SplitterHandler, gc_GMT_Plus_2h, g_coordinates, g_SunriseTime, g_SunsetTime);
RTCSystemTimeHandler        g_SystemTimeHandler(  &g_TimeZoneDSTHandler, gc_sda_pin, gc_scl_pin, gc_irqIn_pin);
 

GPSTimeHandler2             g_GPSHandler( nullptr, g_coordinates);
ManualTimeHandler           g_ManualAdjHandler;

Controller                  g_Controller;

static xQueueHandle       g_queueTimePattern= xQueueCreate( 10, sizeof( MessageTime_t));
static xQueueHandle       g_queueDisplay=     xQueueCreate( 10, sizeof( MessageTime_t));
static SemaphoreHandle_t  g_xSemaphoreRtc;

AdjustmentAdvisor advisor;
//=============================================================================================================
void consoleInTask(void *pvParameter) 
{


  Serial.print( "\nCONSOLE_IN_task:  start\n");

  uint8_t buttons = 0;
  for(;;)
  {
    vTaskDelay( 100 / portTICK_RATE_MS);

    buttons= g_LEDViewHandler.buttonsRead();
    g_Controller.handle( ( Buttons_t)buttons);

    switch( g_Controller.getState())
    {
      case eShow_Time:
        Serial.print( "Show TIME\n");
        g_ConsoleViewHandler.setDisplayMode( displayMode_t::eTime);
        break;

      case eShow_Date:
        Serial.print( "Show DATE\n");
        g_ConsoleViewHandler.setDisplayMode( displayMode_t::eDate);
        break;

      default:;
    };

  }

  vTaskDelete(nullptr);  
}

//=============================================================================================================
void consoleOutTask(void *pvParameter)
{
  // initialize digital pin LED_BUILTIN as an output.
  Timestamp       displayTimestamp;
  MessageTime_t   rcvMsg;

  printTick();  
  Serial.print( "\nCONSOLE_OUT_task:  start\n");

  for(;;)
  {
    vTaskDelay( 10 / portTICK_RATE_MS);                    // wait for a second
    if (xQueueReceive( g_queueDisplay, (void *)&rcvMsg, 0) == pdFALSE)
    {
      continue;
    } 
    
    printTick();  Serial.print( "  console_task  ");  Serial.printf( "-> %d   ", rcvMsg.type);
    displayTimestamp.setEpochTime( rcvMsg.epoch);

    g_ConsoleViewHandler.updateTime( displayTimestamp);
    
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void ntpTask(void *pvParameter)
{
  WiFiUDP     udp;
  NTPClient   timeClient( udp);


  MessageTime_t   ntp_msg;
  ntp_msg.type= src_type_t::NTP;

  printTick();  Serial.print( "\nNTP_task:  start\n"); 

  WiFi.mode(WIFI_STA); 
  
  for(;;)
  { 
    vTaskDelay( 10*1000 / portTICK_RATE_MS);

  //  Serial.printf("\nTrying to connect...\n");  
    WiFi.begin( gc_Ssid, gc_Password);
    while ( !WiFi.isConnected())
    {
      Serial.printf("#");
      vTaskDelay( 3*1000 / portTICK_RATE_MS);
    };
    
    Serial.printf("| NTP... |");

    timeClient.begin(); 
    timeClient.forceUpdate();
    
    ntp_msg.epoch= timeClient.getEpochTime(); 
    ntp_msg.epochMillis= (uint32_t)((1000.0f* timeClient.getMillis())/UINT32_MAX);  //  Serial.printf("millis => %u\n", ntp_msg.epochMillis);
    ntp_msg.rtcMillis= millis();
//
//    Serial.printf("NTP  EpochMillis => %u || RTCmillis => %u\n",  ntp_msg.epochMillis, ntp_msg.rtcMillis);  

    Timestamp timestamp;
    timestamp.setEpochTime( ntp_msg.epoch);

    displayTimestamp( "NTP", timestamp); 
    
//
    while ( xQueueSend( g_queueTimePattern, (void *)&ntp_msg, 10) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

    timeClient.end();

  //  Serial.printf("\nDisconnect...\n");
    WiFi.disconnect();
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void gpsTask(void *pvParameter)
{
  char c;
  MessageTime_t   gps_msg;
  gps_msg.type=   src_type_t::GPS;

  Serial2.begin(9600);
  Serial2.flush();

  vTaskDelay( 800 / portTICK_RATE_MS);
  printTick();  Serial.print( "\nGPS_task:  start\n");  

  for(;;)
  {
    vTaskDelay(  100 / portTICK_RATE_MS);

//    Serial.printf("| GPS...|");

    bool next= true;
    while( next)
    {
      vTaskDelay( 7 / portTICK_RATE_MS);
      if( !Serial2.available())  
      {
//        Serial.printf( "GPS: !Serial2.available\n");
//        Serial.printf( "*");
        continue;
      }

      c= Serial2.read();
      next= g_GPSHandler.collectRecord( c);
    };

    if( !g_GPSHandler.updateTime())
    {
      continue;
    }
    
//    Serial.printf( "GPS: encoded\n");
    gps_msg.epoch=  g_GPSHandler.getTimestamp().getEpochTime(); 
    gps_msg.epochMillis= (uint32_t) g_GPSHandler.getMilliSecond();
    gps_msg.rtcMillis= millis();

    Timestamp timestamp;
    timestamp.setEpochTime( gps_msg.epoch); 
    displayTimestamp( "GPS", timestamp);
     
    while ( xQueueSend( g_queueTimePattern, (void *)&gps_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put GPS time to queue."); 
      vTaskDelay( 2 / portTICK_RATE_MS); 
    }
//    Serial.printf("| GPS...!!!!|\n");

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void manualAdjustmentTask(void *pvParameter)
{
  MessageTime_t   adj_msg;
  adj_msg.type=   src_type_t::MANUAL;

  vTaskDelay( 800 / portTICK_RATE_MS);
  printTick();  Serial.print( "\n manualAdjustmentTask:  start\n");  

  for(;;)
  {
    vTaskDelay(  100 / portTICK_RATE_MS);

    Timestamp timeStamp= g_ManualAdjHandler.getTimestamp();
    adj_msg.epoch = timeStamp.getEpochTime();
    adj_msg.rtcMillis= millis();
    while ( xQueueSend( g_queueTimePattern, (void *)&adj_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put MAN ADJ time to queue."); 
      vTaskDelay( 2 / portTICK_RATE_MS); 
    }
//    Serial.printf("| MANUAL ADJ...!!!!|\n");

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void keyboard_task(void *pvParameter)
{
  vTaskDelay( 20000 / portTICK_RATE_MS);

  pinMode( gc_SI_pin, OUTPUT);
  pinMode( gc_NSCLR_pin, OUTPUT);
  pinMode( gc_RCK_pin, OUTPUT);
  pinMode( gc_SCK_pin, OUTPUT);

  digitalWrite( gc_NSCLR_pin, HIGH);
  digitalWrite( gc_RCK_pin, LOW);
  digitalWrite( gc_SCK_pin, LOW);
  digitalWrite( gc_SI_pin,HIGH);

  uint8_t k=0;

  for(;;)
  {
    vTaskDelay( 5 / portTICK_RATE_MS);

    digitalWrite( gc_SI_pin, k %8 ==0? HIGH:LOW);
    k++;
    
    digitalWrite( gc_SCK_pin, HIGH);
    digitalWrite( gc_RCK_pin, LOW);
    vTaskDelay( 5 / portTICK_RATE_MS);

    digitalWrite( gc_SCK_pin, LOW);
    digitalWrite( gc_RCK_pin, HIGH);
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcReadTask(void *pvParameter)  
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcReadMsg;

  printTick();  Serial.print( "\nRTC_READ_task:  start\n");
  g_SystemTimeHandler.init();
  
  for(;;)
  { 
    vTaskDelay( 10 / portTICK_RATE_MS);

    if( xSemaphoreTake( g_xSemaphoreRtc,0) == pdTRUE)
    {
      
      bool timeUpdated = g_SystemTimeHandler.updateTime(); 
      xSemaphoreGive(  g_xSemaphoreRtc);

      // g_LocalTimestamp updated by g_SystemTimeHandler
      if( timeUpdated)
      {
        Serial.print( "RTC: UPDATE\n");
      
        digitalWrite( gc_PULS_pin,  gc_PULS_pin? LOW:HIGH); 

        // set time for Display
        rtcReadMsg.epoch= g_LocalTimestamp.getEpochTime();
        rtcReadMsg.epochMillis= 0;
        rtcReadMsg.rtcMillis= millis();

        while( xQueueSend( g_queueDisplay, (void *)&rtcReadMsg, 0) != pdTRUE) 
        {
          Serial.println("ERROR: Could not put RTC read time to queue.");  
        }
//          Serial.print( "\nRTC_READ_task: AFTER SEND systemTimeHandler.updateTime()\n");   
        
      } 
      
    }

  }
  
  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcWriteTask(void *pvParameter)
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcWriteMsg;
  MessageTime_t   bestSrcMsg;


  printTick();  Serial.print( "\nRTC_WRITE_task:  start\n");

  for(;;)
  { 
    vTaskDelay( 10 / portTICK_RATE_MS);
//    advisor.setSelectedSource( src_type_t::GPS);

    if (xQueueReceive( g_queueTimePattern, (void *)&rtcWriteMsg, 10) == pdTRUE) 
    {
      if( !advisor.routeSource( bestSrcMsg, rtcWriteMsg))
      {
        continue;
      }

      // set time for RTC
      rtcTimestamp.setEpochTime(  bestSrcMsg.epoch);
      {
        Timestamp timestamp;
        timestamp.setEpochTime( bestSrcMsg.epoch); 
        displayTimestamp( "RTC", timestamp); 
      }      
      if( xSemaphoreTake(  g_xSemaphoreRtc,0) == pdTRUE)
      {
//          Serial.print( "\nRTC_WRITE_task:  setTimestamp\n");
        g_SystemTimeHandler.setTimestamp( rtcTimestamp);

//        lastTimestamp= g_SystemTimeHandler.getTimestamp();
        xSemaphoreGive(  g_xSemaphoreRtc);
      }

    }

    
  }
  
  vTaskDelete(nullptr);
}  

//=============================================================================================================
void setup() 
{
  g_xSemaphoreRtc = xSemaphoreCreateMutex();
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.flush();

  g_ConsoleViewHandler.init();

  pinMode( gc_PULS_pin, OUTPUT);

  vTaskDelay( 3000 / portTICK_RATE_MS);
  Serial.print("setup: start ======================\n"); 

  xTaskCreate( &gpsTask,              "gps_task",         4048, nullptr, 5, nullptr);
  xTaskCreate( &ntpTask,              "ntp_task",         4048, nullptr, 5, nullptr);
  xTaskCreate( &manualAdjustmentTask, "manual_adj_task",  2048, nullptr, 5, nullptr);
  xTaskCreate( &rtcWriteTask,         "rtc_write_task",   2048, nullptr, 5, nullptr);
  xTaskCreate( &rtcReadTask,          "rtc_read_task",    2048, nullptr, 5, nullptr);
  xTaskCreate( &consoleInTask,        "console_in_task",  3048, nullptr, 5, nullptr);
  xTaskCreate( &consoleOutTask,       "console_out_task", 3048, nullptr, 5, nullptr);

}

//=============================================================================================================
void loop() 
{

}

