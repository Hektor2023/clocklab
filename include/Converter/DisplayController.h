#pragma once

#include "Other/LimitedSizeString.h"
#include "TimeType/TimeData.h"



enum class DisplayMode { eLocalTime, eUTCTime, eSunsetTime, eSunriseTime };

constexpr uint8_t cmdStringSize = 30;
using CommandString = LimitedSizeString<cmdStringSize>;

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
  DisplayMode displayMode;
  DisplayCommand cmd;

public:
  DisplayController(void);
  ~DisplayController(void) = default;

  void setDisplayMode(const DisplayMode mode);
  void update(TimeData &data);

  const DisplayCommand &getCommand(void);
};

//===================================================================================
