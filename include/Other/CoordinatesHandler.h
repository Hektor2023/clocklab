#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "TimeType/MyTime.h"
#include "TimeType/MyDate.h"
#include "clocklab_types.h"

class CoordinatesHandler
{
    private:
        Coordinates_t       coordinates;   
        SemaphoreHandle_t   semaphoreCoordinates;

    public:
        CoordinatesHandler( void);
        ~CoordinatesHandler( void) = default;

        const Coordinates_t& getCoordinates( void);
        void  setCoordinates( const Coordinates_t newCoordinate);
        void  setCoordinates( double longitude, double  latitude);

        bool lockData( void);
        void releaseData( void);

};
