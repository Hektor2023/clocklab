#pragma once

#include <cstddef>

#include "Arduino.h"
#include "TimeType/Timestamp.h"
#include "Display/TimeDisplayHandler.h"


//===================================================================================
class ConsoleViewHandler:public TimeDisplayHandler
{
   private:
      Timestamp  lastTimestamp;

   public:
      ConsoleViewHandler( TimeDisplayHandler* ptr= nullptr);
      virtual ~ConsoleViewHandler( void);
      
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);    
;};

//===================================================================================

