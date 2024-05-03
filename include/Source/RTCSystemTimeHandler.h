/**
 * DS1307, DS3231 and DS3232 RTCs basic library
 *
 * Really tiny library to basic RTC functionality on Arduino.
 *
 * Supported features:
 *     * SQuare Wave Generator
 *     * Fixed output pin for DS1307
 *     * RAM for DS1307 and DS3232
 *     * temperature sensor for DS3231 and DS3232
 *     * Alarms (1 and 2) for DS3231 and DS3232
 *     * Power failure check for DS3231 and DS3232
 *
 * See uEEPROMLib for EEPROM support.
 *
 * @copyright Naguissa
 * @author Naguissa
 * @url https://github.com/Naguissa/uRTCLib
 * @url
 * https://www.foroelectro.net/librerias-arduino-ide-f29/rtclib-arduino-libreria-simple-y-eficaz-para-rtc-y-t95.html
 * @email naguissa@foroelectro.net
 */

#pragma once

#include "Arduino.h"
#include "uRTCLib.h"

#include "TimeType/TimeHandler.h"

//===================================================================================
class RTCSystemTimeHandler {
private:
  uRTCLib rtc;
  Timestamp systemTimestamp;

public:
  RTCSystemTimeHandler(const uint8_t sda_pin, const uint8_t scl_pin,
                       const uint8_t irqIn_pin);
  ~RTCSystemTimeHandler(void) = default;

  void init(void);

  Timestamp &getTimestamp(void);
  void setTimestamp(const Timestamp &newTimestamp);

  void forceUpdateTime(void);
  bool updateTime(void);
};

//===================================================================================
