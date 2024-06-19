#pragma once

#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"
#include "Converter/DisplayController.h"

//===================================================================================
void OLedDisplayTask(void *pvParameter);

typedef struct {
  OLEDClockDisplayHandler *ptr2OledDisplayHandler;
  DisplayController *ptr2dspController;
} OledTaskParams_t;
//===================================================================================
