#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>

#include <Source/NTP/NTPClient.h> // https://github.com/arduino-libraries/NTPClient

#include "TimeType/Timestamp.h"
#include "clocklab_types.h"
#include "Other/Tools.h"
#include "WifiCred.h"

//===================================================================================
typedef struct
{
  const char *gc_Ssid;
  const char *gc_Password;
} t_credentials;

//=============================================================================================================
class NTPSourceTime
{
private:
  WiFiUDP udp;
  NTPClient timeClient;
  Timestamp timestamp;
  t_credentials credentials;

public:
  NTPSourceTime(t_credentials &credentials);
  ~NTPSourceTime(void) = default;

  Timestamp &getTimestamp(void);
  bool update(void);
};

//=============================================================================================================
