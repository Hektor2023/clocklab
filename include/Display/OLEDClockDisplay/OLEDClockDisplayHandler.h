#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"
#include "TimeType/TimeData.h"

#include <u8g2lib.h>

// uses SPI pins
//===================================================================================
class OLEDClockDisplayHandler
{
private:
   U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2;

public:
   OLEDClockDisplayHandler(const uint8_t clock, const uint8_t data, const uint8_t cs, const uint8_t dc, const uint8_t reset);
   ~OLEDClockDisplayHandler(void) = default;

   void init(void);
   void update(const TimeData &data);
};

//===================================================================================
