#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"
#include "TimeType/TimeData.h"

#include <TFT_eSPI.h>
#include <string.h>

// uses SPI pins
//===================================================================================
class OLEDClockDisplayHandler
{
private:
   TFT_eSPI tft;

public:
   OLEDClockDisplayHandler(void);
   ~OLEDClockDisplayHandler(void) = default;

   void init(void);
   void update(const TimeData &data);
};

//===================================================================================
