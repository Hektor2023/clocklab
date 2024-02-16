#include "Source/NTP/NTPTask.h"

//=============================================================================================================
void ntpTask(void *pvParameter)
{
  t_credentials   credentials;
  credentials.gc_Ssid =     SSID;
  credentials.gc_Password = PASSWD;
  NTPSourceTime   ntpSource( credentials);

  QueueHandle_t *pg_queue= ( QueueHandle_t *)pvParameter;

  for(;;)
  {
    vTaskDelay( 20*(1000 / portTICK_RATE_MS));

    MessageTime_t   ntp_msg;
    ntp_msg.type=   src_type_t::NTP;
    ntp_msg.epoch=  ntpSource.update();

    Timestamp  timestamp;
    timestamp.setEpochTime( ntp_msg.epoch);
    displayTimestamp( "NTP", timestamp);

    while ( xQueueSend( *pg_queue, (void *)&ntp_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

  }
  vTaskDelete(nullptr);
}

//=============================================================================================================

