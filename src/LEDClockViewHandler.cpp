#include "LEDClockViewHandler.h"


//===================================================================================
LEDClockViewHandler::LEDClockViewHandler( TimeHandler* ptr, 
  const int STB_pin, const int CLK_pin, const int DIO_pin)
  :TimeHandler( ptr), tm( STB_pin, CLK_pin ,DIO_pin, true) 
{
  pinMode( STB_pin, OUTPUT);    
  pinMode( CLK_pin, OUTPUT); 
  pinMode( DIO_pin, OUTPUT);   

  xSemaphoreTM1638plus = xSemaphoreCreateMutex();
  tm.displayBegin();  
  
  tm.setLEDs(0x0000);
};

//===================================================================================
LEDClockViewHandler::~LEDClockViewHandler( void) {};

//===================================================================================
const char* LEDClockViewHandler::getClassName( void)
{
  return("LEDClockViewHandler");
}

//===================================================================================
void LEDClockViewHandler::doAction( void (*fun)( void))
{
  if( fun!= nullptr)
  {
    (*fun)();
  }

}

//===================================================================================
 uint8_t LEDClockViewHandler::buttonsRead( void)
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
void LEDClockViewHandler::modeAdjust( bool flagg)
{
  tm.setLED( 7, flagg? 1:0);  
}

//===================================================================================
void LEDClockViewHandler::updateTime( Timestamp &timestamp)
{ 
 
  MyTime    requiredTime;
  
  requiredTime= timestamp.getTime();
  

  char timeStrBuffer2[ MyTime::getStringBufferSize()];
  std::string s= requiredTime.toString( timeStrBuffer2);
  //  Serial.printf( "->%s\n",s);

  std::replace( s.begin(), s.end(), ':', '-');
  if( xSemaphoreTake( xSemaphoreTM1638plus,( TickType_t ) 0) == pdTRUE)
  {
    tm.displayText( s.c_str());

    //  Serial.printf( "=>%s\n",s);
    tm.setLED( timestamp.getDayOfWeek(), 1);
    xSemaphoreGive( xSemaphoreTM1638plus);
  }
  
  
  TimeHandler::updateTime( timestamp);
}

//===================================================================================
