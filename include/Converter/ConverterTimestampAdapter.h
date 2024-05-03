#pragma once

#include "TimeType/TimeData.h"
#include "TimeType/Timestamp.h"

//===================================================================================
class ConverterTimestampAdapter 
{
    private:
        Timestamp myTimestamp;

    public:
        ConverterTimestampAdapter(void) = default;
        ~ConverterTimestampAdapter(void) = default;

        void update(TimeData &data);
        void setTimestamp(Timestamp &timestamp);
};

//===================================================================================
