#include "Display/LEDClockDisplay/LEDClockDisplayTask.h"
#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"

#include "PinConfig.h"
#include "TimeType/MyTime.h"
#include "TimeType/TimeData.h"

//===================================================================================
void LedDisplayTask(void *pvParameter) {
  static LEDClockDisplayHandler LedDisplayHandler(gc_STB_pin, gc_CLK_pin,
                                                  gc_DIO_pin);
  DisplayController *dspController =  reinterpret_cast<DisplayController *>(pvParameter);

  CommandString msg, old_msg;
//  MyTime lastTime;

  Serial.print("\nLED_DISPLAY_task:  start\n");

  for (;;) {
    vTaskDelay(30 / portTICK_RATE_MS);

    if (dspController->lockData()) 
    {
      DisplayCommand cmd = dspController->getCommand();
      msg = cmd.getMessage();

      if (msg != old_msg) {
//        Serial.printf("\nDisplay CmdLED: %s\n", msg.c_str());
        LedDisplayHandler.updateCommand( cmd);
        old_msg = msg;
      }
      dspController->unlockData();
    }

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
