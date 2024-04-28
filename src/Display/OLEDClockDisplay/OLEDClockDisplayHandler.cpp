#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"

//===================================================================================
OLEDClockDisplayHandler::OLEDClockDisplayHandler(void)
    :u8g2(U8G2_R0, 18, 23, 5, 17, 19)
{
}

//===================================================================================
void OLEDClockDisplayHandler::init(void)
{
  u8g2.begin();
}

//===================================================================================
void OLEDClockDisplayHandler::update(const TimeData &data)
{
  const String dayOfWeekAsString[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

  char dateStrBuffer[MyDate::getStringBufferSize()];
  MyDate date = data.localDate;
  std::string dateAsString = date.toString(dateStrBuffer);

  MyTime time = data.localTime;
  char timeStrBuffer[MyTime::getStringBufferSize()];
  std::string timeAsString = time.toString(timeStrBuffer);

  char timeShortStrBuffer[MyTime::getStringShortBufferSize()];
  std::string timeAsShortString = time.toShortString(timeShortStrBuffer);
/*
  // Set the font size and color
  tft.setTextSize(5); // choose a suitable font
  tft.setTextColor(TFT_GREEN);

  // Clear the screen
  tft.fillScreen(TFT_BLACK);

  tft.setCursor(0, 0);
  tft.println(timeAsString.c_str());
  tft.println("");

  tft.setTextSize(4); // choose a suitable font
  tft.println(dateAsString.c_str());
  tft.println(dayOfWeekAsString[date.getDayOfWeek()].c_str());

  //  std::string sunriseAsString= sunrise.toShortString( timeShortStrBuffer);
  //  tft.println(  sunriseAsString.c_str());

  //  std::string sunsetAsString= sunset.toShortString( timeShortStrBuffer);
  //  tft.println(  sunsetAsString.c_str());
*/

  u8g2.clearBuffer(); // clear the internal memory
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_10x20_tf);
  u8g2.setDrawColor(1);

  u8g2.setCursor(10, 20);
  u8g2.setContrast(80);

  u8g2.print( timeAsString.c_str());
  u8g2.sendBuffer();
}

//===================================================================================
