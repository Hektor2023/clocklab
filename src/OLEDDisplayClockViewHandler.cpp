#include "OLEDDisplayClockViewHandler.h"


//===================================================================================
OLEDDisplayClockViewHandler::OLEDDisplayClockViewHandler( TimeHandler* ptr, const uint8_t sda_pin2, const uint8_t scl_pin2) 
  :TimeHandler( ptr), u8g2( U8G2_R0, scl_pin2, sda_pin2, U8X8_PIN_NONE)
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
  u8g2.setI2CAddress(0x3F * 2);
  u8g2.begin();
}

//===================================================================================
void OLEDDisplayClockViewHandler::doAction( void (*fun)( void))
{
  (*fun)();  
}

//===================================================================================
void OLEDDisplayClockViewHandler::updateTime( Timestamp &timestamp)
{ 
  const String dayOfWeekAsString[]={ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  char dateStrBuffer[MyDate::getStringBufferSize()];
  std::string date= timestamp.getDate().toString( dateStrBuffer);
  
  char timeStrBuffer[MyDate::getStringBufferSize()];
  std::string time= timestamp.getTime().toString( timeStrBuffer);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font 
 // u8g2.drawStr(0, 10, (char*)"Current time:"); // write something to the internal memory
 // u8g2.drawStr(0, 20,(char*)dayOfWeekAsString[ ( timestamp.getDate()).getDayOfWeek()].c_str());  //Maximun 18 characters.
 // u8g2.sendBuffer(); // transfer internal memory to the display
  
 // u8g2.clearBuffer();
 // u8g2.drawStr(0, 30,(char*)date.c_str());     //Maximun 18 characters.
 // u8g2.drawStr(0, 40,(char*)time.c_str());    //Maximun 18 characters.

  u8g2.drawStr(0, 10,(char*)"1BCDEFGHIJKLMNOP\n ");
  u8g2.drawStr(0, 20,(char*)"2BCDEFGHIJKLMNOP");
  u8g2.drawStr(0, 30,(char*)"3BCDEFGHIJKLMNOP\n");
  u8g2.drawStr(0, 40,(char*)"4BCDEFGHIJKLMNOP\n");
  u8g2.drawStr(0, 50,(char*)"5BCDEFGHIJKLMNOP\n");
  u8g2.drawStr(0, 60,(char*)"6BCDEFGHIJKLMNOP\n");
  u8g2.sendBuffer(); // transfer internal memory to the display
  
  baseUpdateTime( timestamp);
}

//===================================================================================
