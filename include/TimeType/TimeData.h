#pragma once

#include "TimeType/MyTime.h"
#include "TimeType/Timestamp.h"
#include "clocklab_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//===================================================================================
struct TimeData {
  Timestamp UTCTimestamp;
  Timestamp localTimestamp;

  Timestamp dSTStartTimestamp;
  Timestamp dSTEndTimestamp;

  MyTime sunriseTime;
  MyTime sunsetTime;

  Coordinates_t coordinates;

  uint8_t dayOfWeek; // Monday =0

  TimeData(void);

  bool lockData(void);
  void unlockData(void);

private:
  SemaphoreHandle_t SemaphoreTimeData;
};

//===================================================================================
