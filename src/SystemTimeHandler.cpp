#include "SystemTimeHandler.h"

//===================================================================================
SystemTimeHandler::SystemTimeHandler( TimeHandler* ptr2timeHandler)
  :TimeHandler( ptr2timeHandler), timer( 940) {}   // 940

//===================================================================================
SystemTimeHandler::~SystemTimeHandler( void) {}

//===================================================================================
void SystemTimeHandler::init( void)
{
  
}

//===================================================================================
Timestamp& SystemTimeHandler::getTimestamp( void)
{
  return( systemTimestamp);
}

//===================================================================================
void SystemTimeHandler::setTimestamp( const Timestamp& newTimestamp)
{
  systemTimestamp.setEpochTime( newTimestamp.getEpochTime());
}

 //===================================================================================
const char* SystemTimeHandler::getClassName( void)
{
  return( "SystemTimeHandler");
}

//===================================================================================
void SystemTimeHandler::forceUpdateTime( void)
{
             
  timer.forceTimeout();
  baseUpdateTime( systemTimestamp);    
            
}
//===================================================================================
void SystemTimeHandler::updateTime( void)
{
             
  if( timer.isTimeout())
  {
    systemTimestamp++;
      
    baseUpdateTime( systemTimestamp);    
  }
  
           
}

//===================================================================================