#pragma once

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "TimeType/Timestamp.h"

#include "TimeType/TimeData.h"

#include <TM1638plus.h>
#include "freertos/FreeRTOS.h"

//===================================================================================
class LEDClockDisplayHandler
{
   private:
      TM1638plus  tm;
  
      SemaphoreHandle_t  xSemaphoreTM1638plus;

   public:
      LEDClockDisplayHandler( const int STB_pin, const int CLK_pin, const int DIO_pin);
      ~LEDClockDisplayHandler( void);

      uint8_t  buttonsRead( void);
     
      void update( const TimeData &data);
      const char* getClassName( void);
};

//===================================================================================
