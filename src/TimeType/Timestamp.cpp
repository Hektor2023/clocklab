
#include "TimeType/Timestamp.h"
#include "Arduino.h"

constexpr uint16_t BASE_YEAR = 1970;
constexpr uint8_t BASE_MONTH = 1;
constexpr uint8_t BASE_DAY = 1;

//===================================================================================
Timestamp::Timestamp(uint32_t epoch) : epoch(epoch){};

//===================================================================================
Timestamp::Timestamp(const Timestamp &t) : epoch(t.epoch){};

//===================================================================================
Timestamp::~Timestamp(void){};

//===================================================================================
Timestamp &Timestamp::operator=(const Timestamp &t) {
  if (&t != this) {
    epoch = t.epoch;
  }
  return (*this);
}

//===================================================================================
uint32_t Timestamp::getEpochTime(void) const { return (epoch); }

//===================================================================================
void Timestamp::setEpochTime(const uint32_t newEpochTime) {
  epoch = newEpochTime;
}

//===================================================================================
int Timestamp::getDayOfWeek(void) const {
  MyDate date = getDate();

  return (date.getDayOfWeek());
}

//===================================================================================
Timestamp &Timestamp::operator++(void) // Prefix increment operator.
{
  epoch++;
  return (*this);
}

//===================================================================================
Timestamp Timestamp::operator++(int) {
  epoch++;
  return (*this);
}

//===================================================================================
bool Timestamp::operator==(const Timestamp &t) { return (epoch == t.epoch); }

//===================================================================================
bool Timestamp::operator!=(const Timestamp &t) { return (epoch != t.epoch); }

//===================================================================================
bool Timestamp::operator<(const Timestamp &t) { return (epoch < t.epoch); };

//===================================================================================
bool Timestamp::operator<=(const Timestamp &t) { return (epoch <= t.epoch); };

//===================================================================================
bool Timestamp::operator>(const Timestamp &t) { return (epoch > t.epoch); };

//===================================================================================
bool Timestamp::operator>=(const Timestamp &t) { return (epoch >= t.epoch); };

//===================================================================================
Timestamp Timestamp::operator+(const Timestamp &t) {
  Timestamp temp;
  temp.epoch = epoch + t.epoch;
  return temp;
};

//===================================================================================
Timestamp Timestamp::operator-(const Timestamp &t) {
  Timestamp temp;
  temp.epoch = epoch - t.epoch;
  return temp;
};

//===================================================================================
Timestamp &Timestamp::operator+=(const Timestamp &t) {
  epoch = epoch + t.epoch;
  return *this;
}

//===================================================================================
Timestamp &Timestamp::operator-=(const Timestamp &t) {
  epoch = epoch - t.epoch;
  return *this;
}

//===================================================================================
const MyDate Timestamp::getDate(void) const {
  //                      1   2   3   4   5   6   7   8   9   10  11  12
  const uint8_t dofm[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  //    Serial.printf( "epoch: %u\n", epoch);
  // calucate year and rest of days
  uint16_t days = epoch / SECS_PER_DAY;
  //    days += BASE_DAY;

  //    Serial.printf( "days1: %u\n", days);
  uint16_t year = BASE_YEAR;
  while (days > MyDate::daysInYear(year)) {
    days = days - MyDate::daysInYear(year);

    year++;
  };
  //   Serial.printf( "days2: %u\n", days);

  // calculate months and day
  uint8_t month;
  for (month = BASE_MONTH; month < 12; month++) {
    if (days <= dofm[month - 1]) {
      break;
    }

    if ((month == 2) && MyDate::isLapYear(year)) {
      days--;
    }

    days = days - dofm[month - 1];
  }
  days++;
  //  Serial.printf( "-> %u-%u-%u\n", days, month, year);

  MyDate date;
  date.setYear(year);
  date.setMonth(month);
  date.setDay(days);
  return date;
}

//===================================================================================
const MyTime Timestamp::getTime(void) const {
  uint32_t days = this->epoch % SECS_PER_DAY;

  MyTime time;
  time.setHour(days / SECS_PER_HOUR);

  uint16_t seconds = days % SECS_PER_HOUR;
  time.setMinute(seconds / SECS_PER_MIN);
  time.setSecond(seconds % SECS_PER_MIN);
  return time;
}

//===================================================================================
uint32_t Timestamp::sumDaysOfFullYearsSinceBase(uint32_t year) const {
  uint32_t sum = 0;

  for (uint16_t y = BASE_YEAR; y < year; y++) {
    sum += MyDate::daysInYear(y);
  }

  return (sum);
}

//===================================================================================
uint16_t Timestamp::sumDaysOfFullMonths(uint8_t month) const {
  uint16_t days = 0;

  //                     0   1   2   3   4   5   6   7   8   9   10  11  12
  const uint8_t dofm[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  for (uint8_t m = 0; m < month; m++) {
    days += dofm[m];
  }

  return (days);
}

//===================================================================================
void Timestamp::setDate(const MyDate &date) {
  uint32_t days = 0;

  days = sumDaysOfFullYearsSinceBase(date.getYear());
  days += sumDaysOfFullMonths(date.getMonth());
  days += date.getDay(); //  -BASE_DAY;

  epoch = epoch % SECS_PER_DAY;
  epoch += (days * SECS_PER_DAY);
}

//===================================================================================
void Timestamp::setTime(const MyTime &time) {
  this->epoch = (this->epoch / SECS_PER_DAY) * SECS_PER_DAY;

  this->epoch += time.getHour() * SECS_PER_HOUR;
  this->epoch += time.getMinute() * SECS_PER_MIN;
  this->epoch += time.getSecond();
}

//===================================================================================
size_t Timestamp::getStringBufferSize(void) {
  return (strlen(MyDate::getStringFormat()) + strlen("|") +
          strlen(MyTime::getStringFormat()) + strlen("|%d"));
}

//===================================================================================
char *Timestamp::toString(char *ptr) {
  MyDate date = getDate();
  MyTime time = getTime();

  char dateStrBuffer[MyDate::getStringBufferSize()];
  char timeStrBuffer[MyTime::getStringBufferSize()];
  sprintf(ptr, "%s|%s|%d", date.toString(dateStrBuffer),
          time.toString(timeStrBuffer), date.getDayOfWeek());
  return (ptr);
}

//===================================================================================
