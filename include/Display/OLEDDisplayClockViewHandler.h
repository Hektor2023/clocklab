#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"
#include "Display/TimeDisplayHandler.h"

#include <TFT_eSPI.h>
#include <string.h>

// uses SPI pins
//===================================================================================
class OLEDDisplayClockViewHandler:public TimeDisplayHandler
{
   private:
      TFT_eSPI tft;
      MyTime& sunrise;
      MyTime& sunset;

   public:
      OLEDDisplayClockViewHandler( TimeDisplayHandler* ptr, MyTime& sunrise, MyTime& sunset);
      virtual ~OLEDDisplayClockViewHandler( void);

      void init( void);
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);
      
   private:
      void doAction( void (*fun)( void)); 
};

//===================================================================================

