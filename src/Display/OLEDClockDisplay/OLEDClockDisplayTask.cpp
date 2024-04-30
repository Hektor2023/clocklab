#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"

#include "TimeType/TimeData.h"
#include "TimeType/MyTime.h"
#include "PinConfig.h"

// 18, 23, 5, 17, 19
// clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset

//===================================================================================
void OLedDisplayTask(void *pvParameter)
{
  OLEDClockDisplayHandler OLEDClockDisplayHandler(gc_OLED_clock_pin, gc_OLED_data_pin, gc_OLED_cs_pin, gc_OLED_dc_pin, gc_OLED_reset_pin);
  TimeData *ptr2timeData = reinterpret_cast<TimeData *>(pvParameter);
  MyTime lastTime;

  Serial.print("\nOLED_DISPLAY_task:  start\n");

  OLEDClockDisplayHandler.init();

  for (;;)
  {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (ptr2timeData->lockData())
    {
      if (ptr2timeData->localTime != lastTime)
      {
        OLEDClockDisplayHandler.update(*ptr2timeData);

        lastTime = ptr2timeData->localTime;
      }

      ptr2timeData->releaseData();
    }
  }

  vTaskDelete(nullptr);
}

//===================================================================================
