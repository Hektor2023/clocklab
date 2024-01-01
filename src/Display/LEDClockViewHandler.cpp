#include "Display/LEDClockViewHandler.h"

//===================================================================================
LEDClockViewHandler::LEDClockViewHandler( TimeDisplayHandler* ptr, MyTime& sunrise, MyTime& sunset,
  const int STB_pin, const int CLK_pin, const int DIO_pin)
  :TimeDisplayHandler( ptr), tm( STB_pin, CLK_pin ,DIO_pin, true), sunrise( sunrise), sunset( sunset) 
{
  pinMode( STB_pin, OUTPUT);    
  pinMode( CLK_pin, OUTPUT); 
  pinMode( DIO_pin, OUTPUT);   

  xSemaphoreTM1638plus = xSemaphoreCreateMutex();
  tm.displayBegin();  
  
  tm.setLEDs( 0);

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
  adjustMode= flagg;
  
}

//===================================================================================
void LEDClockViewHandler::updateTime( Timestamp &timestamp)
{ 
 
  std::string s;
  switch( displayMode)
  {
    case eTime:
      {
          MyTime    requiredTime;
          requiredTime= timestamp.getTime();

          char timeStrBuffer2[ MyTime::getStringBufferSize()];
          s= requiredTime.toString( timeStrBuffer2);
          //  Serial.printf( "->%s\n",s);

          std::replace( s.begin(), s.end(), ':', '-');
      }
      break;

    case eDate:
      {
        MyDate    requiredDate;
        requiredDate= timestamp.getDate();

          char dateStrBuffer2[ MyDate::getStringBufferSize()];
          s= requiredDate.toString( dateStrBuffer2);

          std::replace( s.begin(), s.end(), '/', '.');
      }
      break;

    default:;
  }

  if( xSemaphoreTake( xSemaphoreTM1638plus,( TickType_t ) 0) == pdTRUE)
  {
    tm.displayText( s.c_str());

    //  Serial.printf( "=>%s\n",s);
    tm.setLEDs( 0);
    tm.setLED( timestamp.getDayOfWeek(), 1);
    tm.setLED( 7, adjustMode? 1:0); 
    xSemaphoreGive( xSemaphoreTM1638plus);
  }
  
  
  TimeDisplayHandler::updateTime( timestamp);
}

//===================================================================================
