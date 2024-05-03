#pragma once

#include "Other/CoordinatesHandler.h"
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
  Timestamp dSTStartTimestamp;
  Timestamp dSTEndTimestamp;
  Timestamp localTimestamp;

  MyTime sunriseTime;
  MyTime sunsetTime;

  CoordinatesHandler coordinatesHandler;
  const uint16_t standardTimeOffset;

public:
  DSTSunriseSunsetTimeHandler(uint16_t standardTimeOffset);
  ~DSTSunriseSunsetTimeHandler(void) = default;

  void update(TimeData &timedata);

  CoordinatesHandler &getCoordinatesHander(void);

private:
  void calculateDST(Timestamp &resultDSTStartTimestamp,
                    Timestamp &resultDSTEndTimestamp, MyDate &date);
  void calculateSunriseSunset(const Timestamp &dSTStart,
                              const Timestamp &dSTEnd, MyTime &sunrise,
                              MyTime &sunset, const Coordinates_t &coordinates,
                              const Timestamp &timestamp);
  uint16_t getOffset(Timestamp &current);

  long modd(double a, double b);
};

//===================================================================================
