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
#include "Source/RTCSystemTimeHandler.h"

static constexpr uint16_t Year2K = 2000;
// uRTCLib rtc;
// volatile  uint8_t state = 1;

/*
void ICACHE_RAM_ATTR isr_extern_rtc()
{
    state = 1;
}
*/
//===================================================================================
RTCSystemTimeHandler::RTCSystemTimeHandler(const uint8_t sda_pin,
                                           const uint8_t scl_pin,
                                           const uint8_t irqIn_pin)
    : systemTimestamp(0L) {
  // changed orginal code because of mistake with pin assigment for I2C in
  // ESP8266
  URTCLIB_WIRE.begin(sda_pin, scl_pin); // 4,5 D2 and D1 on ESP8266

  // pin D8
  //  pinMode(  irqIn_pin, INPUT);
  // digitalWrite( irqIn_pin, HIGH); // RTC SQWG output is open drain we need
  // pullup build-in resistor
  //  attachInterrupt(  irqIn_pin, isr_extern_rtc,FALLING);
}

//===================================================================================
void RTCSystemTimeHandler::init(void) {
  rtc.sqwgSetMode(URTCLIB_SQWG_OFF_0);
  rtc.sqwgSetMode(URTCLIB_SQWG_1H);

  // for test
  rtc.set(0, 0, 0, 0, 5, 6, 0);
  //  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte
  //  dayOfMonth, byte month, byte year)
}

//===================================================================================
Timestamp &RTCSystemTimeHandler::getTimestamp(void) {
  return (systemTimestamp);
}

//===================================================================================
void RTCSystemTimeHandler::setTimestamp(const Timestamp &newTimestamp) {
  MyTime newTime = newTimestamp.getTime();
  MyDate newDate = newTimestamp.getDate();

  rtc.set(newTime.getSecond(), newTime.getMinute(), newTime.getHour(),
          newDate.getDayOfWeek(), newDate.getDay(), newDate.getMonth(),
          newDate.getYear() - Year2K);
  systemTimestamp.setEpochTime(newTimestamp.getEpochTime());
}

//===================================================================================
uint8_t RTCSystemTimeHandler::getSeconds(void) { return rtc.second(); }

//===================================================================================
void RTCSystemTimeHandler::refresh(void) { rtc.refresh(); }

//===================================================================================
void RTCSystemTimeHandler::forceUpdateTime(void) {
    refresh();

    MyTime time;
    time.setSecond(rtc.second());
    time.setMinute(rtc.minute());
    time.setHour(rtc.hour());
    systemTimestamp.setTime(time);

    MyDate date;
    date.setDay(rtc.day());
    date.setMonth(rtc.month());
    date.setYear(Year2K + rtc.year());
    systemTimestamp.setDate(date);
  }

  //===================================================================================
  bool RTCSystemTimeHandler::updateTime(void) {
    static uint8_t lastSec2 = 0;

    refresh();
    if (lastSec2 != rtc.second()) {
      //    state=0; // for irq handling

      lastSec2 = rtc.second();
      forceUpdateTime();
      return (true);
    }

    return (false);
  }

  //===================================================================================
