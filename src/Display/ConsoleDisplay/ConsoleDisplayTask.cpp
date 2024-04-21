#include "Display/ConsoleDisplay/ConsoleDisplayTask.h"
#include "Display/ConsoleDisplay/ConsoleDisplayHandler.h"

#include "TimeType/TimeData.h"
#include "TimeType/MyTime.h"

//===================================================================================
void consoleDisplayTask(void *pvParameter)
{
  static ConsoleDisplayHandler consoleDisplayHandler;
  TimeData *ptr2timeData = reinterpret_cast<TimeData *>(pvParameter);
  MyTime lastTime;

  Serial.print("\nCONSOLE_DISPLAY_task:  start\n");

  for (;;)
  {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (ptr2timeData->lockData())
    {
      if (ptr2timeData->localTime != lastTime)
      {
        consoleDisplayHandler.update(*ptr2timeData);

        lastTime = ptr2timeData->localTime;
      }

      ptr2timeData->releaseData();
    }
  }

  vTaskDelete(nullptr);
}

//===================================================================================
