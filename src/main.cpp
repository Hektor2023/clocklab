#include <Arduino.h>
#include <array>

#include "PinConfig.h"
#include "WifiCred.h"

#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"
#include "clocklab_types.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Converter/ConverterTimestampAdapter.h"
#include "Converter/DSTSunriseSunsetTimeHandler.h"
#include "Converter/DisplayController.h"

#include "Source/GPS/GPSTask.h"
#include "Source/NTP/NTPTask.h"
#include "Source/RTCSystemTimeHandler.h"

#include "Display/ConsoleDisplay/ConsoleDisplayTask.h"
#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"

#include "Other/AdjustmentAdvisor.h"
#include "Other/Controller.h"
#include "Other/Tools.h"

//=============================================================================================================
// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0

static constexpr auto oneHour{ 3600};
static constexpr auto gc_GMT_Plus_2h{2 * oneHour};

static TimeData timeData;

// Location Nowy Dworm Mazowiecki
// double  g_Latitude{  52.4465078};  // 52.2507628, 020.4409067
// double  g_Longitude{ 20.6925219};  //

static RTCSystemTimeHandler g_RTCSystemTimeHandler(gc_sda_pin, gc_scl_pin, gc_irqIn_pin);

static xQueueHandle g_queueSourceTime = xQueueCreate(3, sizeof(MessageTime_t));
static SemaphoreHandle_t g_xSemaphoreRtc;

static AdjustmentAdvisor g_advisor;

//=============================================================================================================
void consoleOutTask(void *pvParameter) {
  printTick();  Serial.print("\nCONSOLE_OUT_task:  start\n");

  for (;;) 
  {
    vTaskDelay(30 / portTICK_RATE_MS);
    if (timeData.lockData()) 
      {
        //        displayTimestamp.setEpochTime(rcvMsg.epoch);

        //        timeData.localTime = displayTimestamp.getTime();
        //        timeData.localDate = displayTimestamp.getDate();
      }

      timeData.unlockData();
//   
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcReadTask(void *pvParameter) { // set new time for Displays via DisplayController
  static ConverterTimestampAdapter timestampAdapter;
  static DSTSunriseSunsetTimeHandler dstHandlder(gc_GMT_Plus_2h);
  static DisplayController displController;

  Timestamp rtcTimestamp;

  printTick(); Serial.print("\nRTC_READ_task:  start\n");

  g_RTCSystemTimeHandler.init();

  static bool needUpdate = true;
  static uint8_t lastSec2 = 0;

  for (;;) 
  {
    vTaskDelay(10 / portTICK_RATE_MS);

    if (xSemaphoreTake(g_xSemaphoreRtc, 0) == pdTRUE) 
    {
      
      g_RTCSystemTimeHandler.refresh();
      if (lastSec2 != g_RTCSystemTimeHandler.getSeconds()) 
      {
        lastSec2 = g_RTCSystemTimeHandler.getSeconds();
        needUpdate = true;
      }

      if (needUpdate && timeData.lockData()) 
      {
        needUpdate = false;

        g_RTCSystemTimeHandler.forceUpdateTime();
        //        Serial.print("\nRTC: UPDATE\t");
        digitalWrite(gc_PULS_pin, gc_PULS_pin ? LOW : HIGH);

        rtcTimestamp = g_RTCSystemTimeHandler.getTimestamp();
        displayTimestamp("RTC2", rtcTimestamp);

        timestampAdapter.setTimestamp(rtcTimestamp);

        timestampAdapter.update(timeData); // sends Timestamp to timedata
        dstHandlder.update(timeData);      // modify DSTTimes, sunrise, sunset
        displController.update( timeData); // sends selected timedata to displays via cmd

        DisplayCommand cmd = displController.getCommand();
        auto msg = cmd.getMessage();
        Serial.printf("\nDisplay Cmd: %s\n", msg.c_str());

        timeData.unlockData();
      }

      xSemaphoreGive(g_xSemaphoreRtc);
    }

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcWriteTask(void *pvParameter) { // sets new RTC time 
  QueueHandle_t *ptr2queueSource = reinterpret_cast<QueueHandle_t *>(pvParameter);

  MessageTime_t rtcWriteMsg;
  MessageTime_t bestSrcMsg;

  printTick();
  Serial.print("\nRTC_WRITE_task:  start\n");

  for (;;) {
    vTaskDelay(10 / portTICK_RATE_MS);
    g_advisor.setSelectedSource(src_type_t::GPS);

    if (xQueueReceive(*ptr2queueSource, (void *)&rtcWriteMsg, 0) == pdTRUE) 
    {
      if (g_advisor.routeSource(bestSrcMsg, rtcWriteMsg)) 
      {
        if ((xSemaphoreTake(g_xSemaphoreRtc, 0) == pdTRUE)) 
        {
          Timestamp rtcTimestamp;
          rtcTimestamp.setEpochTime(bestSrcMsg.epoch);

          g_RTCSystemTimeHandler.setTimestamp(rtcTimestamp);
          {
            Timestamp timestamp(bestSrcMsg.epoch);
            displayTimestamp("rtcWriteTask RTC:", timestamp);
          }

          if (bestSrcMsg.type == src_type_t::GPS) 
          {
            if (timeData.lockData()) 
            {
              timeData.coordinates = bestSrcMsg.coordinate;
              timeData.unlockData();
            }

          }

          xSemaphoreGive(g_xSemaphoreRtc);
        }

      }

    }

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void setup() {
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
  xTaskCreate(&rtcReadTask, "rtc_read_task", 2048, nullptr, 5, nullptr); // 2048

  //xTaskCreate(&consoleOutTask, "console_out_task", 2600, nullptr, 5, nullptr); // 3048
  xTaskCreate(&consoleDisplayTask, "console_display_task", 2048, &timeData, 5, nullptr);
  // xTaskCreate(&LedDisplayTask, "LED_display_task", 2048, &timeData, 5, nullptr);
  // xTaskCreate(&OLedDisplayTask, "OLED_display_task", 3048, &timeData, 5, nullptr);
}

//=============================================================================================================
void loop() {}
