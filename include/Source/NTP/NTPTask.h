#pragma once 

#include "Source/NTP/NTPSourceTime.h"


extern void ntpTask(void *pvParameter);

typedef struct
{
    const char*     ssid;
    const char*     passwd;
    QueueHandle_t*  srcQueue;
} ntpTaskParams_t; 