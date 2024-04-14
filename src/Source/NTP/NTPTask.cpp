#include "Source/NTP/NTPTask.h"


//=============================================================================================================
void ntpTask(void *pvParameter)
{
  ntpTaskParams_t *pparams= reinterpret_cast< ntpTaskParams_t*>( pvParameter);
  t_credentials             credentials;
  credentials.gc_Ssid =     pparams->ssid;
  credentials.gc_Password = pparams->passwd;
  NTPSourceTime   ntpSource( credentials);


  for(;;)
  {
    vTaskDelay( 20 *(1000 / portTICK_RATE_MS));

    MessageTime_t   ntp_msg;
    ntp_msg.type=   src_type_t::NTP;
    ntp_msg.epoch=  ntpSource.getTimestamp().getEpochTime();

    Timestamp  timestamp( ntp_msg.epoch);
    displayTimestamp( "NTP", timestamp);

    QueueHandle_t* ptr= pparams->srcQueue; 
    while ( xQueueSend( *ptr, (void *)&ntp_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

  }
  vTaskDelete(nullptr);
}

//=============================================================================================================

