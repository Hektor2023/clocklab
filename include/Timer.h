#pragma once

#include <cstddef>
#include "Arduino.h"
#include "Timestamp.h"

constexpr uint32_t Second= 1000L;
  

typedef void ( *action)(void);
 
//===================================================================================
class Timer
{
   private:
      uint32_t interval;
      bool     forceTimeoutFlagg;

   public:
      Timer( uint32_t interval);
      Timer( Timer& t);
      ~Timer( void);
      
      Timer& operator=(Timer& t);

      void      setInterval( uint32_t interval);
      uint32_t  getInterval( void);

      void run( void ( *ptr_action)( void)); 
      
      void forceTimeout( void);
     
      void setForceTimeoutFlagg( bool flagg);
      bool getForceTimeoutFlagg( void);

      bool isTimeout( void);
      void delay( void);
};

//===================================================================================

