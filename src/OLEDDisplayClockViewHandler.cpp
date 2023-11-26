#include "OLEDDisplayClockViewHandler.h"


//===================================================================================
OLEDDisplayClockViewHandler::OLEDDisplayClockViewHandler(TimeDisplayHandler* ptr) 
  :TimeDisplayHandler( ptr), tft( TFT_eSPI())
{
   
}

//===================================================================================
OLEDDisplayClockViewHandler::~OLEDDisplayClockViewHandler( void) {};

//===================================================================================
const char* OLEDDisplayClockViewHandler::getClassName( void)
{
  return("OLEDDisplayClockViewHandler");
}

//===================================================================================
void OLEDDisplayClockViewHandler::init( void)
{
  // Initialize TFT LCD

  tft.begin();
  tft.setRotation(90);
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
}

//===================================================================================
void OLEDDisplayClockViewHandler::doAction( void (*fun)( void))
{
  (*fun)();  
}

//===================================================================================
void OLEDDisplayClockViewHandler::updateTime( Timestamp &timestamp)
{ 
  const String dayOfWeekAsString[]={ "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};

  char dateStrBuffer[MyDate::getStringBufferSize()];
  MyDate date= timestamp.getDate();
  std::string dateAsString= date.toString( dateStrBuffer);
  
  char timeStrBuffer[MyDate::getStringBufferSize()];
  std::string timeAsString= timestamp.getTime().toString( timeStrBuffer);

    // Set the font size and color
  tft.setTextSize(4); // choose a suitable font
  tft.setTextColor(TFT_GREEN);
  
  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  tft.setCursor(0, 0);
  tft.println( timeAsString.c_str());
  tft.println( dateAsString.c_str());
  tft.println( dayOfWeekAsString[ date.getDayOfWeek()].c_str());
 
  std::string sunriseAsString= sunrise.toString( timeStrBuffer);
  tft.println(  sunriseAsString.c_str());

  std::string sunsetAsString= sunset.toString( timeStrBuffer);
  tft.println(  sunsetAsString.c_str());

  baseUpdateTime( timestamp);
}

//===================================================================================
