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

const  char*   ssid { SSID };
const  char*   password{ PASSWD };

typedef uint32_t  epoch_t;

typedef struct {
  char      src[4];
  epoch_t   epoch;
  uint32_t  epochMillis;
  uint32_t  rtcMillis;
} MessageTime_t;

constexpr int xCoreId_0{ 0};
constexpr int xCoreId_1{ 1};

constexpr uint8_t   irqIn_pin{ 15}; // D8 - don't use D4

constexpr uint8_t   rxd2_pin{17};
constexpr uint8_t   txd2_pin{16};

constexpr uint8_t   sda_pin{21};
constexpr uint8_t   scl_pin{22};

constexpr uint8_t   DIO_pin{ 13}; // 25
constexpr uint8_t   CLK_pin{ 33}; // 26
constexpr uint8_t   STB_pin{ 32}; // 27


constexpr uint8_t   SI_pin{25};
constexpr uint8_t   RCK_pin{26};
constexpr uint8_t   SCK_pin{27};
constexpr uint8_t   NSCLR_pin{14};

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0
constexpr auto       GMT_Plus_2h{ 2* 3600};
constexpr auto       period_1000_Millis{ 1000};


// Location Nowy Dworm Mazowiecki
double  latitude{  52.4465078};  // 52.2507628, 020.4409067
double  longitude{ 20.6925219};  // 

MyTime      sunriseTime, sunsetTime;
Timestamp   localTimestamp;

//OLEDDisplayClockViewHandler OLEDViewHandler( NULL, sda_pin2, scl_pin2);

LEDClockViewHandler         LEDViewHandler( NULL, STB_pin, CLK_pin, DIO_pin);
ConsoleViewHandler          consoleViewHandler( &LEDViewHandler);

SplitterTimeHandler         splitterHandler(NULL, localTimestamp);  
DSTSunriseSunsetTimeHandler timeZoneDSTHandler( &splitterHandler, GMT_Plus_2h, longitude, latitude, sunriseTime, sunsetTime);
RTCSystemTimeHandler        systemTimeHandler(  &timeZoneDSTHandler, sda_pin, scl_pin, irqIn_pin);
 
GPSTimeHandler2             GPSHandler( NULL);

Controller                  controller( &systemTimeHandler);

static xQueueHandle       queueTimePattern= xQueueCreate( 10, sizeof( MessageTime_t));
static xQueueHandle       queueDisplay=     xQueueCreate( 10, sizeof( MessageTime_t));
static SemaphoreHandle_t  xSemaphoreRtc;

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
  Timestamp displayTimestamp;
  MessageTime_t   rcvMsg;

  printTick();  Serial.print( "\nCONSOLE_task:  start\n");

  for(;;)
  {
    if (xQueueReceive( queueDisplay, (void *)&rcvMsg, 0) == pdTRUE) 
    {
      printTick();  Serial.print( "  console_task  ");  Serial.printf( "-> %s   ", rcvMsg.src);
      displayTimestamp.setEpochTime( rcvMsg.epoch);

      consoleViewHandler.updateTime( displayTimestamp);
      {
        static uint8_t lastDay=0;
        uint8_t currentDay= displayTimestamp.getDate().getDay();
        if( currentDay> lastDay)
        {
          lastDay= currentDay;

          char timeStrBuffer[ MyTime::getStringBufferSize()];
          Serial.printf("\t  => Sunrise: %s  ",   sunriseTime.toString( timeStrBuffer));
          Serial.printf("/      Sunset:  %s  \n", sunsetTime.toString( timeStrBuffer));  
        }
      }  

    }
    vTaskDelay( 10 / portTICK_RATE_MS);                    // wait for a second
  }

  vTaskDelete(NULL);
}

//=============================================================================================================
void rtc_read_task(void *pvParameter)
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcReadMsg={ .src= "RTC"};

  printTick();  Serial.print( "\nRTC_READ_task:  start\n");


  systemTimeHandler.init();


  for(;;)
  { 
      if( xSemaphoreTake( xSemaphoreRtc,0) == pdTRUE)
      { 
        systemTimeHandler.updateTime();
        xSemaphoreGive(  xSemaphoreRtc);

        if( rtcReadMsg.epoch!= localTimestamp.getEpochTime())
        {
          rtcReadMsg.epoch= localTimestamp.getEpochTime();
          rtcReadMsg.epochMillis= 0;
          rtcReadMsg.rtcMillis= millis();

          while( xQueueSend( queueDisplay, (void *)&rtcReadMsg, 10) != pdTRUE) 
          {
            Serial.println("ERROR: Could not put RTC read time to queue.");  
          }
        }  

      }

    vTaskDelay( 2 / portTICK_RATE_MS);
  }
  
  vTaskDelete(NULL);
}

//=============================================================================================================
void rtc_write_task(void *pvParameter)
{
  Timestamp       rtcTimestamp;
  MessageTime_t   rtcWriteMsg;

  printTick();  Serial.print( "\nRTC_WRITE_task:  start\n");

  for(;;)
  { 
 
    if (xQueueReceive( queueTimePattern, (void *)&rtcWriteMsg, 10) == pdTRUE) 
    {

      rtcTimestamp.setEpochTime( rtcWriteMsg.epoch);
      // rtcTimestamp++;

//      while( (millis() -rtcWriteMsg.rtcMillis) <= ( period_1000_Millis -rtcWriteMsg.epochMillis))  
//      { 
//        taskYIELD(); 
//      };

      if( !controller.isAdjustMode()) 
      {
        if( xSemaphoreTake(  xSemaphoreRtc,0) == pdTRUE)
        {
          systemTimeHandler.setTimestamp( rtcTimestamp);
          xSemaphoreGive(  xSemaphoreRtc);
        }
      }

    }

    vTaskDelay( 10 / portTICK_RATE_MS);
  }
  
  vTaskDelete(NULL);
}  

//=============================================================================================================
void ntp_task(void *pvParameter)
{
  WiFiUDP     udp;
  NTPClient   timeClient( udp);


  MessageTime_t   ntp_msg={ .src= "NTP"};

  printTick();  Serial.print( "\nNTP_task:  start\n"); 

  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, password);

  for(;;)
  { 
  //  Serial.printf("\nTrying to connect...\n");  
    WiFi.begin(ssid, password);
    while ( !WiFi.isConnected())
    {
      taskYIELD();
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
    Serial.printf( "\nNTP TimestampAsString= %s  \n",timestamp.toString( timestampAsString ));

    while ( xQueueSend( queueTimePattern, (void *)&ntp_msg, 10) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

     timeClient.end();

  //  Serial.printf("\nDisconnect...\n");
    WiFi.disconnect();
     
    vTaskDelay( 10*1000 / portTICK_RATE_MS);
  }

  vTaskDelete( NULL);
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

  char buffer[100]; 
 
  for(;;)
  {
//    Serial.printf("| GPS...|");

    bool next= true;
    while( next)
    {
      if( !Serial2.available())  
      {
        vTaskDelay( 5 / portTICK_RATE_MS);
        continue;
      }

      c= Serial2.read();
      next= GPSHandler.collectRecord( c, buffer, (uint8_t)sizeof( buffer));
    };

//    Serial.printf( "GPS: %s\n", buffer);
    GPSHandler.updateTime( buffer);

//        Serial.printf( "GPS: encoded\n");
    gps_msg.epoch=  GPSHandler.getTimestamp().getEpochTime(); 
    gps_msg.epochMillis= (uint32_t) GPSHandler.getMilliSecond();  //  Serial.printf("millis => %u\n", epochMillis);
    gps_msg.rtcMillis= millis();
    while ( xQueueSend( queueTimePattern, (void *)&gps_msg, 10) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put GPS time to queue."); 
      vTaskDelay( 2 / portTICK_RATE_MS); 
    }

    vTaskDelay(  100 / portTICK_RATE_MS);
//    Serial.printf("| GPS...!!!!|\n");
  }

  vTaskDelete( NULL);
}

//=============================================================================================================
void keyboard_task(void *pvParameter)
{
  vTaskDelay( 20000 / portTICK_RATE_MS);

  pinMode( SI_pin, OUTPUT);
  pinMode( NSCLR_pin, OUTPUT);
  pinMode( RCK_pin, OUTPUT);
  pinMode( SCK_pin, OUTPUT);


  digitalWrite( NSCLR_pin, HIGH);
  digitalWrite( RCK_pin, LOW);
  digitalWrite( SCK, LOW);
  digitalWrite( SI_pin,HIGH);

  uint8_t k=0;

  for(;;)
  {
    digitalWrite( SI_pin, k %8 ==0? HIGH:LOW);
    k++;
    
    digitalWrite( SCK_pin, HIGH);
    digitalWrite( RCK_pin, LOW);
    vTaskDelay( 5 / portTICK_RATE_MS);

    digitalWrite( SCK_pin, LOW);
    digitalWrite( RCK_pin, HIGH);

    vTaskDelay( 5 / portTICK_RATE_MS);
  }

  vTaskDelete(NULL);
}

//=============================================================================================================
void setup() 
{
  xSemaphoreRtc = xSemaphoreCreateMutex();
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.flush();

  vTaskDelay( 3000 / portTICK_RATE_MS);
  Serial.print("setup: start ======================\n"); 

  xTaskCreate( &gps_task,       "gps_task",       4048, NULL, 5, NULL);
  xTaskCreate( &ntp_task,       "ntp_task",       3048, NULL, 5, NULL);
  xTaskCreate( &rtc_write_task, "rtc_write_task", 2048, NULL, 5, NULL);
  
  xTaskCreate( &rtc_read_task,  "rtc_read_task",  2048, NULL, 5, NULL);
  xTaskCreate( &console_task,   "console_task",   3048, NULL, 5, NULL);

//  OLEDViewHandler.init();

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
}  eButtons;

uint8_t buttons=0;

void loop() {
  buttons= LEDViewHandler.buttonsRead();

  if( buttons!= 0)  
  {
    Serial.printf("| Button..|  %x\n", buttons);
  }

  switch( (eButtons)buttons)
  {
    case  eMode:     
          if( !controller.isAdjustMode())
          {
            Serial.printf("| ADJUST|\n");
            controller.execute( "stop");
            LEDViewHandler.modeAdjust( true);
          }
          else
          {
            Serial.printf("| COUNTING|\n");
            controller.execute( "start");
            LEDViewHandler.modeAdjust( false);
          }
          break;

//    case  eYear:      controller.execute("year");   break;
//    case  eMonth:     controller.execute("month");  break;
//    case  eDay:       controller.execute("day");    break;
    case  eHour:      controller.execute("hour");   break;
    case  eMinute:    controller.execute("minute"); break;

    case  ePlus:      controller.execute("+");      break;
    case  eMinus:     controller.execute("-");      break;

    default:
        break;
  }

  vTaskDelay( 100 / portTICK_RATE_MS);

}

