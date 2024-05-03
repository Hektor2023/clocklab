#include "TimeType/TimeData.h"

//===================================================================================
TimeData::TimeData(void)
    : SemaphoreTimeData(xSemaphoreCreateMutex())
{
}

//===================================================================================
bool TimeData::lockData(void)
{
  return xSemaphoreTake(SemaphoreTimeData, 0) == pdTRUE;
}

//===================================================================================
void TimeData::unlockData(void)
{
  xSemaphoreGive(SemaphoreTimeData);
}

//===================================================================================
