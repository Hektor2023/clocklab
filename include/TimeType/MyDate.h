#pragma once

#include "Arduino.h"
#include <cstddef>

typedef unsigned char uint8_t;

//===================================================================================
class MyDate {
private:
  uint8_t day;
  uint8_t month;
  uint16_t year;

public:
  MyDate(void);
  MyDate(uint8_t day, uint8_t month, uint16_t year);
  MyDate(const MyDate &date);
  ~MyDate(void);

  static bool isLapYear(uint16_t _year);
  static uint16_t daysInYear(uint16_t _year);

  MyDate &operator=(const MyDate &date);

  uint8_t getDay(void) const;
  void setDay(const uint8_t day);

  uint8_t getMonth(void) const;
  void setMonth(const uint8_t month);

  uint16_t getYear(void) const;
  void setYear(const uint16_t year);

  void dayInc(void);
  void dayDec(void);
  void monthInc(void);
  void monthDec(void);
  void yearInc(void);
  void yearDec(void);

  // 0 = Sunday, 1 = Monday ... 7 = Unknown
  uint8_t getDayOfWeek(void) const;
  const char *getDayOfWeekAsString(void);

  static MyDate getDSTStart(uint16_t year);
  static MyDate getDSTEnd(uint16_t year);

  static bool isValidDay(uint8_t day, uint8_t month, uint16_t year);
  static bool isValidMonth(uint8_t month);
  static bool isValidYear(uint16_t year);

  static const char *getStringFormat(void);
  static size_t getStringBufferSize(void);
  char *toString(char *buffer);

private:
  uint8_t getMonthCode(void) const;
  uint8_t getYearCode(void) const;
  uint8_t getCenturyCode(void) const;
  uint8_t getDayCode(void) const;
  uint8_t getLeapCode(void) const;
};

//===================================================================================
