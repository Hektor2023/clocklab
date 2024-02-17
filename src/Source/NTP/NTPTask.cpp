#include "Source/NTP/NTPTask.h"

constexpr TickType_t OneSecond = (1000 / portTICK_RATE_MS);

//=============================================================================================================
void ntpTask(void *pvParameter)
{
  ntpTaskParams_t *pparams= reinterpret_cast< ntpTaskParams_t*>( pvParameter);
  t_credentials             credentials;
  credentials.gc_Ssid =     pparams->ssid;
  credentials.gc_Password = pparams->passwd;
  static NTPSourceTime   ntpSource( credentials);

  for(;;)
  {
    vTaskDelay( 20 *OneSecond);

    MessageTime_t   ntp_msg;
    ntp_msg.type=   src_type_t::NTP;
    ntp_msg.epoch=  ntpSource.update();

    Timestamp  timestamp( ntp_msg.epoch);
    displayTimestamp( "NTP", timestamp);

    while ( xQueueSend( *(pparams->srcQueue), (void *)&ntp_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

  }
  vTaskDelete(nullptr);
}

//=============================================================================================================

