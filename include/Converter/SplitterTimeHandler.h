#pragma once

#include "TimeType/TimeHandler.h"

//===================================================================================
class SplitterTimeHandler:public TimeHandler
{
   protected:
      Timestamp &duplicatedTimestamp;    
   public:
      SplitterTimeHandler(  TimeHandler* ptr, Timestamp &duplicatedTimestamp); 
      virtual ~SplitterTimeHandler( void);

      virtual void updateTime( Timestamp &timestamp); 

      virtual const char* getClassName( void);
};

//===================================================================================

