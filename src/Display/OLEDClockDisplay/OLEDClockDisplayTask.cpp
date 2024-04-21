#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"

#include "TimeType/TimeData.h"
#include "TimeType/MyTime.h"

//===================================================================================
void OLedDisplayTask(void *pvParameter)
{
  OLEDClockDisplayHandler OLEDClockDisplayHandler;
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
