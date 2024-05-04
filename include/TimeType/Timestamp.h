#pragma once

#include <cstddef>
#include <string.h>

#include "TimeType/MyDate.h"
#include "TimeType/MyTime.h"

//===================================================================================
/* Useful Constants */
constexpr uint32_t SECS_PER_MIN = 60UL;
constexpr uint32_t SECS_PER_HOUR = 3600UL;
constexpr uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;

//===================================================================================
class Timestamp {
private:
  uint32_t epoch;

public:
  Timestamp(uint32_t epoch = 0L);
  Timestamp(const Timestamp &t);
  ~Timestamp(void);

  uint32_t getEpochTime(void) const;
  void setEpochTime(const uint32_t newEpochTime);

  int getDayOfWeek(void) const;

  Timestamp &operator=(const Timestamp &t);
  Timestamp &operator++(void);
  Timestamp operator++(int);

  bool operator==(const Timestamp &t);
  bool operator!=(const Timestamp &t);
  bool operator<(const Timestamp &t);
  bool operator<=(const Timestamp &t);
  bool operator>(const Timestamp &t);
  bool operator>=(const Timestamp &t);

  Timestamp operator+(const Timestamp &t);
  Timestamp operator-(const Timestamp &t);
  Timestamp &operator+=(const Timestamp &t);
  Timestamp &operator-=(const Timestamp &t);

  const MyDate getDate(void) const;
  void setDate(const MyDate &date);

  const MyTime getTime(void) const;
  void setTime(const MyTime &time);

  static size_t getStringBufferSize(void);
  char *toString(char *Stringbuffer);

private:
  uint32_t sumDaysOfFullYearsSinceBase(uint32_t year) const;
  uint16_t sumDaysOfFullMonths(uint8_t month) const;
};

//===================================================================================

