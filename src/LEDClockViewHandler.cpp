#include "LEDClockViewHandler.h"

//===================================================================================
LEDClockViewHandler::LEDClockViewHandler( TimeHandler* ptr, 
  const int STB_pin, const int CLK_pin, const int DIO_pin)
  :TimeHandler( ptr), tm( STB_pin, CLK_pin ,DIO_pin, false) 
{
  pinMode( STB_pin, OUTPUT);    
  pinMode( CLK_pin, OUTPUT); 
  pinMode( DIO_pin, OUTPUT);   

  tm.displayBegin();  
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
void LEDClockViewHandler::updateTime( Timestamp &timestamp)
{ 
 
  MyTime    requiredTime;
  
  requiredTime= timestamp.getTime();
  

  char timeStrBuffer2[ MyTime::getStringBufferSize()];
  std::string s= requiredTime.toString( timeStrBuffer2);
  //  Serial.printf( "->%s\n",s);

  std::replace( s.begin(), s.end(), ':', '-');
  tm.displayText( s.c_str());

  //  Serial.printf( "=>%s\n",s);

  tm.setLEDs(0x0000);
  tm.setLED( timestamp.getDayOfWeek(), 1);
  
  TimeHandler::updateTime( timestamp);
}

//===================================================================================
