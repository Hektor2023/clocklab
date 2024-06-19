#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"

//===================================================================================
LEDClockDisplayHandler::LEDClockDisplayHandler(const int STB_pin,
                                               const int CLK_pin,
                                               const int DIO_pin)
    : tm(STB_pin, CLK_pin, DIO_pin, true) {
  pinMode(STB_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, OUTPUT);

  xSemaphoreTM1638plus = xSemaphoreCreateMutex();
  tm.displayBegin();

  tm.setLEDs(0);
};

//===================================================================================
uint16_t LEDClockDisplayHandler::buttonsRead(void) {
  static uint16_t lastKeys = -1;
  uint16_t keys;

  if (xSemaphoreTake(xSemaphoreTM1638plus, (TickType_t)0) == pdTRUE) {
    keys = tm.readButtons();
    xSemaphoreGive(xSemaphoreTM1638plus);

    if (keys != lastKeys) {
      lastKeys = keys;
      return (keys);
    }
  }

  return (0);
}

//===================================================================================
void LEDClockDisplayHandler::updateCommand(DisplayCommand &cmd) {
  CommandString msg = cmd.getMessage();
  Serial.printf("\nDisplay CmdLED-2: %s\n", msg.c_str());

  switch (cmd.getCmdMode()) 
  {
    case DisplayMode::eLocalTime:
    case DisplayMode::eUTCTime: 
    {
      size_t endOfFirstField = msg.find('|');
      size_t endOfSecondField = msg.find('|', endOfFirstField+1);

      std::string timeAsString =
          msg.substr(endOfFirstField + 1, endOfSecondField - endOfFirstField - 1);
      std::replace(timeAsString.begin(), timeAsString.end(), ':', '-');
      std::string dayOfWeekNumber =
          msg.substr(endOfSecondField + 1, msg.length() - endOfSecondField - 1);

      Serial.printf("\nDisplay CmdLED-TIME: (%s)%s|%d\n", timeAsString.c_str(),
                    dayOfWeekNumber.c_str(),
                    atoi(const_cast<const char *>(dayOfWeekNumber.c_str())));

      while (xSemaphoreTake(xSemaphoreTM1638plus, (TickType_t)0) != pdTRUE) {}; 
      
      tm.displayText(timeAsString.c_str());

      tm.setLEDs(0);
      tm.setLED( atoi( const_cast< const char*>( dayOfWeekNumber.c_str())), 1); 
  
      xSemaphoreGive(xSemaphoreTM1638plus);
      
      break;
    }

    case DisplayMode::eLocalDate:
    {
      size_t endOfFirstField = msg.find('|');
      size_t endOfSecondField = msg.find('|', endOfFirstField + 1);

      std::string dateAsString = msg.substr(  0, endOfFirstField);
      std::replace(dateAsString.begin(), dateAsString.end(), '/', '.');
      std::string dayOfWeekNumber =
          msg.substr(endOfSecondField + 1, msg.length() - endOfSecondField - 1);

      Serial.printf("\nDisplay CmdLED-DATE: (%s)%s|%d\n", dateAsString.c_str(),
                    dayOfWeekNumber.c_str(),
                    atoi(const_cast<const char *>(dayOfWeekNumber.c_str())));

      while (xSemaphoreTake(xSemaphoreTM1638plus, (TickType_t)0) != pdTRUE) {
      };

      tm.displayText(dateAsString.c_str());

      tm.setLEDs(0);
      tm.setLED(atoi(const_cast<const char *>(dayOfWeekNumber.c_str())), 1);

      xSemaphoreGive(xSemaphoreTM1638plus);
      break;
    }

    case DisplayMode::eSunriseTime: 
    {
      Serial.printf("SUNRISE |%s|\n", msg.c_str());
      break;
    }

    case DisplayMode::eSunsetTime: 
    {
      Serial.printf("SUNSET |%s|\n", msg.c_str());
      break;
    }

    default:;
  }

}

//===================================================================================
