#ifndef oleddisplayclockviewhandler_h
#define oleddisplayclockviewhandler_h

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "Timestamp.h"
#include "TimeHandler.h"

#include <U8g2lib.h>
#include <string.h>

// uses I2C pins
//===================================================================================
class OLEDDisplayClockViewHandler:public TimeHandler
{
   private:
   //   U8G2_ST7567_HEM6432_F_HW_I2C u8g2;
   //   U8G2_ST7567_ENH_DG128064_F_SW_I2C u8g2;
      U8G2_ST7567_OS12864_F_HW_I2C u8g2;
      
   public:
      OLEDDisplayClockViewHandler( TimeHandler* ptr, const uint8_t sda_pin, const uint8_t scl_pin);
      virtual ~OLEDDisplayClockViewHandler( void);

      void init( void);
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);
      
   private:
      void doAction( void (*fun)( void)); 
};

//===================================================================================
#endif
