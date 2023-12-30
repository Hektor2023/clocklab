#include "Tools.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//=============================================================================================================
void printTick(void)
{
  Serial.print( xTaskGetTickCount());
  Serial.print( ":\t");
}

//=============================================================================================================
void displayTimestamp( const char *srcName, Timestamp timestamp)
{
  char timestampAsString[ timestamp.getStringBufferSize()];

  Serial.printf( "\n%s Timestamp= %s\n",srcName, timestamp.toString( timestampAsString ));
} 

//=============================================================================================================
