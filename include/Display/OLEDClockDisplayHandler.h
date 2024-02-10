#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"

#include "Display/TimestampObserver.h"

#include <TFT_eSPI.h>
#include <string.h>

// uses SPI pins
//===================================================================================
class OLEDClockDisplayHandler:public TimestampListener
{
   private:
      TFT_eSPI tft;
   
   public:
      OLEDClockDisplayHandler( void);
      virtual ~OLEDClockDisplayHandler( void);

      void init( void);
      virtual void update( TimestampObserver* observer);
      virtual const char* getClassName( void);
};

//===================================================================================

