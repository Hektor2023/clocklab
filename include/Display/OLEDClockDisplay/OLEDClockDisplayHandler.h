#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"
#include "TimeType/TimeData.h"

#include <u8g2lib.h>
#include <MUIU8g2.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <string.h>

// uses SPI pins
//===================================================================================
class OLEDClockDisplayHandler
{
private:
   U8G2_ST7565_ERC12864_ALT_F_4W_SW_SPI u8g2;

public : OLEDClockDisplayHandler(void);
   ~OLEDClockDisplayHandler(void) = default;

   void init(void);
   void update(const TimeData &data);
};

//===================================================================================
