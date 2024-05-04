#pragma once

#include "Arduino.h"
#include <cstddef>
#include <string>


#include "Converter/DisplayController.h"
#include "TimeType/TimeData.h"

#include "freertos/FreeRTOS.h"
#include <TM1638plus.h>


//===================================================================================
class LEDClockDisplayHandler {
private:
  TM1638plus tm;
  SemaphoreHandle_t xSemaphoreTM1638plus;

public:
  LEDClockDisplayHandler(const int STB_pin, const int CLK_pin,
                         const int DIO_pin);
  ~LEDClockDisplayHandler(void) = default;

  void updateCommand(DisplayCommand &cmd);
  uint8_t buttonsRead(void);
};

//===================================================================================
