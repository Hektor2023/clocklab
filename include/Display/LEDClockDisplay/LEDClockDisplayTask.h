#pragma once

#include "Display\LEDClockDisplay\LEDClockDisplayHandler.h"
#include "Converter\DisplayController.h"

//===================================================================================
void LedDisplayTask(void *pvParameter);

typedef struct {
  LEDClockDisplayHandler *ptr2ledDisplayHandler;
  DisplayController *ptr2dspController;
} ledTaskParams_t;

//===================================================================================
