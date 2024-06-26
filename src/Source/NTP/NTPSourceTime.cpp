#include "Source/NTP/NTPSourceTime.h"

//===================================================================================
NTPSourceTime::NTPSourceTime(t_credentials &credentials)
    : timeClient(udp), credentials(credentials)
{
  WiFi.mode(WIFI_STA);
};

//===================================================================================
bool NTPSourceTime::update(void)
{
  Serial.printf("Connecting...");
  WiFi.begin(&credentials.gc_Ssid[0], &credentials.gc_Password[0]);

  while (!WiFi.isConnected())
  {
    Serial.printf("#");
    vTaskDelay(3 * (1000 / portTICK_RATE_MS));
  };

  timeClient.begin();
  if (!timeClient.forceUpdate())
  {
    timeClient.end();
    WiFi.disconnect();
    return false;
  }

  uint32_t epoch = timeClient.getEpochTime();
  //  Serial.printf("epoch ->%u\n", epoch);

  uint32_t epochMillis = (uint32_t)((1000.0f * timeClient.getMillis()) / UINT32_MAX);
  //  Serial.printf("millis => %u\n", epochMillis);

  // set next entire second
  vTaskDelay((1000 - epochMillis) / portTICK_RATE_MS);
  epoch++;

  timeClient.end();
  //  Serial.printf("\nDisconnect...\n");
  WiFi.disconnect();

  timestamp.setEpochTime(epoch);
  return true;
}

//===================================================================================
Timestamp &NTPSourceTime::getTimestamp(void)
{
  return timestamp;
}

//===================================================================================