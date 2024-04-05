#include "Display/OLEDClockDisplayHandler.h"
 
//===================================================================================
OLEDClockDisplayHandler::OLEDClockDisplayHandler( void)
  :tft( TFT_eSPI())
{
   
}

//===================================================================================
OLEDClockDisplayHandler::~OLEDClockDisplayHandler( void) {};

//===================================================================================
const char* OLEDClockDisplayHandler::getClassName( void)
{
  return("OLEDClockDisplayHandler");
}

//===================================================================================
void OLEDClockDisplayHandler::init( void)
{
  // Initialize TFT LCD

  tft.begin();
  tft.setRotation(90);
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
}

//===================================================================================
void OLEDClockDisplayHandler::update( TimestampObserver* observer)
{ 
  const String dayOfWeekAsString[]={ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};

  Timestamp timestamp = observer->getTimestamp();

  char dateStrBuffer[MyDate::getStringBufferSize()];
  MyDate date;
  timestamp.getDate( date);
  std::string dateAsString= date.toString( dateStrBuffer);
  
  MyTime time;
  timestamp.getTime( time);
  char timeStrBuffer[MyTime::getStringBufferSize()];
  std::string timeAsString= time.toString( timeStrBuffer);

  char timeShortStrBuffer[MyTime::getStringShortBufferSize()];
  std::string timeAsShortString= time.toShortString( timeShortStrBuffer);
  
    // Set the font size and color
  tft.setTextSize(5); // choose a suitable font
  tft.setTextColor(TFT_GREEN);
  
  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  tft.setCursor(0, 0);
  tft.println( timeAsString.c_str());
  tft.println( "");

  tft.setTextSize(4); // choose a suitable font
  tft.println( dateAsString.c_str());
  tft.println( dayOfWeekAsString[ date.getDayOfWeek()].c_str());
 
//  std::string sunriseAsString= sunrise.toShortString( timeShortStrBuffer);
//  tft.println(  sunriseAsString.c_str());

//  std::string sunsetAsString= sunset.toShortString( timeShortStrBuffer);
//  tft.println(  sunsetAsString.c_str());

}

//===================================================================================
