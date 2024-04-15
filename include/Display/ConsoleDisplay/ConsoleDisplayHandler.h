#pragma once

#include <cstddef>

#include "Arduino.h"
#include "TimeType/TimeData.h"

//===================================================================================
class ConsoleDisplayHandler 
{
   public:
      ConsoleDisplayHandler( void) = default;
      ~ConsoleDisplayHandler( void) = default;
      
      void update( const TimeData &data);
;};

//===================================================================================
