#include "Source/GPS/GPSTask.h"


//=============================================================================================================
void gpsTask(void *pvParameter)
{
  QueueHandle_t*  ptr2queueSource = reinterpret_cast< QueueHandle_t*>( pvParameter);
  GPSTimeHandler2 GPSHandler;  

  Timestamp   lastGpsTime;
  Serial2.begin(9600);
  Serial2.flush();

  vTaskDelay( 800 / portTICK_RATE_MS);
  Serial.print( "\nGPS_task:  start\n");  

  for(;;)
  {
    vTaskDelay(  100 / portTICK_RATE_MS);

//    Serial.printf("| GPS...|");

    bool next= true;
    while( next)
    {
      vTaskDelay( 7 / portTICK_RATE_MS);
      if( !Serial2.available())  
      {
//        Serial.printf( "GPS: !Serial2.available\n");
//        Serial.printf( "*");
        continue;
      }

      char c = Serial2.read();
      next= GPSHandler.collectRecord( c);
    };

    if( !GPSHandler.updateTime())
    {
      continue;
    }

    Timestamp diffTime= GPSHandler.getTimestamp() - lastGpsTime;
    displayTimestamp( "diffTime", diffTime);
    if( diffTime.getEpochTime() < (1 * 60))
    {
      continue;
    }

    lastGpsTime = GPSHandler.getTimestamp();
//    Serial.printf( "GPS: encoded\n");

    MessageTime_t   gps_msg;
    gps_msg.type=   src_type_t::GPS;
    gps_msg.epoch=  GPSHandler.getTimestamp().getEpochTime(); 
    gps_msg.coordinate = GPSHandler.getCoordinate(); 

    uint32_t  epochMillis= (uint32_t) GPSHandler.getMilliSecond();
   
    // set next entire second
    vTaskDelay( ( 1000 -  epochMillis)/ portTICK_RATE_MS);
  

    Timestamp timestamp( gps_msg.epoch); 
    displayTimestamp( "GPS", timestamp);
     
    while ( xQueueSend( *ptr2queueSource, (void *)&gps_msg, 0) != pdTRUE) 
    {
      Serial.println("ERROR: Could not put GPS time to queue."); 
      vTaskDelay( 2 / portTICK_RATE_MS); 
    }
//    Serial.printf("| GPS...!!!!|\n");

  }

  vTaskDelete(nullptr);
}

//=============================================================================================================

