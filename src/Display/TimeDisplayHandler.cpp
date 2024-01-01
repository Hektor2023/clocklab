#include "Display/TimeDisplayHandler.h"

//===================================================================================
TimeDisplayHandler::TimeDisplayHandler( TimeDisplayHandler* ptr):ptr( ptr), displayMode( eTime)
  {}

//===================================================================================
void TimeDisplayHandler::baseUpdateTime( Timestamp &timestamp)
{    
  if( ptr)
  {
    ptr->setDisplayMode( displayMode);
    ptr->updateTime( timestamp);

 //   Serial.println( ptr->getClassName());
    ptr->updateLocation( coordinates);
    ptr->updateSunriseSunset( sunrise, sunset);
  }
            
}

//===================================================================================
void TimeDisplayHandler::init( void) 
{
  if( ptr)
  {
    ptr->init();
  }
  
}

//===================================================================================
void TimeDisplayHandler::updateLocation( Coordinates_t coordinates)
{
  this->coordinates= coordinates;

}

//===================================================================================
void TimeDisplayHandler::updateSunriseSunset( MyTime sunrise, MyTime sunset)
{
  this->sunrise= sunrise;
  this->sunset= sunset;
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
