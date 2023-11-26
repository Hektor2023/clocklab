#include "TimeDisplayHandler.h"

//===================================================================================
TimeDisplayHandler::TimeDisplayHandler( TimeDisplayHandler* ptr):ptr( ptr), displayMode( eTime),
      longitude( 0),latitude(0){}

//===================================================================================
void TimeDisplayHandler::baseUpdateTime( Timestamp &timestamp)
{    
  if( ptr)
  {
    ptr->setDisplayMode( displayMode);
    ptr->updateTime( timestamp);

 //   Serial.println( ptr->getClassName());
    ptr->updateLocation( longitude, latitude);
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
void TimeDisplayHandler::updateLocation( double longitude, double latitude)
{
  this->longitude= longitude;
  this->latitude= latitude;
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
