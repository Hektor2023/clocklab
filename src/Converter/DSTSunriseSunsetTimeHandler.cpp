#include "Converter/DSTSunriseSunsetTimeHandler.h"

DSTSunriseSunsetTimeHandler::DSTSunriseSunsetTimeHandler(
    uint16_t standardTimeOffset)
    : standardTimeOffset(standardTimeOffset), dSTStartTimestamp(0),
      dSTEndTimestamp(0), localTimestamp(0), sunriseTime(), sunsetTime() {
  MyTime time(1, 0, 0); // 1:0:0  change time on 1st am universal time

  dSTStartTimestamp.setTime(time);
  dSTEndTimestamp.setTime(time);
}

CoordinatesHandler &DSTSunriseSunsetTimeHandler::getCoordinatesHander(void) {
  return coordinatesHandler;
}

void DSTSunriseSunsetTimeHandler::update(TimeData &timeData) {
  Timestamp UTCtimestamp = timeData.UTCTimestamp;

  static uint16_t lastYear = 0;
  // compare timestamps

  MyDate date = UTCtimestamp.getDate();
  if (date.getYear() != lastYear) {
    lastYear = date.getYear();
    calculateDST(timeData.dSTStartTimestamp, timeData.dSTEndTimestamp, date);
  }

  static bool needUpdateSunriseSunset = false;
  static uint8_t lastday = 0;
  if ((date.getDay() != lastday) &&
      ((coordinatesHandler.getCoordinates().latitude != 0) &&
       (coordinatesHandler.getCoordinates().longitude != 0))) {
    needUpdateSunriseSunset = true;
  }

  Serial.printf("\n needUpdateSunriseSunset= %d\n",
                (int)needUpdateSunriseSunset);
  if (needUpdateSunriseSunset && coordinatesHandler.lockData()) {
    lastday = date.getDay();
    calculateSunriseSunset(timeData.dSTStartTimestamp, timeData.dSTEndTimestamp,
                           timeData.sunriseTime, timeData.sunsetTime,
                           coordinatesHandler.getCoordinates(), UTCtimestamp);

    needUpdateSunriseSunset = false;

    Serial.printf("\n SunriseSunset= updatedd\n");
    coordinatesHandler.releaseData();
  }
  char dateStrBuffer[MyDate::getStringBufferSize()];
  MyDate tmpDate;
  tmpDate = timeData.dSTStartTimestamp.getDate();
  Serial.printf("\nNew DSTstart:  %s ", tmpDate.toString(dateStrBuffer));
  tmpDate = timeData.dSTEndTimestamp.getDate();
  Serial.printf("-  New DSTend: %s\n", tmpDate.toString(dateStrBuffer));

  uint16_t currentTimeOffset = getOffset(UTCtimestamp);
  Serial.printf("\n currentTimeOffset= %d\n", currentTimeOffset);
  timeData.localTimestamp = UTCtimestamp + currentTimeOffset;
}

void DSTSunriseSunsetTimeHandler::calculateDST(
    Timestamp &resultDSTStartTimestamp, Timestamp &resultDSTEndTimestamp,
    MyDate &date) {
  MyDate dSTdateStart = MyDate::getDSTStart(date.getYear());
  MyDate dSTdateEnd = MyDate::getDSTEnd(date.getYear());

  resultDSTStartTimestamp.setDate(dSTdateStart);
  resultDSTEndTimestamp.setDate(dSTdateEnd);

  char dateStrBuffer[MyDate::getStringBufferSize()];
  Serial.printf("\nNew DSTstart:  %s ", dSTdateStart.toString(dateStrBuffer));
  Serial.printf("-  New DSTend: %s\n", dSTdateEnd.toString(dateStrBuffer));
}

void DSTSunriseSunsetTimeHandler::calculateSunriseSunset(
    const Timestamp &dSTStart, const Timestamp &dSTEnd, MyTime &sunrise,
    MyTime &sunset, const Coordinates_t &coordinates,
    const Timestamp &timestamp) {

  MyDate date = timestamp.getDate();
  uint8_t day = date.getDay();
  uint8_t month = date.getMonth();
  uint16_t year = date.getYear();

  double E6 = (month <= 2) ? (month + 12) : month;
  double E7 = (month <= 2) ? (year - 1) : year;
  double L6 = 2 - int(year / 100) + int(year / 400);
  double L7 =
      int(365.25 * (E7 + 4716)) + int(30.6001 * (E6 + 1)) + day + L6 - 1524.5;
  double M3 = (L7 - 2451545) / 36525;
  double M4 = 280.46646 + 36000.76983 * M3 + 0.0003032 * M3 * M3;
  double M5 = 357.52911 + 35999.05029 * M3 - 0.0001537 * M3 * M3;
  double O5 = modd((M5 / 360), 360) * GradToRad;
  double O4 = modd((M4 / 360), 360);
  double M6 = (1.914602 - 0.004817 * M3 - 0.000014 * M3 * M3) * sin(O5);
  double M7 = (0.019993 - 0.000101 * M3) * sin(2 * O5);
  double M8 = 0.000289 * sin(3 * O5);
  double M9 = M6 + M7 + M8;

  double N7 = 125.04 - 1934.136 * M3;
  double N9 = (N7 < 0) ? (N7 + 360) : N7;

  double M11 = 23.43930278 - 0.0130042 * M3 - 0.000000163 * M3 * M3;
  double N10 = O4 + M9 - 0.00569 - 0.00478 * sin(N9 * GradToRad);

  double N11 = asin(sin(M11 * GradToRad) * sin(N10 * GradToRad));

  double N16 = -0.01483 - sin(N11) * sin(coordinates.latitude * GradToRad);
  double O16 = cos(N11) * cos(coordinates.latitude * GradToRad);

  double Z =
      13 - coordinates.longitude / 15 +
      (7.7 * sin((O4 + 78) * GradToRad) - 9.5 * sin(2 * O4 * GradToRad)) / 60;
  double P15 = (acos(N16 / O16) * 57.29577951) / 15;

  // Z-1 in Summer, Z in Winter
  constexpr uint8_t oneHour = 1;

  Z = (const_cast<Timestamp &>(dSTStart) <= timestamp) && ((const_cast<Timestamp &>(dSTEnd) > timestamp))? Z + oneHour: Z;
  double P17 = Z - P15; //- godzina wschodu Słońca
  double Q17 = Z + P15; //- godzina zachodu Słońca

  sunrise.setHour((uint8_t)P17);
  sunrise.setMinute((uint8_t)modd(P17, 60));
  sunrise.setSecond((uint8_t)modd(P17, 3600) / 10);

  sunset.setHour((uint8_t)Q17);
  sunset.setMinute((uint8_t)modd(Q17, 60));
  sunset.setSecond((uint8_t)modd(Q17, 3600) / 10);
}

uint16_t DSTSunriseSunsetTimeHandler::getOffset(Timestamp &current) {
  uint16_t currentTimeOffset = standardTimeOffset - SECS_PER_HOUR;

  if ((dSTStartTimestamp <= current) && (current < dSTEndTimestamp)) {
    // UTC+2 from Marz to October
    currentTimeOffset = standardTimeOffset;
  }

  return (currentTimeOffset);
}

long DSTSunriseSunsetTimeHandler::modd(double a, double b) {
  return ((a - long(a)) * b);
}
