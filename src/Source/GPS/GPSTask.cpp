#include "Source/GPS/GPSTask.h"

//=============================================================================================================
constexpr uint8_t oneMinute = 60;

void gpsTask(void *pvParameter)
{
  QueueHandle_t *ptr2queueSource = reinterpret_cast<QueueHandle_t *>(pvParameter);
  GPSTimeHandler2 GPSHandler;

  Timestamp lastGpsTime;
  Serial2.begin(9600);
  Serial2.flush();

  vTaskDelay(800 / portTICK_RATE_MS);
  Serial.print("\nGPS_task:  start\n");
  bool next = true;

  for (;;)
  {
    vTaskDelay(10 / portTICK_RATE_MS);

    if (next)
    {
      if (Serial2.available())
      {
        //        Serial.printf( "GPS: !Serial2.available\n");
        //        Serial.printf( "*");
        char c = Serial2.read();
        next = GPSHandler.collectRecord(c);
        //        Serial.printf( "%c", c);
      }
    }
    else
    {
      if (GPSHandler.updateTime())
      {
        //        Serial.printf( "+");
        Serial.printf("GPS: encoded\n");

        Timestamp diffTime = GPSHandler.getTimestamp() - lastGpsTime;

        uint32_t epochMillis = (uint32_t)GPSHandler.getMilliSecond();
        vTaskDelay((1000 - epochMillis) / portTICK_RATE_MS); // set next entire second
        if (diffTime.getEpochTime() > (2 * oneMinute))
        {
          lastGpsTime = GPSHandler.getTimestamp();

          MessageTime_t gps_msg;
          gps_msg.type = src_type_t::GPS;
          gps_msg.epoch = GPSHandler.getTimestamp().getEpochTime() + 2; // ??? why 2
          gps_msg.coordinate = GPSHandler.getCoordinate();

          Serial.printf("GPS: send data from device\n");
          if (xQueueSend(*ptr2queueSource, (void *)&gps_msg, 0) != pdTRUE)
          {
            Serial.println("ERROR: Could not put GPS time to queue.");
          }
        }
      }
      next = true;
    }
  }

  vTaskDelete(nullptr);
}

//=============================================================================================================
