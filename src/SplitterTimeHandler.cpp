#include "SplitterTimeHandler.h"


//===================================================================================
SplitterTimeHandler::SplitterTimeHandler(  TimeHandler* ptr, Timestamp &duplicatedTimestamp)
  :TimeHandler( ptr), duplicatedTimestamp( duplicatedTimestamp) {}

//===================================================================================
SplitterTimeHandler::~SplitterTimeHandler() {}

//===================================================================================
void SplitterTimeHandler::updateTime( Timestamp &timestamp)
{
  duplicatedTimestamp.setEpochTime( timestamp.getEpochTime());
  baseUpdateTime( timestamp);  
}

//===================================================================================
const char* SplitterTimeHandler::getClassName( void)
{
  return( "SplitterTimeHandler");
}

//===================================================================================
