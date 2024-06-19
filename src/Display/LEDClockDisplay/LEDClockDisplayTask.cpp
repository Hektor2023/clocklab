#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"


#include "TimeType/MyTime.h"
#include "TimeType/TimeData.h"

//===================================================================================
void LedDisplayTask(void *pvParameter) {
  ledTaskParams_t *ptr2ledParams = reinterpret_cast<ledTaskParams_t *>(pvParameter);
  CommandString msg, old_msg;

  Serial.print("\nLED_DISPLAY_task:  start\n");

  for (;;) {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (ptr2ledParams->ptr2dspController->lockData()) 
    {
      DisplayCommand cmd = ptr2ledParams->ptr2dspController->getCommand();
      msg = cmd.getMessage();

      if (msg != old_msg) {
//        Serial.printf("\nDisplay CmdLED: %s\n", msg.c_str());
          ptr2ledParams->ptr2ledDisplayHandler->updateCommand(cmd);
          old_msg = msg;
      }
      ptr2ledParams->ptr2dspController->unlockData();
    }
  }

  vTaskDelete(nullptr);
}

//===================================================================================
