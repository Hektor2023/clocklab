#ifndef consoleviewhandler_h
#define consoleviewhandler_h

#include <cstddef>

#include "Arduino.h"
#include "Timestamp.h"
#include "TimeDisplayHandler.h"


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
#endif
