#pragma once

#include <stdint.h>

#include "TimeType/Timestamp.h"
#include "clocklab_types.h"

constexpr unsigned char delimeterSizeMap = 20;
constexpr unsigned char bufferSize = 100;

//===================================================================================
class GPSTimeHandler2
{
protected:
   Timestamp GPSTimestamp;
   uint8_t milliSecond;
   Coordinates_t coordinates;

   signed char delimeterMap[delimeterSizeMap]; // map of delimeter positions
   char buffer[bufferSize];

private:
   void getField(const unsigned char fieldNo, char *field, const char *text);
   bool isValidRecord(void);
   bool isValidCheckSum(void);
   unsigned char calculateChecksum(void);
   unsigned char countDelimeter(void);
   void fillDelimeterMap(char *buffer);

public:
   GPSTimeHandler2(void);
   ~GPSTimeHandler2(void) = default;

   uint8_t getCentiSecond(void);

   Timestamp &getTimestamp(void);
   uint8_t getMilliSecond(void);

   Coordinates_t getCoordinate(void);

   bool updateTime(void);
   bool collectRecord(char c);
};

//===================================================================================
