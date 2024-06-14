#pragma once

#include "Other/LimitedSizeString.h"
#include "TimeType/TimeData.h"
#include "freertos/FreeRTOS.h"

enum class DisplayMode {
  eLocalTime,
  eUTCTime,
  eLocalDate, 
  eSunsetTime,
  eSunriseTime
};

using CommandString = std::string;

//===================================================================================
class DisplayCommand {
private:
  DisplayMode mode;
  CommandString msg;

public:
  DisplayCommand(void);
  ~DisplayCommand(void) = default;

  const DisplayMode &getCmdMode(void);
  void setCmdMode(const DisplayMode mode);

  const CommandString &getMessage(void);
  void setMessage(const CommandString cmdMsg);
};

//===================================================================================
class DisplayController {
private:
  SemaphoreHandle_t SemaphoreDisplayController;

  DisplayMode displayMode;
  DisplayCommand cmd;

public:
  DisplayController(void);
  ~DisplayController(void) = default;

  void setDisplayMode(const DisplayMode mode);
  void update(TimeData &data);

  const DisplayCommand &getCommand(void);

  bool lockData(void);
  void unlockData(void);
};

//===================================================================================
