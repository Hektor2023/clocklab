#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"
#include "Display/TimeDisplayHandler.h"

#include <TM1638plus.h>
#include "freertos/FreeRTOS.h"

//===================================================================================
class LEDClockViewHandler:public TimeDisplayHandler
{
   private:
      MyTime  clockTimePresented;
      
      TM1638plus  tm;
      bool        adjustMode;

      MyTime& sunrise;
      MyTime& sunset;
      SemaphoreHandle_t  xSemaphoreTM1638plus;


   public:
      LEDClockViewHandler( TimeDisplayHandler* ptr, MyTime& sunrise, MyTime& sunset, const int STB_pin, const int CLK_pin, const int DIO_pin);
      virtual ~LEDClockViewHandler( void);

      uint8_t  buttonsRead( void);
      void     modeAdjust( bool flagg);
           
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);
      
   private:
      void doAction( void (*fun)( void)); 
};

//===================================================================================
