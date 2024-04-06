#pragma once

#include <cstddef>

#include "Arduino.h"

#include "TimeType/TimeData.h"

//===================================================================================
class ConsoleDisplayHandler 
{
   public:
      ConsoleDisplayHandler( void);
      ~ConsoleDisplayHandler( void);
      
      void update( const TimeData &data);
      const char* getClassName( void);    
;};

//===================================================================================

