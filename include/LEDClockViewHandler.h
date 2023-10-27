#ifndef ledclockviewhandler_h
#define ledclockviewhandler_h

#include <cstddef>
#include <string>
#include "Arduino.h"

#include "Timestamp.h"
#include "TimeHandler.h"

#include <TM1638plus.h>
#include "freertos/FreeRTOS.h"

//===================================================================================
class LEDClockViewHandler:public TimeHandler
{
   private:
      MyTime  clockTimePresented;
      
      TM1638plus tm;
       SemaphoreHandle_t  xSemaphoreTM1638plus;

   public:
      LEDClockViewHandler( TimeHandler* ptr, const int STB_pin, const int CLK_pin, const int DIO_pin);
      virtual ~LEDClockViewHandler( void);

      uint8_t buttonsRead( void);
      void modeAdjust( bool flagg);

      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);
      
   private:
      void doAction( void (*fun)( void)); 
};

//===================================================================================
#endif
