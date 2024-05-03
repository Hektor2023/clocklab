#include "Display/OLEDClockDisplay/OLEDClockDisplayHandler.h"

//===================================================================================
OLEDClockDisplayHandler::OLEDClockDisplayHandler(const uint8_t clock,
                                                 const uint8_t data,
                                                 const uint8_t cs,
                                                 const uint8_t dc,
                                                 const uint8_t reset)
    : u8g2(U8G2_R0, clock, data, cs, dc, reset) {}

//===================================================================================
void OLEDClockDisplayHandler::init(void) { u8g2.begin(); }

//===================================================================================
/*
 void OLEDClockDisplayHandler::update(const TimeData &data)
 {
   const String dayOfWeekAsString[] = {"Monday", "Tuesday", "Wednesday",
 "Thursday", "Friday", "Saturday", "Sunday"};

   char dateStrBuffer[MyDate::getStringBufferSize()];
   MyDate date = data.localDate;
   std::string dateAsString = date.toString(dateStrBuffer);

   MyTime time = data.localTime;
   char timeStrBuffer[MyTime::getStringBufferSize()];
   std::string timeAsString = time.toString(timeStrBuffer);

   char timeShortStrBuffer[MyTime::getStringShortBufferSize()];
   std::string timeAsShortString = time.toShortString(timeShortStrBuffer);

   u8g2.clearBuffer(); // clear the internal memory
   u8g2.enableUTF8Print();
   u8g2.setFont(u8g2_font_12x6LED_tf);
   u8g2.setDrawColor(1);

   u8g2.setCursor(0, 26);
   u8g2.setContrast(80);

   u8g2.print(timeAsString.c_str());

   u8g2.setCursor(0, 46);
   u8g2.setFont(u8g2_font_helvB12_tf);
   u8g2.print(dayOfWeekAsString[date.getDayOfWeek()].c_str());
   u8g2.sendBuffer();
 }
*/
//===================================================================================
