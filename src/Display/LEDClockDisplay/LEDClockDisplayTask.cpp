#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"

#include "PinConfig.h"
#include "TimeType/MyTime.h"
#include "TimeType/TimeData.h"

//===================================================================================
void LedDisplayTask(void *pvParameter) {
  static LEDClockDisplayHandler LedDisplayHandler(gc_STB_pin, gc_CLK_pin,
                                                  gc_DIO_pin);
  TimeData *ptr2timeData = reinterpret_cast<TimeData *>(pvParameter);
  MyTime lastTime;

  Serial.print("\nLED_DISPLAY_task:  start\n");

  for (;;) {
    vTaskDelay(30 / portTICK_RATE_MS);

    /*
        if (ptr2timeData->lockData())
        {
          if (ptr2timeData->localTime != lastTime)
          {
            LedDisplayHandler.update(*ptr2timeData);

            lastTime = ptr2timeData->localTime;
          }

          ptr2timeData->releaseData();
        }
    */
  }

  vTaskDelete(nullptr);
}

//===================================================================================
