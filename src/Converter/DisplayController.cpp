#include "Converter/DisplayController.h"
#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"

DisplayCommand::DisplayCommand(void)
    : mode{DisplayMode::eLocalTime}, msg("00:00:00") {}

const DisplayMode &DisplayCommand::getCmdMode(void) { return mode; }

void DisplayCommand::setCmdMode(const DisplayMode cmdMode) { mode = cmdMode; }

const LimitedSizeString<cmdStringSize> &DisplayCommand::getMessage(void) {
  return msg;
}

void DisplayCommand::setMessage(const LimitedSizeString<cmdStringSize> cmdMsg) {
  msg = cmdMsg;
}

DisplayController::DisplayController(void)
    : displayMode(DisplayMode::eLocalTime), cmd() {}

void DisplayController::setDisplayMode(const DisplayMode mode) {
  displayMode = mode;
}

const DisplayCommand &DisplayController::getCommand(void) { return cmd; }

void DisplayController::update(TimeData &data) {
  cmd.setCmdMode(displayMode);

  switch (displayMode) {
  case DisplayMode::eLocalTime: {
//    cmd.setMessage("01:02:03|0");
      MyTime localTime = data.localTimestamp.getTime();
      char timeStrBuffer[MyTime::getStringBufferSize()];
      cmd.setMessage(localTime.toString(timeStrBuffer));
    break;
  }

  case DisplayMode::eDate: {
//    cmd.setMessage("04/05/06");
      MyDate localDate = data.localTimestamp.getDate();
      char dateStrBuffer[MyDate::getStringBufferSize()];
      cmd.setMessage(localDate.toString(dateStrBuffer));
    break;
  }

  default:;
  }
}

//===================================================================================