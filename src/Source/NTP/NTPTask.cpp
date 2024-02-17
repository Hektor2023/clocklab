#include "Source/NTP/NTPTask.h"


//=============================================================================================================
void ntpTask(void *pvParameter)
{
//  ntpTaskParams_t *pparams= reinterpret_cast< ntpTaskParams_t*>( pvParameter);
  t_credentials             credentials;
  credentials.gc_Ssid =     SSID; //pparams->ssid;
  credentials.gc_Password = PASSWD;//pparams->passwd;
  NTPSourceTime   ntpSource( credentials);
  MessageTime_t   ntp_msg;

  for(;;)
  {
    vTaskDelay( 20 *(1000 / portTICK_RATE_MS));

    ntp_msg.type=   src_type_t::NTP;
    ntp_msg.epoch=  ntpSource.update();

    Timestamp  timestamp( ntp_msg.epoch);
    displayTimestamp( "NTP", timestamp);

    while ( xQueueSend( *( reinterpret_cast<QueueHandle_t*>( pvParameter)), (void *)&ntp_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put NTP time to queue.");  
    }

  }
  vTaskDelete(nullptr);
}

//=============================================================================================================

