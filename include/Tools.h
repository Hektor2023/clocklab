#pragma once

#include "Timestamp.h"

typedef struct {
  double  longitude;
  double  latitude;
} Coordinates_t;

void printTick(void);
void displayTimestamp( const char *srcName, Timestamp timestamp);

