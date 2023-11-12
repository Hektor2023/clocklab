#include "ConsoleViewHandler.h"

//===================================================================================
ConsoleViewHandler::ConsoleViewHandler( TimeDisplayHandler* ptr):TimeDisplayHandler( ptr) {};

//===================================================================================
ConsoleViewHandler::~ConsoleViewHandler( void) {};

//===================================================================================
void ConsoleViewHandler::updateTime( Timestamp &timestamp)
{
 
  switch( displayMode)
  {
    case eTime:
      {
        MyTime currentTime= timestamp.getTime();
        char timeStrBuffer[ MyTime::getStringBufferSize()];

        Serial.printf("%s\n", currentTime.toString( timeStrBuffer));  
      }  
      break;

    case eDate:
      {
        MyDate currentDate= timestamp.getDate();
        char dateStrBuffer[ MyDate::getStringBufferSize()];

         Serial.printf("(%10s) %s\n", timestamp.getDayOfWeekAsString(), currentDate.toString( dateStrBuffer));          
      }
      break;

    default:;
  }

  TimeDisplayHandler::updateTime( timestamp);

}

//===================================================================================
const char* ConsoleViewHandler::getClassName( void)
{
  return( "ConsoleViewHandler");
}

//===================================================================================
