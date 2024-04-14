#include "Display/LEDClockDisplay/LEDClockDisplayHandler.h"

//===================================================================================
LEDClockDisplayHandler::LEDClockDisplayHandler( const int STB_pin, const int CLK_pin, const int DIO_pin)
 :tm( STB_pin, CLK_pin ,DIO_pin, true)
{
  pinMode( STB_pin, OUTPUT);    
  pinMode( CLK_pin, OUTPUT); 
  pinMode( DIO_pin, OUTPUT);   

  xSemaphoreTM1638plus = xSemaphoreCreateMutex();
  tm.displayBegin();  
  
  tm.setLEDs( 0);

};

//===================================================================================
 uint8_t LEDClockDisplayHandler::buttonsRead( void)
 {
    static uint8_t lastKeys= -1;
    uint8_t keys;

    if( xSemaphoreTake( xSemaphoreTM1638plus,( TickType_t ) 0) == pdTRUE)
    {
      keys= tm.readButtons();
      xSemaphoreGive( xSemaphoreTM1638plus);

      if( keys!= lastKeys)
      {
        lastKeys= keys;
        return( keys);
      } 
      
    }

    return( 0);  
 }

//===================================================================================
void LEDClockDisplayHandler::update( const TimeData &data)
{ 
  std::string s;

  MyTime requiredTime = data.localTime;
  MyDate requiredDate = data.localDate;

  char timeStrBuffer[ MyTime::getStringBufferSize()];
  s= requiredTime.toString( timeStrBuffer);
  std::replace( s.begin(), s.end(), ':', '-');

  if( xSemaphoreTake( xSemaphoreTM1638plus,( TickType_t ) 0) == pdTRUE)
  {
    tm.displayText( s.c_str());

    //  Serial.printf( "=>%s\n",s);
    tm.setLEDs( 0);
    tm.setLED(requiredDate.getDayOfWeek(), 1);
//    tm.setLED( 7, adjustMode? 1:0); 
    xSemaphoreGive( xSemaphoreTM1638plus);
  }
  
}

//===================================================================================
