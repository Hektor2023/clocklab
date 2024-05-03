#pragma once

#include "TimeType/TimeData.h"
#include "TimeType/Timehandler.h"
#include "TimeType/Timestamp.h"
#include "clocklab_types.h"
#include <stdint.h>

constexpr auto RadToGrad = 180.0 / PI;
constexpr auto GradToRad = PI / 180.0;

//===================================================================================
class DSTSunriseSunsetTimeHandler {
private:
  Timestamp localTimestamp;

  const uint16_t standardTimeOffset;

public:
  DSTSunriseSunsetTimeHandler(uint16_t standardTimeOffset);
  ~DSTSunriseSunsetTimeHandler(void) = default;

  void update(TimeData &timedata);

private:
  void calculateDST(Timestamp &resultDSTStartTimestamp,
                    Timestamp &resultDSTEndTimestamp, MyDate &date);
  void calculateSunriseSunset(const Timestamp &dSTStart,
                              const Timestamp &dSTEnd, MyTime &sunrise,
                              MyTime &sunset, const Coordinates_t &coordinates,
                              const Timestamp &timestamp);
  uint16_t getOffset(Timestamp &dSTStart,Timestamp &dSTEnd,Timestamp &current);

  long modd(double a, double b);
};

//===================================================================================
