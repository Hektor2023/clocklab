#pragma once

#include <cstddef>

#include "Arduino.h"

#include "Display/TimestampObserver.h"

//===================================================================================
class ConsoleDisplayHandler:public TimestampListener  
{

   public:
      ConsoleDisplayHandler( void);
      virtual ~ConsoleDisplayHandler( void);
      
      virtual void update( TimestampObserver* observer);
      virtual const char* getClassName( void);    
;};

//===================================================================================

