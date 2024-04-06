#include <Arduino.h>

#include <SPI.h>
#include <string.h>
#include "clocklab_types.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Tools.h"

#include "TimeType/Timestamp.h"
#include "TimeType/TimeData.h"

#include "Source/NTP/NTPTask.h"
#include "Source/RTCSystemTimeHandler.h"
#include "Source/GPS/GPSTimeHandler2.h"
#include "Source/ManualTimeHandler.h"

#include "Converter/SplitterTimeHandler.h"
#include "Converter/DSTSunriseSunsetTimeHandler.h"

#include "AdjustmentAdvisor.h"

#include "Controller.h"

#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"
#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"
#include "Display/ConsoleDisplay/ConsoleDisplayHandler.h"


#include "WifiCred.h"

//=============================================================================================================

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


static TimeData  timeData;

// Location Nowy Dworm Mazowiecki
//double  g_Latitude{  52.4465078};  // 52.2507628, 020.4409067
//double  g_Longitude{ 20.6925219};  // 
Coordinates_t g_coordinates{  52.4465078, 20.6925219};

MyTime      g_SunriseTime, g_SunsetTime;

Timestamp                   g_LocalTimestamp;
SplitterTimeHandler         g_SplitterHandler( nullptr, g_LocalTimestamp);  
DSTSunriseSunsetTimeHandler g_TimeZoneDSTHandler( &g_SplitterHandler, gc_GMT_Plus_2h, g_coordinates, g_SunriseTime, g_SunsetTime);
RTCSystemTimeHandler        g_SystemTimeHandler(  &g_TimeZoneDSTHandler, gc_sda_pin, gc_scl_pin, gc_irqIn_pin);
 
Controller                  g_Controller;

static xQueueHandle       g_queueSourceTime= xQueueCreate( 5, sizeof( MessageTime_t));
static xQueueHandle       g_queueDisplay=    xQueueCreate( 15, sizeof( MessageTime_t));
static SemaphoreHandle_t  g_xSemaphoreRtc;

AdjustmentAdvisor         g_advisor;


static ntpTaskParams_t  ntpParams{ SSID, PASSWD, static_cast< QueueHandle_t* >( &g_queueSourceTime)};

//=============================================================================================================
void OLedDisplayTask(void *pvParameter)
{
  OLEDClockDisplayHandler OLEDClockDisplayHandler;
  TimeData *ptr2timeData= reinterpret_cast< TimeData*>( pvParameter);
  MyTime lastTime;

  printTick();  
  Serial.print( "\nOLED_DISPLAY_task:  start\n");

  OLEDClockDisplayHandler.init();

  for(;;)
  {
    vTaskDelay( 30 / portTICK_RATE_MS);

    if(ptr2timeData->lockData())
    {
      if( ptr2timeData->localTime != lastTime)
      {
        OLEDClockDisplayHandler.update( *ptr2timeData);

        lastTime =ptr2timeData->localTime;
      }

      ptr2timeData->releaseData();
    }

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void LedDisplayTask(void *pvParameter)
{
  static LEDClockDisplayHandler LedDisplayHandler( gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
  TimeData *ptr2timeData= reinterpret_cast< TimeData*>( pvParameter);
  MyTime lastTime;

  printTick();  
  Serial.print( "\nLED_DISPLAY_task:  start\n");


  for(;;)
  {
    vTaskDelay( 30 / portTICK_RATE_MS);

    if( ptr2timeData->lockData())
    {
      if( ptr2timeData->localTime != lastTime)
      {
        LedDisplayHandler.update( *ptr2timeData);

        lastTime = ptr2timeData->localTime;
      }

      timeData.releaseData();
    }

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void consoleDisplayTask(void *pvParameter)
{
  static ConsoleDisplayHandler consoleDisplayHandler;
  TimeData *ptr2timeData= reinterpret_cast< TimeData*>( pvParameter);
  MyTime lastTime;

  printTick();  
  Serial.print( "\nCONSOLE_DISPLAY_task:  start\n");

  for(;;)
  {
    vTaskDelay( 30 / portTICK_RATE_MS);

    if( ptr2timeData->lockData())
    {
      if( ptr2timeData->localTime != lastTime)
      {
        consoleDisplayHandler.update( *ptr2timeData);

        lastTime = ptr2timeData->localTime;
      }

      timeData.releaseData();
    }

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void consoleOutTask(void *pvParameter)
{
  // initialize digital pin LED_BUILTIN as an output.
  Timestamp       displayTimestamp;
  MessageTime_t   rcvMsg;
  MyDate date;

  printTick();  
  Serial.print( "\nCONSOLE_OUT_task:  start\n");

  for(;;)
  {
    vTaskDelay( 30 / portTICK_RATE_MS);                  
    if( timeData.lockData())
    {
      if (xQueueReceive( g_queueDisplay, (void *)&rcvMsg, 0) == pdTRUE)
      {
        displayTimestamp.setEpochTime( rcvMsg.epoch);

        displayTimestamp.getDate( date);
   
        displayTimestamp.getTime( timeData.localTime);
        displayTimestamp.getDate( timeData.localDate);
      } 

      timeData.releaseData();
    }
    

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void gpsTask(void *pvParameter)
{
  GPSTimeHandler2  g_GPSHandler( nullptr, g_coordinates);  

  MessageTime_t   gps_msg;
  gps_msg.type=   src_type_t::GPS;

  Timestamp   lastGpsTime;
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

      char c = Serial2.read();
      next= g_GPSHandler.collectRecord( c);
    };

    if( !g_GPSHandler.updateTime())
    {
      continue;
    }

    Timestamp diffTime= g_GPSHandler.getTimestamp() - lastGpsTime;
    displayTimestamp( "diffTime", diffTime);
    if( diffTime.getEpochTime() < (1 * 60))
    {
      continue;
    }

    lastGpsTime = g_GPSHandler.getTimestamp();

//    Serial.printf( "GPS: encoded\n");
    gps_msg.epoch=  g_GPSHandler.getTimestamp().getEpochTime(); 
    uint32_t  epochMillis= (uint32_t) g_GPSHandler.getMilliSecond();
   
    // set next entire second
    vTaskDelay( ( 1000 -  epochMillis)/ portTICK_RATE_MS);
  

    Timestamp timestamp( gps_msg.epoch); 
    displayTimestamp( "GPS", timestamp);
     
    while ( xQueueSend( g_queueSourceTime, (void *)&gps_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put GPS time to queue."); 
      vTaskDelay( 2 / portTICK_RATE_MS); 
    }
//    Serial.printf("| GPS...!!!!|\n");

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
        Serial.print( "RTC: UPDATE\t");
      
        digitalWrite( gc_PULS_pin,  gc_PULS_pin? LOW:HIGH); 

        // set time for Display
        rtcReadMsg.epoch= g_LocalTimestamp.getEpochTime();
        
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
//    g_advisor.setSelectedSource( src_type_t::GPS);

    if (xQueueReceive( g_queueSourceTime, (void *)&rtcWriteMsg, 10) == pdTRUE) 
    {
      if( !g_advisor.routeSource( bestSrcMsg, rtcWriteMsg))
      {
        continue;
      }

      // set time for RTC
      rtcTimestamp.setEpochTime(  bestSrcMsg.epoch);
      {
        Timestamp timestamp( bestSrcMsg.epoch);
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

  vTaskDelay( 3000 / portTICK_RATE_MS);

  pinMode( gc_PULS_pin, OUTPUT);


  Serial.print("setup: start ======================\n"); 

//  xTaskCreate( &gpsTask,              "gps_task",         4048, nullptr, 5, nullptr);
  xTaskCreate( &ntpTask,              "ntp_task",         4048,  &ntpParams, 5, nullptr);

  xTaskCreate( &rtcWriteTask,         "rtc_write_task",   2048, nullptr, 5, nullptr);
  xTaskCreate( &rtcReadTask,          "rtc_read_task",    2048, nullptr, 5, nullptr);

  xTaskCreate( &consoleOutTask,       "console_out_task",     3048, nullptr, 5, nullptr);
  xTaskCreate( &consoleDisplayTask,   "console_display_task", 2048,  &timeData, 5, nullptr);
  xTaskCreate( &LedDisplayTask,       "LED_display_task",     2048,  &timeData, 5, nullptr);
  xTaskCreate( &OLedDisplayTask,      "OLED_display_task",    3048,  &timeData, 5, nullptr);
}

//=============================================================================================================
void loop() 
{

}

