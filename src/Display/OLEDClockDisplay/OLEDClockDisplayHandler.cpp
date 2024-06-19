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
void OLEDClockDisplayHandler::updateCommand(DisplayCommand &cmd)
{
  CommandString msg = cmd.getMessage();

  Serial.printf("\n!!!!  Display CmdOLED: %s\n", msg.c_str());

  switch (cmd.getCmdMode()) {
  case DisplayMode::eLocalTime:
  case DisplayMode::eUTCTime:
  case DisplayMode::eLocalDate: 
  {
    const String dayOfWeekAsString[] = {"Monday",   "Tuesday", "Wednesday",
                                        "Thursday", "Friday",  "Saturday",
                                        "Sunday"};

    size_t endOfFirstField = msg.find('|');
    size_t endOfSecondField = msg.find('|', endOfFirstField + 1);

    std::string dateAsString = msg.substr(0, endOfFirstField);

    std::string timeAsString =
        msg.substr(endOfFirstField + 1, endOfSecondField - endOfFirstField - 1);
    
    std::string dayOfWeekNumber =
        msg.substr(endOfSecondField + 1, msg.length() - endOfSecondField - 1);

    Serial.printf("\nDisplay CmdOLED-TIME: (%s)%s|%d\n", timeAsString.c_str(),
                  dayOfWeekNumber.c_str(),
                  atoi(const_cast<const char *>(dayOfWeekNumber.c_str())));

    u8g2.clearBuffer(); // clear the internal memory
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_12x6LED_tf);
    u8g2.setDrawColor(1);

    u8g2.setCursor(0, 20);
    u8g2.setContrast(80);

    u8g2.print(timeAsString.c_str());
    u8g2.setCursor(0, 40);

    u8g2.print(dateAsString.c_str());
    u8g2.setCursor(0, 60);
    u8g2.setFont(u8g2_font_helvB12_tf);

    uint8_t dayOfWeek =
        atoi(const_cast<const char *>(dayOfWeekNumber.c_str()));
    u8g2.print(dayOfWeekAsString[dayOfWeek].c_str());
    u8g2.sendBuffer();

    break;
  }

case DisplayMode::eSunriseTime: 
{
      //    Serial.printf("|%s|\n", cmd.getMessage());
  break;
}

case DisplayMode::eSunsetTime: 
{
      //    Serial.printf("|%s|\n", cmd.getMessage());
  break;
}

      default:;
    }
    
}

//===================================================================================
