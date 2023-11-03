#include "TimeHandler.h"

//===================================================================================
TimeHandler::TimeHandler( TimeHandler* ptr):ptr( ptr) {}

//===================================================================================
void TimeHandler::baseUpdateTime( Timestamp &timestamp)
{    
  if( ptr)
  {
    
    ptr->updateTime( timestamp);
 //   Serial.println( ptr->getClassName());
  }
            
}
       
//===================================================================================
void TimeHandler::updateTime( Timestamp &timestamp)
{            
  baseUpdateTime( timestamp);
}
       
//===================================================================================
