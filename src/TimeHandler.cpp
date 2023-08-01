#include "TimeHandler.h"

//===================================================================================
TimeHandler::TimeHandler( TimeHandler* ptr):ptr( ptr) {}

//===================================================================================
//TimeHandler::~TimeHandler( void) {}

//===================================================================================
/*
void TimeHandler::setValidTime( bool validFlagg) 
{

  this->validTime= validFlagg;
  if( ptr)
  {  
//    Serial.printf("Class: %s\n", getClassName()), 
    ptr->setValidTime( validFlagg);
  }    
}
*/
//===================================================================================
/*
bool TimeHandler::isValidTime( void) 
{
  if( ptr)
  {  
    return( this->validTime);
  }

  return( false);
}
*/
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
