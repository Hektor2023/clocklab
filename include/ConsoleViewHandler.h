#ifndef consoleviewhandler_h
#define consoleviewhandler_h

#include <cstddef>

#include "Arduino.h"

#include "Timestamp.h"
#include "TimeHandler.h"

//===================================================================================
class ConsoleViewHandler:public TimeHandler
{
   private:
      Timestamp  lastTimestamp;
      
   public:
      ConsoleViewHandler( TimeHandler* ptr= nullptr);
      virtual ~ConsoleViewHandler( void);
      
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);      
;};

//===================================================================================
#endif
