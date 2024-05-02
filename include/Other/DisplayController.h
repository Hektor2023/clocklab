#pragma once

#include "LimitedSizeString.h"
#include "TimeType/TimeData.h"

constexpr uint8_t cmdStringSize = 30;

enum class DisplayMode { eLocalTime, eDate, eSunsetTime, eSunriseTime };

//===================================================================================
class DisplayCommand {
private:
  DisplayMode mode;
  LimitedSizeString<cmdStringSize> msg;

public:
  DisplayCommand(void);
  ~DisplayCommand(void) = default;

  const DisplayMode &getCmdMode(void);
  void setCmdMode(const DisplayMode mode);

  const LimitedSizeString<cmdStringSize> &getMessage(void);
  void setMessage(const LimitedSizeString<cmdStringSize> cmdMsg);
};

class DisplayController {
private:
  DisplayMode displayMode;
  DisplayCommand cmd;

public:
  DisplayController(void);
  ~DisplayController(void) = default;

  void setDisplayMode(const DisplayMode mode);
  void update(const TimeData &data);

  const DisplayCommand &getCommand(void);
};

//===================================================================================
