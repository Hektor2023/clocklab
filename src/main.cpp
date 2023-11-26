#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
  
#include <NTPClient.h>   // https://github.com/arduino-libraries/NTPClient

#include "Timestamp.h"
#include "ConsoleViewHandler.h"
#include "RTCSystemTimeHandler.h"
#include "SplitterTimeHandler.h"

#include "DSTSunriseSunsetTimeHandler.h"
#include "Controller.h"
#include "LEDClockViewHandler.h"

#include "OLEDDisplayClockViewHandler.h"
#include "GPSTimeHandler2.h"

#include "WifiCred.h"


const  char*   gc_Ssid { SSID };
const  char*   gc_Password{ PASSWD };

typedef uint32_t  epoch_t;

typedef struct {
  char      src[4];
  epoch_t   epoch;
  uint32_t  epochMillis;
  uint32_t  rtcMillis;
} MessageTime_t;

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

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0
constexpr auto       gc_GMT_Plus_2h{ 2* 3600};
constexpr auto       gc_period_1000_Millis{ 1000};


// Location Nowy Dworm Mazowiecki
double  g_Latitude{  52.4465078};  // 52.2507628, 020.4409067
double  g_Longitude{ 20.6925219};  // 

MyTime      g_SunriseTime, g_SunsetTime;
Timestamp   g_LocalTimestamp;


OLEDDisplayClockViewHandler g_OLEDViewHandler( nullptr);
//LEDClockViewHandler         g_LEDViewHandler(  nullptr, gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
LEDClockViewHandler         g_LEDViewHandler( &g_OLEDViewHandler, gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
ConsoleViewHandler          g_ConsoleViewHandler( &g_LEDViewHandler);

SplitterTimeHandler         g_SplitterHandler(nullptr, g_LocalTimestamp);  
DSTSunriseSunsetTimeHandler g_TimeZoneDSTHandler( &g_SplitterHandler, gc_GMT_Plus_2h, g_Longitude, g_Latitude, g_SunriseTime, g_SunsetTime);
RTCSystemTimeHandler        g_SystemTimeHandler(  &g_TimeZoneDSTHandler, gc_sda_pin, gc_scl_pin, gc_irqIn_pin);
 

GPSTimeHandler2             g_GPSHandler( nullptr, g_Longitude, g_Latitude);

Controller                  g_Controller( &g_SystemTimeHandler);

static xQueueHandle       g_queueTimePattern= xQueueCreate( 10, sizeof( MessageTime_t));
static xQueueHandle       g_queueDisplay=     xQueueCreate( 10, sizeof( MessageTime_t));
static SemaphoreHandle_t  g_xSemaphoreRtc;

//=============================================================================================================
void printTick(void)
{
  Serial.print( xTaskGetTickCount());
  Serial.print( ":\t");
}

//=============================================================================================================
void console_task(void *pvParameter)
{
  // initialize digital pin LED_BUILTIN as an output.
  Timestamp       displayTimestamp;
  MessageTime_t   rcvMsg;

  printTick();  Serial.print( "\nCONSOLE_task:  start\n");

  for(;;)
  {
    if (xQueueReceive( g_queueDisplay, (void *)&rcvMsg, 0) == pdTRUE) 
    {
      printTick();  Serial.print( "  console_task  ");  Serial.printf( "-> %s   ", rcvMsg.src);
      displayTimestamp.setEpochTime( rcvMsg.epoch);

      g_ConsoleViewHandler.updateLocation( g_Longitude, g_Latitude);
      g_ConsoleViewHandler.updateSunriseSunset( g_SunriseTime, g_SunsetTime);
      g_ConsoleViewHandler.updateTime( displayTimestamp);
      {
        static uint8_t lastDay=0;
        uint8_t currentDay= displayTimestamp.getDate().getDay();
        if( currentDay> lastDay)
        {
          lastDay= currentDay;

          char timeStrBuffer[ MyTime::getStringBufferSize()];
          Serial.printf("\t  => Sunrise: %s  ",   g_SunriseTime.toString( timeStrBuffer));
          Serial.printf("/      Sunset:  %s  \n", g_SunsetTime.toString( timeStrBuffer));  
        }
      }  

    }
    vTaskDelay( 10 / portTICK_RATE_MS);                    // wait for a second
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtc_read_task(void *pvParameter)
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcReadMsg={ .src= "RTC"};

  printTick();  Serial.print( "\nRTC_READ_task:  start\n");
  g_SystemTimeHandler.init();

  for(;;)
  { 
      if( xSemaphoreTake( g_xSemaphoreRtc,0) == pdTRUE)
      { 
        g_SystemTimeHandler.updateTime();
        xSemaphoreGive(  g_xSemaphoreRtc);

 //       Serial.print( "\nRTC_READ_task:  systemTimeHandler.updateTime() ");
        char timestampAsString[  g_LocalTimestamp.getStringBufferSize()];
//        Serial.printf( "\nRTC_READ_task:  systemTimeHandler.updateTime() = %s  \n",localTimestamp.toString( timestampAsString ));

        if( rtcReadMsg.epoch!= g_LocalTimestamp.getEpochTime())
        {
          rtcReadMsg.epoch= g_LocalTimestamp.getEpochTime();
          rtcReadMsg.epochMillis= 0;
          rtcReadMsg.rtcMillis= millis();

          while( xQueueSend( g_queueDisplay, (void *)&rtcReadMsg, 10) != pdTRUE) 
          {
            Serial.println("ERROR: Could not put RTC read time to queue.");  
          }

//          Serial.print( "\nRTC_READ_task: AFTER SEND systemTimeHandler.updateTime()\n");
        }  

      }

    vTaskDelay( 2 / portTICK_RATE_MS);
  }
  
  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtc_write_task(void *pvParameter)
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcWriteMsg;

  printTick();  Serial.print( "\nRTC_WRITE_task:  start\n");

  for(;;)
  { 
 
    if (xQueueReceive( g_queueTimePattern, (void *)&rtcWriteMsg, 10) == pdTRUE) 
    {
      
      rtcTimestamp.setEpochTime( rtcWriteMsg.epoch);
      // rtcTimestamp++;

      while( (millis() -rtcWriteMsg.rtcMillis) <= ( gc_period_1000_Millis -rtcWriteMsg.epochMillis))  
      { 
        taskYIELD(); 
      };

      if( !g_Controller.isAdjustMode()) 
      {
        if( xSemaphoreTake(  g_xSemaphoreRtc,0) == pdTRUE)
        {
//          Serial.print( "\nRTC_WRITE_task:  setTimestamp\n");
          g_SystemTimeHandler.setTimestamp( rtcTimestamp);
          xSemaphoreGive(  g_xSemaphoreRtc);
        }
      }

    }

    vTaskDelay( 10 / portTICK_RATE_MS);
  }
  
  vTaskDelete(nullptr);
}  

//=============================================================================================================
void ntp_task(void *pvParameter)
{
  WiFiUDP     udp;
  NTPClient   timeClient( udp);


  MessageTime_t   ntp_msg={ .src= "NTP"};

  printTick();  Serial.print( "\nNTP_task:  start\n"); 

  WiFi.mode(WIFI_STA); 
  
  for(;;)
  { 
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
    ntp_msg.epochMillis= (uint32_t)((1000.0f* timeClient.getMillis())/UINT32_MAX);  //  Serial.printf("millis => %u\n", epochMillis);
    ntp_msg.rtcMillis= millis();

    Timestamp timestamp;
    timestamp.setEpochTime( ntp_msg.epoch); 
    char timestampAsString[  timestamp.getStringBufferSize()];
    Serial.printf( "\nNTP Timestamp= %s  \n",timestamp.toString( timestampAsString ));

    while ( xQueueSend( g_queueTimePattern, (void *)&ntp_msg, 10) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

     timeClient.end();

  //  Serial.printf("\nDisconnect...\n");
    WiFi.disconnect();
     
    vTaskDelay( 10*1000 / portTICK_RATE_MS);
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void gps_task(void *pvParameter)
{
  char c;
  MessageTime_t   gps_msg={ .src= "GPS"};
  Serial2.begin(9600);
  Serial2.flush();

  vTaskDelay( 800 / portTICK_RATE_MS);
  printTick();  Serial.print( "\nGPS_task:  start\n");  

  for(;;)
  {
    Serial.printf("| GPS...|");

    bool next= true;
    while( next)
    {
      if( !Serial2.available())  
      {
//        Serial.printf( "GPS: !Serial2.available\n");
        Serial.printf( "*");
        vTaskDelay( 40 / portTICK_RATE_MS);
        continue;
      }

      c= Serial2.read();
      next= g_GPSHandler.collectRecord( c);
    };

//    Serial.printf( "GPS: %s\n", buffer);
    if( g_GPSHandler.updateTime())
    {
//        Serial.printf( "GPS: encoded\n");
      gps_msg.epoch=  g_GPSHandler.getTimestamp().getEpochTime(); 
      gps_msg.epochMillis= (uint32_t) g_GPSHandler.getMilliSecond();  //  Serial.printf("millis => %u\n", epochMillis);
      gps_msg.rtcMillis= millis();
      while ( xQueueSend( g_queueTimePattern, (void *)&gps_msg, 10) != pdTRUE) 
      {
        Serial.println("ERROR: Could not put GPS time to queue."); 
        vTaskDelay( 2 / portTICK_RATE_MS); 
      }
    }

    vTaskDelay(  100 / portTICK_RATE_MS);
//    Serial.printf("| GPS...!!!!|\n");
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
    digitalWrite( gc_SI_pin, k %8 ==0? HIGH:LOW);
    k++;
    
    digitalWrite( gc_SCK_pin, HIGH);
    digitalWrite( gc_RCK_pin, LOW);
    vTaskDelay( 5 / portTICK_RATE_MS);

    digitalWrite( gc_SCK_pin, LOW);
    digitalWrite( gc_RCK_pin, HIGH);

    vTaskDelay( 5 / portTICK_RATE_MS);
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

  vTaskDelay( 3000 / portTICK_RATE_MS);
  Serial.print("setup: start ======================\n"); 

  xTaskCreate( &gps_task,       "gps_task",       4048, nullptr, 5, nullptr);
  xTaskCreate( &ntp_task,       "ntp_task",       4048, nullptr, 5, nullptr);
  xTaskCreate( &rtc_write_task, "rtc_write_task", 2048, nullptr, 5, nullptr);
  
  xTaskCreate( &rtc_read_task,  "rtc_read_task",  2048, nullptr, 5, nullptr);
  xTaskCreate( &console_task,   "console_task",   3048, nullptr, 5, nullptr);
}

typedef 
enum
{ 
  eMode     =  0x01,

  eYear     =  0x02,
  eMonth    =  0x04,
  eDay      =  0x08,

  eHour     =  0x10,
  eMinute   =  0x20,

  ePlus     =  0x40,
  eMinus    =  0x80,
}  t_Buttons;

uint8_t buttons=0;

void loop() {
  buttons= g_LEDViewHandler.buttonsRead();

  if( buttons!= 0)  
  {
    Serial.printf("| Button..|  %x\n", buttons);
  }

  switch( (t_Buttons)buttons)
  {
    case  eMode:     
          if( !g_Controller.isAdjustMode())
          {
            Serial.printf("| ADJUST|\n");
            g_Controller.execute( "stop");
            g_LEDViewHandler.modeAdjust( true);
          }
          else
          {
            Serial.printf("| COUNTING|\n");
            g_Controller.execute( "start");
            g_LEDViewHandler.modeAdjust( false);
          }
          break;

//    case  eYear:      controller.execute("year");   break;
//    case  eMonth:     controller.execute("month");  break;
//    case  eDay:       controller.execute("day");    break;
    case  eHour:      g_Controller.execute("hour");   break;
    case  eMinute:    g_Controller.execute("minute"); break;

    case  ePlus:      g_Controller.execute("+");      break;
    case  eMinus:     g_Controller.execute("-");      break;

    default:
        break;
  }

  vTaskDelay( 100 / portTICK_RATE_MS);

}

