#pragma once

#include <cstddef>

#include "TimeType/TimeHandler.h"
#include "Source/SystemTimeHandler/Timer.h"

//===================================================================================
class SystemTimeHandler:public TimeHandler
{
   protected:
      Timestamp     systemTimestamp;  
      TimeHandler*  ptr2timeHandler;
      Timer         timer;
      
   private:
      void updateTime( Timestamp &timestamp);

   public:
      SystemTimeHandler( TimeHandler* ptr= nullptr);
      virtual ~SystemTimeHandler( void);

      virtual void init( void);
     
      virtual Timestamp&  getTimestamp( void);
      virtual void        setTimestamp( const Timestamp& newTimestamp);
      virtual const char* getClassName( void);

      void forceUpdateTime( void);
      void updateTime( void);
};

//===================================================================================

