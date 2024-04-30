#include <Arduino.h>
#include <u8g2lib.h>
#include <MUIU8g2.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "clocklab_types.h"
#include <SPI.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Other/CoordinatesHandler.h"

#include "Other/AdjustmentAdvisor.h"
#include "Other/Controller.h"
#include "Other/Tools.h"

#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"

#include "Display/ConsoleDisplay/ConsoleDisplayTask.h"
#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"

#include "Source/GPS/GPSTask.h"
#include "Source/NTP/NTPTask.h"
#include "Source/RTCSystemTimeHandler.h"

#include "Converter/DSTSunriseSunsetTimeHandler.h"
#include "Converter/SplitterTimeHandler.h"

#include "PinConfig.h"
#include "WifiCred.h"

//=============================================================================================================
// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0
constexpr auto gc_GMT_Plus_2h{2 * 3600};
constexpr auto gc_period_1000_Millis{1000};

static TimeData timeData;

// Location Nowy Dworm Mazowiecki
// double  g_Latitude{  52.4465078};  // 52.2507628, 020.4409067
// double  g_Longitude{ 20.6925219};  //

MyTime g_SunriseTime, g_SunsetTime;

Timestamp g_LocalTimestamp;
SplitterTimeHandler g_SplitterHandler(nullptr, g_LocalTimestamp);
DSTSunriseSunsetTimeHandler g_TimeZoneDSTHandler(&g_SplitterHandler, gc_GMT_Plus_2h, g_SunriseTime, g_SunsetTime);
RTCSystemTimeHandler g_RTCSystemTimeHandler(&g_TimeZoneDSTHandler, gc_sda_pin, gc_scl_pin, gc_irqIn_pin);

static xQueueHandle g_queueSourceTime = xQueueCreate(3, sizeof(MessageTime_t));
static xQueueHandle g_queueDisplay = xQueueCreate(3, sizeof(MessageTime_t));
static SemaphoreHandle_t g_xSemaphoreRtc;

AdjustmentAdvisor g_advisor;

//=============================================================================================================
void consoleOutTask(void *pvParameter)
{
  //  UBaseType_t uxHighWaterMark; // 2320
  //  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);

  // initialize digital pin LED_BUILTIN as an output.
  Timestamp displayTimestamp;
  MessageTime_t rcvMsg;
  MyDate date;

  printTick();
  Serial.print("\nCONSOLE_OUT_task:  start\n");

  for (;;)
  {
    vTaskDelay(30 / portTICK_RATE_MS);
    if (timeData.lockData())
    {
      if (xQueueReceive(g_queueDisplay, (void *)&rcvMsg, 0) == pdTRUE)
      {
        displayTimestamp.setEpochTime(rcvMsg.epoch);

        displayTimestamp.getDate(date);

        displayTimestamp.getTime(timeData.localTime);
        displayTimestamp.getDate(timeData.localDate);
      }

      timeData.releaseData();
    }

    //    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //    Serial.printf("\nConsoleOut_task:  uxHighWaterMark= %d\n", uxHighWaterMark);
    //    break;
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcReadTask(void *pvParameter)
{
  //  UBaseType_t uxHighWaterMark; // 968
  //  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);

  Timestamp rtcTimestamp;
  MessageTime_t rtcReadMsg;

  printTick();
  Serial.print("\nRTC_READ_task:  start\n");
  g_RTCSystemTimeHandler.init();

  for (;;)
  {
    vTaskDelay(10 / portTICK_RATE_MS);

    if (xSemaphoreTake(g_xSemaphoreRtc, 0) == pdTRUE)
    {
      bool timeUpdated = g_RTCSystemTimeHandler.updateTime();
      xSemaphoreGive(g_xSemaphoreRtc);

      // g_LocalTimestamp updated by g_SystemTimeHandler
      if (timeUpdated)
      {
        Serial.print("RTC: UPDATE\t");

        digitalWrite(gc_PULS_pin, gc_PULS_pin ? LOW : HIGH);

        // set time for Display
        rtcReadMsg.epoch = g_LocalTimestamp.getEpochTime();
        while (xQueueSend(g_queueDisplay, (void *)&rtcReadMsg, 0) != pdTRUE)
        {
          Serial.println("ERROR: Could not put RTC read time to queue.");
        }
        //          Serial.print( "\nRTC_READ_task: AFTER SEND
        //          systemTimeHandler.updateTime()\n");
      }
    }

    //    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //    Serial.printf("\nRTC_READ_task:  uxHighWaterMark= %d\n", uxHighWaterMark);
    //    break;
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcWriteTask(void *pvParameter)
{
  //  UBaseType_t uxHighWaterMark; // 1276
  //  uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);

  QueueHandle_t *ptr2queueSource = reinterpret_cast<QueueHandle_t *>(pvParameter);
  Timestamp rtcTimestamp;
  MessageTime_t rtcWriteMsg;
  MessageTime_t bestSrcMsg;

  printTick();
  Serial.print("\nRTC_WRITE_task:  start\n");

  for (;;)
  {
    vTaskDelay(10 / portTICK_RATE_MS);
    g_advisor.setSelectedSource(src_type_t::NTP);

    if (xQueueReceive(*ptr2queueSource, (void *)&rtcWriteMsg, 0) == pdTRUE)
    {
      if (g_advisor.routeSource(bestSrcMsg, rtcWriteMsg))
      {
        if ((xSemaphoreTake(g_xSemaphoreRtc, 0) == pdTRUE))
        {
          rtcTimestamp.setEpochTime(bestSrcMsg.epoch);
          g_RTCSystemTimeHandler.setTimestamp(rtcTimestamp);

          Timestamp timestamp(bestSrcMsg.epoch);
          displayTimestamp("RTC", timestamp);

          if (bestSrcMsg.type == src_type_t::GPS)
          {
            CoordinatesHandler &handler = g_TimeZoneDSTHandler.getCoordinatesHander();
            if (handler.lockData())
            {
              handler.setCoordinates(
                  bestSrcMsg.coordinate); // TODO: move to RTC SystemTimeHandler
              handler.releaseData();
            }
          }
          xSemaphoreGive(g_xSemaphoreRtc);
        }
      }
    }

    //    uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    //    Serial.printf("\nRTC_WRITE_task:  uxHighWaterMark= %d\n", uxHighWaterMark);
    //    break;
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void setup()
{
  static QueueHandle_t *ptr2src_queue = static_cast<QueueHandle_t *>(&g_queueSourceTime);
  static ntpTaskParams_t ntpParams{SSID, PASSWD, ptr2src_queue};

  g_xSemaphoreRtc = xSemaphoreCreateMutex();

  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.flush();

  vTaskDelay(3000 / portTICK_RATE_MS);
  pinMode(gc_PULS_pin, OUTPUT);

  Serial.print("setup: start ======================\n");

  xTaskCreate(&gpsTask, "gps_task", 5048, ptr2src_queue, 5, nullptr);
  xTaskCreate(&ntpTask, "ntp_task", 4048, &ntpParams, 5, nullptr);

  xTaskCreate(&rtcWriteTask, "rtc_write_task", 2048, ptr2src_queue, 5, nullptr); //
  xTaskCreate(&rtcReadTask, "rtc_read_task", 2048, nullptr, 5, nullptr);         // 2048

  xTaskCreate(&consoleOutTask, "console_out_task", 2600, nullptr, 5, nullptr); // 3048
  xTaskCreate(&consoleDisplayTask, "console_display_task", 2048, &timeData, 5, nullptr);
  xTaskCreate(&LedDisplayTask, "LED_display_task", 2048, &timeData, 5, nullptr);
  xTaskCreate(&OLedDisplayTask, "OLED_display_task", 3048, &timeData, 5, nullptr);
}

//=============================================================================================================
void loop() {}
