#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "TimeType/MyTime.h"
#include "TimeType/MyDate.h"
#include "clocklab_types.h"

//===================================================================================
struct TimeData
{
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
