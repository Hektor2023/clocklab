#include <cstddef>

#include <Arduino.h>
#include <stdint.h>
#include <string>


#include "Build_defs.h"
#include "Timestamp.h"
#include "TimeHandler.h"


#include "ConsoleViewHandler.h"
#include "AnalogClockViewHandler.h"
#include "LedClockViewHandler.h"
#include "OLEDDisplayClockViewHandler.h"
#include "TimeSunriseSunsetHandler.h"
#include "TimeZoneDSTHandler.h"
#include "SystemTimeHandler.h"
#include "Controller.h"  

#include "NTPWifiTimeHandler.h"

// pin used to controling Hands of Clock

const uint8_t analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

/*
const uint8_t minL_pin = D8; 
const uint8_t minR_pin = D7;  
const uint8_t secL_pin = D6;      
const uint8_t secR_pin = D5; 
*/

const uint8_t STB_pin = D4;
const uint8_t CLK_pin = D6;
const uint8_t DIO_pin = D7;




const auto  Monitor_Baudrate{ 115200};
const auto  clockhostname{ "AGClock"};
const auto  oneSecond{ 1000}; // [ms]
const auto  update_NTP_interval_in_mSec{ 30*  oneSecond};
const auto  start_epoch_time{ 1654387200};  // 00:00:00 - 05/06/2022

MyTime  sunriseTime, sunsetTime;

// Location Nowy Dworm Mazowiecki
double  longitude= 20.693; 
double  latitude=  52.447;

// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT  0 = 0
const auto   GMT_Plus_1h{ 1* 3600};

//===================================================================================
void showVersion( void)
{
  Timer  pauseTimer( 2* Second); 
  
  while( !pauseTimer.isTimeout()) 
  {
      Serial.print( "#");
      yield();
  };
  
  Serial.printf( "\nVer: %s\n", (char*)&completeVersion);  
}

//===================================================================================

 
//  AnalogClockViewHandler      clockViewHandler(   nullptr, minL_pin, minR_pin, secL_pin, secR_pin);  
  LEDClockViewHandler         clockViewHandler(   nullptr, STB_pin, CLK_pin, DIO_pin);
//  OLEDDisplayClockViewHandler clockViewHandler(   nullptr);
  ConsoleViewHandler          consoleViewHandler( &clockViewHandler);
  TimeSunriseSunsetHandler    sunriseTimeHandler( &consoleViewHandler, longitude, latitude, sunriseTime, sunsetTime);
  TimeZoneDSTHandler          timeZoneDSTHandler( &sunriseTimeHandler, GMT_Plus_1h);
  NTPWiFiTimeHandler          sourceTimeHandler(  &timeZoneDSTHandler, update_NTP_interval_in_mSec);
  SystemTimeHandler           systemTimeHandler(  &sourceTimeHandler);
  
  Controller                  controller( &systemTimeHandler);

//===================================================================================
void setup( void) 
{ 
  Serial.begin( Monitor_Baudrate);
  Serial.flush();
  
  showVersion();
  sourceTimeHandler.init(clockhostname);
}
//===================================================================================
void loop( void) 
{
  std::string   cmd, result;

  do
  {
    if( Serial.available() >0)
    {
      cmd= Serial.readStringUntil('\n').c_str();
      result= controller.execute( cmd.c_str());

      Serial.println( result.c_str());  
    }
    
    yield(); 
  }
  while( systemTimeHandler.isInManualMode()); 

//  uint8_t sensorValue = analogRead( analogInPin);
//  Serial.printf( "sensor= %d\n", sensorValue);
  
  yield();   
  systemTimeHandler.updateTime();
  
}

//===================================================================================
