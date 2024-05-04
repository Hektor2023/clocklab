#include "Converter/DisplayController.h"
#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"

//===================================================================================
DisplayCommand::DisplayCommand(void)
    : mode{DisplayMode::eLocalTime}, msg("00:00:00") {}

//===================================================================================
const DisplayMode &DisplayCommand::getCmdMode(void) { return mode; }

//===================================================================================
void DisplayCommand::setCmdMode(const DisplayMode cmdMode) { mode = cmdMode; }

//===================================================================================
const CommandString &DisplayCommand::getMessage(void) { return msg; }

//===================================================================================
void DisplayCommand::setMessage(const CommandString cmdMsg) { msg = cmdMsg; }

//===================================================================================
DisplayController::DisplayController(void)
    : displayMode(DisplayMode::eLocalTime), cmd() {}

//===================================================================================
void DisplayController::setDisplayMode(const DisplayMode mode) {
  displayMode = mode;
}

//===================================================================================
const DisplayCommand &DisplayController::getCommand(void) { return cmd; }

//===================================================================================
void DisplayController::update(TimeData &data) {
  cmd.setCmdMode(displayMode);

  switch (displayMode) {
  case DisplayMode::eLocalTime: {
    Timestamp localTimestamp = data.localTimestamp;

    char timestampStrBuffer[Timestamp::getStringBufferSize()];
    cmd.setMessage(localTimestamp.toString(timestampStrBuffer));
    break;
  }

  case DisplayMode::eUTCTime: {
    Timestamp UTCTimestamp = data.UTCTimestamp;

    char timestampStrBuffer[Timestamp::getStringBufferSize()];
    cmd.setMessage(UTCTimestamp.toString(timestampStrBuffer));
    break;
  }

  case DisplayMode::eSunriseTime: {
    constexpr char invalidTime[] = "--:--:--";
    MyTime noonTime(00, 00, 00);

    MyTime sunriseTime = data.sunriseTime;
    char timeStrBuffer[MyTime::getStringBufferSize()];
    cmd.setMessage((noonTime == sunriseTime)
                       ? invalidTime
                       : sunriseTime.toString(timeStrBuffer));
    break;
  }

  case DisplayMode::eSunsetTime: {
    constexpr char invalidTime[] = "--:--:--";
    MyTime noonTime(00, 00, 00);

    MyTime sunsetTime = data.sunsetTime;
    char timeStrBuffer[MyTime::getStringBufferSize()];
    cmd.setMessage((noonTime == sunsetTime)
                       ? invalidTime
                       : sunsetTime.toString(timeStrBuffer));
    break;
  }

  default:;
  }
}

//===================================================================================
