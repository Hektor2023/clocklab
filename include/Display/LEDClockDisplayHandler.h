#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"

#include "Display/TimestampObserver.h"

#include <TM1638plus.h>
#include "freertos/FreeRTOS.h"

//===================================================================================
class LEDClockDisplayHandler:public TimestampListener
{
   private:
      TM1638plus  tm;
  
      SemaphoreHandle_t  xSemaphoreTM1638plus;

   public:
      LEDClockDisplayHandler( const int STB_pin, const int CLK_pin, const int DIO_pin);
      virtual ~LEDClockDisplayHandler( void);

      uint8_t  buttonsRead( void);
     
      virtual void update( TimestampObserver* observer);
      virtual const char* getClassName( void);
};

//===================================================================================
