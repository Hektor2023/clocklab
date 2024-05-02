#pragma once

#include "TimeType/MyDate.h"
#include "TimeType/MyTime.h"
#include "clocklab_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//===================================================================================
struct TimeData {
  MyDate localDate;
  MyTime localTime;
  MyTime sunriseTime;
  MyTime sunsetTime;

  TimeData(void);

  bool lockData(void);
  void releaseData(void);

private:
  SemaphoreHandle_t SemaphoreTimeData;
};

//===================================================================================
