
#include "GPSTimeHandler.h"


//===================================================================================
GPSTimeHandler::GPSTimeHandler( TimeHandler* ptr2timeHandler):TimeHandler( ptr2timeHandler), GPSTimestamp( 0L), centiSecond( 0) {}

//===================================================================================
bool GPSTimeHandler::encode( char c) 
{
  return( gps.encode(c));
}

//===================================================================================
Timestamp& GPSTimeHandler::getTimestamp( void)
{
  return( GPSTimestamp);
}

 //===================================================================================
const char* GPSTimeHandler::getClassName( void)
{
  return( "GPSTimeHandler");
}

 //===================================================================================
void GPSTimeHandler::updateTime( Timestamp &timestamp)
{

}

//===================================================================================
uint8_t GPSTimeHandler::getCentiSecond( void)
{
  return( centiSecond);
} 

//===================================================================================
void GPSTimeHandler::updateTime( void)
{
  static u_int8_t lastSecond= 0;
  static u_int8_t lastCentiSecond =0;

  if (gps.date.isValid() && gps.time.isValid() && ((gps.time.second()!= lastSecond) ||( gps.time.centisecond()!= lastCentiSecond)))
  {
    lastSecond= gps.time.second();
    lastCentiSecond= gps.time.centisecond();

    MyTime time;
    time.setHour( gps.time.hour());
    time.setMinute( gps.time.minute());
    time.setSecond( lastSecond);
    centiSecond= lastCentiSecond;
  
    MyDate  date;
    date.setYear( gps.date.year());
    date.setMonth( gps.date.month());
    date.setDay( gps.date.day());

    GPSTimestamp.setDate( date);
    GPSTimestamp.setTime( time); 
    baseUpdateTime( GPSTimestamp);    
  }

}

//===================================================================================
