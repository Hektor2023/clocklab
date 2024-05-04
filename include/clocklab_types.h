#pragma once

#include <Arduino.h>

//===================================================================================
typedef enum { NONE = 0, MANUAL, RTC, NTP, GPS } src_type_t;

typedef uint32_t epoch_t;

typedef struct {
  double longitude;
  double latitude;
} Coordinates_t;

typedef struct {
  src_type_t type = src_type_t::NONE;
  epoch_t epoch;
  Coordinates_t coordinate;
} MessageTime_t;

//===================================================================================