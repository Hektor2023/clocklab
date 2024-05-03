#include "Display/ConsoleDisplay/ConsoleDisplayTask.h"
#include "Display/ConsoleDisplay/ConsoleDisplayHandler.h"

#include "TimeType/MyTime.h"
#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"

//===================================================================================
void consoleDisplayTask(void *pvParameter) {
  static ConsoleDisplayHandler consoleDisplayHandler;
  TimeData *ptr2timeData = reinterpret_cast<TimeData *>(pvParameter);
  Timestamp lastTimestamp;

  Serial.print("\nCONSOLE_DISPLAY_task:  start\n");

  for (;;) 
  {
    vTaskDelay(30 / portTICK_RATE_MS);

//
//    if (ptr2timeData->localTimestamp != lastTimestamp) {
      //      consoleDisplayHandler.updateCommand(DisplayCommand & cmd);
      //      (*ptr2timeData);
//      lastTimestamp = ptr2timeData->localTimestamp;
//    }

//    ptr2timeData->releaseData();
  }

  vTaskDelete(nullptr);
}

//===================================================================================
