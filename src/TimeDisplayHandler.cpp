#include "TimeDisplayHandler.h"

//===================================================================================
TimeDisplayHandler::TimeDisplayHandler( TimeDisplayHandler* ptr):ptr( ptr), displayMode( eTime) {}

//===================================================================================
void TimeDisplayHandler::baseUpdateTime( Timestamp &timestamp)
{    
  if( ptr)
  {
    ptr->setDisplayMode( displayMode);
    ptr->updateTime( timestamp);
 //   Serial.println( ptr->getClassName());
  }
            
}
       
//===================================================================================
void TimeDisplayHandler::updateTime( Timestamp &timestamp)
{            
  baseUpdateTime( timestamp);
}

//===================================================================================
void TimeDisplayHandler::setDisplayMode( displayMode_t mode)
{
    displayMode= mode;
}

//===================================================================================
displayMode_t TimeDisplayHandler::getDisplayMode( void)
{
    return( displayMode);
} 

//===================================================================================
