#include "Other/AdjustmentAdvisor.h"

#include <Arduino.h>
//=============================================================================================================
AdjustmentAdvisor::AdjustmentAdvisor(void)
    : selectedSource(src_type_t::NTP)
{
}

//=============================================================================================================
void AdjustmentAdvisor::setSelectedSource(src_type_t src)
{
  selectedSource = src;
}

//=============================================================================================================
bool AdjustmentAdvisor::routeSource(MessageTime_t &bestSrc_msg, const MessageTime_t &new_msg)
{
  const char *srcName[]{"NONE", "MANUAL", "RTC", "NTP", "GPS"};
  bool success = false;

  if (new_msg.type == selectedSource)
  {
    bestSrc_msg = new_msg;

    Serial.printf("Selected src: %s\n", srcName[(int)bestSrc_msg.type]);
    success = true;
  }

  return success;
}

//=============================================================================================================
