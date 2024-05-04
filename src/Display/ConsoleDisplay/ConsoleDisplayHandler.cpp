#include "Display/ConsoleDisplay/ConsoleDisplayHandler.h"

//===================================================================================
void ConsoleDisplayHandler::updateCommand(DisplayCommand &cmd) {
  switch (cmd.getCmdMode()) {
  case DisplayMode::eLocalTime: {
    Serial.printf("[%s]\n", cmd.getMessage());
    break;
  }
  case DisplayMode::eUTCTime: {
    Serial.printf("[%s]\n", cmd.getMessage());
    break;
  }
  case DisplayMode::eSunriseTime: {
    Serial.printf("|%s|\n", cmd.getMessage());
    break;
  }
  case DisplayMode::eSunsetTime: {
    Serial.printf("|%s|\n", cmd.getMessage());
    break;
  }
  default:;
  }
}

//===================================================================================