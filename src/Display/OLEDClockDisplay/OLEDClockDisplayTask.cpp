#include "Display/OLEDClockDisplay/OLEDClockDisplayTask.h"
#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"

#include "TimeType/MyTime.h"
#include "TimeType/TimeData.h"

// 18, 23, 5, 17, 19
// clock, uint8_t data, uint8_t cs, uint8_t dc, uint8_t reset

//===================================================================================
void OLedDisplayTask(void *pvParameter) {
  OledTaskParams_t *ptr2oledTaskParams = reinterpret_cast<OledTaskParams_t *>(pvParameter);
  CommandString msg, old_msg;

  Serial.print("\nOLED_DISPLAY_task:  start\n");

  ptr2oledTaskParams->ptr2OledDisplayHandler->init();

  for (;;) {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (ptr2oledTaskParams->ptr2dspController->lockData()) {
      DisplayCommand cmd = ptr2oledTaskParams->ptr2dspController->getCommand();
      msg = cmd.getMessage();

      if (msg != old_msg) {
//        Serial.printf("\nDisplay CmdOLED: %s\n", msg.c_str());
        ptr2oledTaskParams->ptr2OledDisplayHandler->updateCommand(cmd);
        old_msg = msg;
      }
      ptr2oledTaskParams->ptr2dspController->unlockData();
    }
  }

  vTaskDelete(nullptr);
}

//===================================================================================
