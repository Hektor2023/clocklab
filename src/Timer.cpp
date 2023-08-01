#include "Timer.h"

//===================================================================================
Timer::Timer( uint32_t interval):interval( interval), forceTimeoutFlagg( false) {}
       
//===================================================================================
Timer::Timer( Timer& t) 
{
  interval= t.interval;
}

//===================================================================================
Timer::~Timer( void) {}

//===================================================================================
void  Timer::setInterval( uint32_t interval)
{
  this->interval= interval;  
}

//===================================================================================
uint32_t  Timer::getInterval( void)
{
  return( interval);        
}

//===================================================================================
void  Timer::forceTimeout( void)
{
  forceTimeoutFlagg= true;
}

//===================================================================================
void  Timer::setForceTimeoutFlagg( bool flagg)
{
  forceTimeoutFlagg= flagg;
}

//===================================================================================
bool  Timer::getForceTimeoutFlagg( void)
{
  return( forceTimeoutFlagg);
}

//===================================================================================
bool Timer::isTimeout( void)
{
  static uint32_t previous_Millis= millis();
  uint32_t  currentMillis;
 
  
  currentMillis = millis();
  if(( getForceTimeoutFlagg() || (currentMillis - previous_Millis) >= interval)) 
  {
    previous_Millis = currentMillis;
    
    // save the last time 
//    Serial.printf("Timestamp: %s",   Timestamp( currentMillis - previous_Millis).toString());
    setForceTimeoutFlagg( false);
    return( true);  
  }
  
  yield();
  return( false); 
}    

//===================================================================================
void Timer::run( void ( *ptr_action)( void))
{
  while( !isTimeout())
  {
   yield();    
  };

  if( ptr_action!= nullptr) 
  {
    (*ptr_action)(); 
    yield();
  }
  
}

//===================================================================================
Timer& Timer::operator=(Timer& t)
{
  interval= t.interval;
  return( *this);   
}

//===================================================================================
void Timer::delay( void)
{
  run( nullptr);  
  
}

//===================================================================================
