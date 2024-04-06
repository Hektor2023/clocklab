#include "Display/ConsoleDisplay/ConsoleDisplayHandler.h"

//===================================================================================
ConsoleDisplayHandler::ConsoleDisplayHandler( void) {};

//===================================================================================
ConsoleDisplayHandler::~ConsoleDisplayHandler( void) {};

//===================================================================================
void ConsoleDisplayHandler::update( const TimeData &data)
{
  MyTime currentTime = data.localTime;
  char timeStrBuffer[ MyTime::getStringBufferSize()];
  MyDate currentDate = data.localDate;
  char dateStrBuffer[ MyDate::getStringBufferSize()];
  Serial.printf("[%s \t|\t %s]\n", currentTime.toString( timeStrBuffer), currentDate.toString( dateStrBuffer));  
}

//===================================================================================
const char* ConsoleDisplayHandler::getClassName( void)
{
  return( "ConsoleDisplayHandler");
}

//===================================================================================
