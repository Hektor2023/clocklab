#pragma once
#include <cstddef>

#include "Timestamp.h"

//===================================================================================
class TimeHandler
{
   protected:
      TimeHandler* ptr;
      bool         bypassModule;
      bool         validTime;

   protected:
      void baseUpdateTime( Timestamp &timestamp);

   public:
      TimeHandler( TimeHandler* ptr);
      virtual ~TimeHandler( void)= default;
      
      virtual void updateTime( Timestamp &timestamp);

      virtual const char* getClassName( void)=0;
             
   private:
      TimeHandler(const TimeHandler&) = delete;
      const TimeHandler& operator=(const TimeHandler&) = delete;  
};

//===================================================================================

