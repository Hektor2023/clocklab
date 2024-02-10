#include "Display/TimestampObserver.h"

#include <cstddef>
//===================================================================================
TimestampObserver::TimestampObserver(void)
{

};

//===================================================================================
Timestamp TimestampObserver::getTimestamp( void)
{
  return timestamp;
}

//===================================================================================
void TimestampObserver::update( Timestamp &newTimestamp)
{
  timestamp = newTimestamp;
  for( int8_t i = 0; i < idx; i++)
  {
    listeners[ i]->update( this);
  }

};

//===================================================================================
void TimestampObserver::addListener( TimestampListener *listerner)
{
  if( idx <  MaxNumberListeners) 
  {
    listeners[ idx] = listerner;
    idx++;
  }
  
};
