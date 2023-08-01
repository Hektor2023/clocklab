#include "ConsoleViewHandler.h"

//===================================================================================
ConsoleViewHandler::ConsoleViewHandler( TimeHandler* ptr):TimeHandler( ptr) {};

//===================================================================================
ConsoleViewHandler::~ConsoleViewHandler( void) {};

//===================================================================================
void ConsoleViewHandler::updateTime( Timestamp &timestamp)
{
 
  char timestampStrBuffer[ Timestamp::getStringBufferSize()];
  Serial.printf("(%10s) %s\n",
                 timestamp.getDayOfWeekAsString(), 
                 timestamp.toString( timestampStrBuffer));
  TimeHandler::updateTime( timestamp);

  
}

//===================================================================================
const char* ConsoleViewHandler::getClassName( void)
{
  return( "ConsoleViewHandler");
}

//===================================================================================
