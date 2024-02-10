#pragma once

#include "TimeType/Timestamp.h"


constexpr uint8_t MaxNumberListeners = 4;

class TimestampObserver;

//===================================================================================
class TimestampListener 
{
  public:  
    TimestampListener(void) = default;
    virtual ~TimestampListener(void) = default;

    virtual void update( TimestampObserver* observer) = 0;  
};

//===================================================================================
class TimestampObserver
{
  private:
    TimestampListener*   listeners[ MaxNumberListeners];
    uint8_t   idx;
    Timestamp timestamp;

  public:
    TimestampObserver(void);
    ~TimestampObserver(void) = default;

    Timestamp getTimestamp( void);

    void update( Timestamp &timestamp);
    void addListener( TimestampListener *listerner);
};

//===================================================================================

