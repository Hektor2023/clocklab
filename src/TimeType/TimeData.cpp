#include "TimeType/TimeData.h"


//===================================================================================
TimeData::TimeData( void)
: SemaphoreTimeData( xSemaphoreCreateMutex())
{
}

//===================================================================================
bool TimeData::lockData( void)
{
  return xSemaphoreTake( SemaphoreTimeData,0) == pdTRUE;
}

//===================================================================================
void TimeData::releaseData( void)
{
  xSemaphoreGive(  SemaphoreTimeData);
}

//===================================================================================
