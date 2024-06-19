#include <Arduino.h>
#include <array>

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

#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"

#include "Other/AdjustmentAdvisor.h"
#include "Other/ButtonController.h"
#include "Other/Tools.h"

constexpr uint8_t gc_irqIn_pin{15}; // D8 - don't use D4

// constexpr uint8_t   gc_rxd2_pin{17};
constexpr uint8_t gc_txd2_pin{16};

constexpr uint8_t gc_sda_pin{21};
constexpr uint8_t gc_scl_pin{22};

constexpr uint8_t gc_DIO_pin{13}; // 25
constexpr uint8_t gc_CLK_pin{33}; // 26
constexpr uint8_t gc_STB_pin{32}; // 27

constexpr uint8_t gc_SI_pin{25};
constexpr uint8_t gc_RCK_pin{26};
constexpr uint8_t gc_SCK_pin{27};
constexpr uint8_t gc_NSCLR_pin{14};

constexpr uint8_t gc_PULS_pin{4};

constexpr uint8_t gc_OLED_clock_pin{18};
constexpr uint8_t gc_OLED_data_pin{23};
constexpr uint8_t gc_OLED_cs_pin{5};
constexpr uint8_t gc_OLED_dc_pin{17};
constexpr uint8_t gc_OLED_reset_pin{19};

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
static LEDClockDisplayHandler LedDisplayHandler(gc_STB_pin, gc_CLK_pin, gc_DIO_pin);
static OLEDClockDisplayHandler OLEDClockDisplayHandler(gc_OLED_clock_pin,
                                                gc_OLED_data_pin,
                                                gc_OLED_cs_pin, gc_OLED_dc_pin,
                                                gc_OLED_reset_pin);
static DisplayController displController;

static xQueueHandle g_queueSourceTime = xQueueCreate(3, sizeof(MessageTime_t));
static SemaphoreHandle_t g_xSemaphoreRtc;
static AdjustmentAdvisor g_advisor;

//=============================================================================================================
void buttonControlerTask(void *pvParameter) {
  ledTaskParams_t *ptr2ledParams =  reinterpret_cast<ledTaskParams_t *>(pvParameter);

  ButtonController controller;
  Buttons_t button;

  Serial.print("\nBUTTON_task:  start\n");

  for (;;) 
  {
    vTaskDelay(30 / portTICK_RATE_MS);

    uint16_t btn = ptr2ledParams->ptr2ledDisplayHandler->buttonsRead();
    if (( btn!= 0) && ptr2ledParams->ptr2dspController->lockData()) {
        button = static_cast< Buttons_t>( btn);

        Serial.printf("\n###### Button: %d # %d\n", btn, button);
        switch (button) {
        case Buttons_t::eButton1:
          ptr2ledParams->ptr2dspController->setDisplayMode(
              DisplayMode::eLocalTime);
          break;

        case Buttons_t::eButton2:
          ptr2ledParams->ptr2dspController
            ->setDisplayMode(DisplayMode::eUTCTime);
            break;

        case Buttons_t::eButton3:
          ptr2ledParams->ptr2dspController->setDisplayMode(
              DisplayMode::eLocalDate);
          break;

        case Buttons_t::eButton4:
          ptr2ledParams->ptr2dspController->setDisplayMode(
              DisplayMode::eSunriseTime);
          break;

        case Buttons_t::eButton5:
          ptr2ledParams->ptr2dspController->setDisplayMode(
              DisplayMode::eSunsetTime);
          break;

        default:;
        }

        ptr2ledParams->ptr2dspController->unlockData();
      }
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void consoleOutTask(void *pvParameter) {
  DisplayController *ptr2dspController =  reinterpret_cast<DisplayController *>(pvParameter);
  printTick();  Serial.print("\nCONSOLE_OUT_task:  start\n");
  CommandString msg, old_msg;

  for (;;) 
  {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (ptr2dspController->lockData()) 
    {
      DisplayCommand cmd = ptr2dspController->getCommand();
      msg = cmd.getMessage();

      if( msg != old_msg)
      {
        Serial.printf("\nDisplay DISPLAY: %s\n", msg.c_str());
        old_msg = msg;
      }
      ptr2dspController->unlockData();
    }
//   
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
void rtcReadTask(void *pvParameter) { // set new time for Displays via DisplayController
  static ConverterTimestampAdapter timestampAdapter;
  static DSTSunriseSunsetTimeHandler dstHandlder(gc_GMT_Plus_2h);

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

//        DisplayCommand cmd = displController.getCommand();
//        auto msg = cmd.getMessage();
//        Serial.printf("\nDisplay Cmd: %s\n", msg.c_str());

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
  static ntpTaskParams_t ntpTaskParams{SSID, PASSWD, &g_queueSourceTime};
  static ledTaskParams_t ledTaskParams{ &LedDisplayHandler, &displController};
  static OledTaskParams_t oledTaskParams{&OLEDClockDisplayHandler, &displController};

  g_xSemaphoreRtc = xSemaphoreCreateMutex();

  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.flush();

  vTaskDelay(3000 / portTICK_RATE_MS);
  pinMode(gc_PULS_pin, OUTPUT);

  Serial.print("setup: start ======================\n");

  xTaskCreate(&gpsTask, "gps_task", 5048, &g_queueSourceTime, 5, nullptr);
  xTaskCreate(&ntpTask, "ntp_task", 4048, &ntpTaskParams, 5, nullptr);

  xTaskCreate(&rtcWriteTask, "rtc_write_task", 2048, &g_queueSourceTime, 5, nullptr);                                                  //
  xTaskCreate(&rtcReadTask, "rtc_read_task", 2048, nullptr, 5, nullptr); // 2048

  xTaskCreate(&consoleOutTask, "console_out_task", 2600, &displController, 5, nullptr); // 3048

  xTaskCreate(&LedDisplayTask, "LED_display_task", 2048, &ledTaskParams, 5, nullptr);
  xTaskCreate(&OLedDisplayTask, "OLED_display_task", 3048, &oledTaskParams, 5, nullptr);

  xTaskCreate(&buttonControlerTask, "buttons_task", 3048, &ledTaskParams, 5,  nullptr);
}

//=============================================================================================================
void loop() {}
