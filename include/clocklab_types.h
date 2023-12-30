#ifndef clocklab_types_h
#define clocklab_types_h


#include <Arduino.h>

typedef uint32_t  epoch_t;

typedef enum
{ 
    NONE= 0,
    MANUAL,
    RTC,
    NTP,
    GPS
} src_type_t;

typedef struct {
  src_type_t  type= src_type_t::NONE;
  epoch_t     epoch;
  uint32_t    epochMillis;
  uint32_t    rtcMillis;
} MessageTime_t;

#endif