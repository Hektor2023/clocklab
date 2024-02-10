#include "Display/ConsoleDisplayHandler.h"

//===================================================================================
ConsoleDisplayHandler::ConsoleDisplayHandler( void) {};

//===================================================================================
ConsoleDisplayHandler::~ConsoleDisplayHandler( void) {};

//===================================================================================
void ConsoleDisplayHandler::update( TimestampObserver* observer)
{
  Timestamp timestamp= observer->getTimestamp();

  MyTime currentTime= timestamp.getTime();
  char timeStrBuffer[ MyTime::getStringBufferSize()];

  Serial.printf("[%s]\n", currentTime.toString( timeStrBuffer));  
}

//===================================================================================
const char* ConsoleDisplayHandler::getClassName( void)
{
  return( "ConsoleDisplayHandler");
}

//===================================================================================
