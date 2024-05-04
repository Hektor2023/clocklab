#pragma once

#include <cstddef>

#include "Arduino.h"
#include "Converter/DisplayController.h"
#include "TimeType/TimeData.h"

//===================================================================================
class ConsoleDisplayHandler {
public:
  ConsoleDisplayHandler(void) = default;
  ~ConsoleDisplayHandler(void) = default;

  void updateCommand(DisplayCommand &cmd);
};

//===================================================================================
