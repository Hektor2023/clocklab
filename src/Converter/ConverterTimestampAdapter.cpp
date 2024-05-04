#include "Converter/ConverterTimestampAdapter.h"

//===================================================================================
void ConverterTimestampAdapter::update(TimeData &data) {
  data.UTCTimestamp = myTimestamp;
}

void ConverterTimestampAdapter::setTimestamp(Timestamp &timestamp) {
  myTimestamp = timestamp;
}

//===================================================================================
