#include "Other/CoordinatesHandler.h"

#include <Arduino.h>

//===================================================================================
CoordinatesHandler::CoordinatesHandler( void)
: semaphoreCoordinates( xSemaphoreCreateMutex())
{

}

//===================================================================================
const Coordinates_t& CoordinatesHandler::getCoordinates( void)
{
    return coordinates;
}

//===================================================================================
void  CoordinatesHandler::setCoordinates( const Coordinates_t newCoordinates)
{
    coordinates = newCoordinates;
}

//===================================================================================
void  CoordinatesHandler::setCoordinates( double newLongitude, double  newLatitude)
{
    coordinates.longitude = newLongitude;
    coordinates.latitude =newLatitude;
}

//===================================================================================
bool CoordinatesHandler::lockData( void)
{
    return xSemaphoreTake( semaphoreCoordinates,0) == pdTRUE;
}

//===================================================================================
void CoordinatesHandler::releaseData( void)
{
    xSemaphoreGive( semaphoreCoordinates);
}

//=============================================================================================================
