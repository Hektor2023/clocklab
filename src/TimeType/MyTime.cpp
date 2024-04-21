#include "TimeType/MyTime.h"

//===================================================================================
MyTime::MyTime(void) : hour(0), minute(0), second(0) {}

//===================================================================================
MyTime::MyTime(uint8_t hour, uint8_t minute, uint8_t second) : hour(hour), minute(minute), second(second) {}

//===================================================================================
MyTime::MyTime(const MyTime &time) : hour(time.hour), minute(time.minute), second(time.second) {}

//===================================================================================
MyTime::~MyTime(void) {}

//===================================================================================
bool operator==(MyTime const &time1, MyTime const &time2)
{
  return ((time1.hour == time2.hour) && (time1.minute == time2.minute) && (time1.second == time2.second));
}

//===================================================================================
bool operator!=(MyTime const &time1, MyTime const &time2)
{
  return (!(time1 == time2));
}

//===================================================================================
uint8_t MyTime::getHour(void) const
{
  return (hour);
}

//===================================================================================
void MyTime::setHour(uint8_t hour)
{
  this->hour = hour;
}

//===================================================================================
uint8_t MyTime::getMinute(void) const
{
  return (minute);
}

//===================================================================================
void MyTime::setMinute(uint8_t minute)
{
  this->minute = minute;
}

//===================================================================================
uint8_t MyTime::getSecond(void) const
{
  return (second);
}

//===================================================================================
void MyTime::setSecond(uint8_t second)
{
  this->second = second;
}

//===================================================================================
void MyTime::hourInc(void)
{
  hour = (hour < 24) ? hour + 1 : 0;
}

//===================================================================================
void MyTime::hourDec(void)
{
  hour = (hour > 0) ? hour - 1 : 23;
}

//===================================================================================
void MyTime::minuteInc(void)
{
  minute = (minute < 60) ? minute + 1 : 0;
}

//===================================================================================
void MyTime::minuteDec(void)
{
  minute = (minute > 0) ? minute - 1 : 59;
}

//===================================================================================
void MyTime::secondInc(void)
{
  second = (second < 60) ? second + 1 : 0;
}

//===================================================================================
void MyTime::secondDec(void)
{
  second = (second > 0) ? second - 1 : 59;
}

//===================================================================================
bool MyTime::isValidHour(uint8_t hour)
{
  return (hour < 24);
}

//===================================================================================
bool MyTime::isValidMinute(uint8_t minute)
{
  return (minute < 60);
}

//===================================================================================
bool MyTime::isValidSecond(uint8_t second)
{
  return (second < 60);
}

//===================================================================================
MyTime &MyTime::operator=(const MyTime &time)
{
  if (&time != this)
  {
    hour = time.hour;
    minute = time.minute;
    second = time.second;
  }
  return (*this);
}

//===================================================================================
MyTime &MyTime::operator++(void)
{

  second++;
  if (!isValidSecond(second))
  {
    second = 0;
    minute++;
    if (!isValidMinute(minute))
    {
      minute = 0;
      hour++;
      if (!isValidHour(hour))
      {
        hour = 0;
      }
    }
  }

  return (*this);
}

//===================================================================================
MyTime MyTime::operator++(int)
{
  ++(*this);
  return (*this);
}

//===================================================================================
uint16_t MyTime::getInSeconds(void)
{
  return (getHour() * 3600 + getMinute() * 60 + getSecond());
}

//===================================================================================
void MyTime::setInSeconds(uint16_t timeInSecs)
{
  uint16_t tdiff = timeInSecs;

  setHour((uint8_t)(tdiff / 3600));
  tdiff %= 3600;
  setMinute((uint8_t)(tdiff / 60));
  setSecond((uint8_t)(tdiff % 60));
}

//===================================================================================
const char *MyTime::getStringFormat(void)
{
  return ("%02d:%02d:%02d");
}

//===================================================================================
const char *MyTime::getStringShortFormat(void)
{
  return ("%02d:%02d");
}

//===================================================================================
size_t MyTime::getStringBufferSize(void)
{
  return (strlen(MyTime::getStringFormat()) + 1);
}

//===================================================================================
size_t MyTime::getStringShortBufferSize(void)
{
  return (strlen(MyTime::getStringShortFormat()) + 1);
}

//===================================================================================
char *MyTime::toString(char *ptr)
{
  sprintf(ptr, MyTime::getStringFormat(), hour, minute, second);
  return (ptr);
}

//===================================================================================
char *MyTime::toShortString(char *ptr)
{
  sprintf(ptr, MyTime::getStringShortFormat(), hour, minute);
  return (ptr);
}

//===================================================================================
