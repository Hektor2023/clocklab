#ifndef oleddisplayclockviewhandler_h
#define oleddisplayclockviewhandler_h

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "Timestamp.h"
#include "TimeDisplayHandler.h"

#include <TFT_eSPI.h>
#include <string.h>

// uses SPI pins
//===================================================================================
class OLEDDisplayClockViewHandler:public TimeDisplayHandler
{
   private:
      TFT_eSPI tft;
      
   public:
      OLEDDisplayClockViewHandler( TimeDisplayHandler* ptr);
      virtual ~OLEDDisplayClockViewHandler( void);

      void init( void);
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);
      
   private:
      void doAction( void (*fun)( void)); 
};

//===================================================================================
#endif
