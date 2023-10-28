
#include "GPSTimeHandler2.h"

const signed char endOfValidPosition= -1;
const char delimeter=',';
const char checkSumMarker='*';

//===================================================================================
GPSTimeHandler2::GPSTimeHandler2( TimeHandler* ptr2timeHandler):TimeHandler( ptr2timeHandler), GPSTimestamp( 0L), milliSecond( 0) {}

//===================================================================================
Timestamp& GPSTimeHandler2::getTimestamp( void)
{
  return( GPSTimestamp);
}

//===================================================================================
uint8_t GPSTimeHandler2::getMilliSecond( void)
{
    return( milliSecond);
}
 //===================================================================================
const char* GPSTimeHandler2::getClassName( void)
{
  return( "GPSTimeHandler2");
}

//=============================================================================================================
void GPSTimeHandler2::fillDelimeterMap( char *text, signed char* delMap, const unsigned int delMapSize, const char delimeter)
{
    // clear map
    memset( delMap, endOfValidPosition, delMapSize);

    int j=0;
    for( int i=0; i< (int)strlen(text)&& j<(int)delMapSize; i++)
    {
        if( text[ i]== delimeter)
        {
            delMap[ j++]= i;
        }

    }

}

//=============================================================================================================
unsigned char GPSTimeHandler2::countDelimeter( const signed char* delMap, const unsigned int delMapSize, const char delimeter)
{
    unsigned char counter=0;

    for( int i=0; i< (int)delMapSize; i++)
    {
        if( delMap[ i]== endOfValidPosition)
        {
            break;
        }

        counter++;
    }

    return( counter);
}

//=============================================================================================================
bool GPSTimeHandler2::collectRecord( char c, char* buffer, uint8_t bufferSize)
{
  bool next= true;
  static uint8_t idx= 0;

  switch( c)
      {
        case '$':
            idx=0;
            break;

        case '\n':
            break;

        case '\r':
            next= false;
            buffer[ idx]='\0';
            break;

        default:
            if( idx< bufferSize)  buffer[ idx++]= c;
            break;
      }

  return( next);
}

//=============================================================================================================
void GPSTimeHandler2::getField( const unsigned char fieldNo, char* field, const char* text, const char delimeter, const signed char* delimeterMap, const unsigned int delimeterMapSize)
{
    switch( fieldNo)
    {
        case 0:
            if( delimeterMap[ fieldNo] !=0)
            {
                size_t len= delimeterMap[ fieldNo] -1;
                strncpy( field, text+0, len);
                field[ len]='\0';
            }
            else
            {
                strcpy( field, " ");
            }
            break;

        default:
            unsigned char delimeterQty= countDelimeter(delimeterMap, delimeterMapSize, delimeter);
//            printf("\n%d - %d\n", delimeterMap[ fieldNo-1],delimeterMap[ fieldNo]);

            if( fieldNo>= delimeterQty) // last field or greater
            {

                if(( fieldNo== delimeterQty) &&( (delimeterMap[ fieldNo-1] +1) != delimeterMap[ fieldNo]))
                {
                    size_t len= strlen(text) -(delimeterMap[ fieldNo-1]+1);
                    strncpy( field, text+ delimeterMap[ fieldNo-1]+1, len);
                    field[ len]='\0';
                }
                else
                {
                    strcpy( field, " ");
                }
            }
            else
            {
                if((delimeterMap[ fieldNo-1] +1) != delimeterMap[ fieldNo])
                {
                    size_t len= delimeterMap[ fieldNo] -(delimeterMap[ fieldNo-1]+1);
                    strncpy( field, text+ delimeterMap[ fieldNo-1]+1, len);
                    field[ len]='\0';
                }
                else
                {
                    strcpy( field, " ");
                }
            }
            break;
    }

}

//=============================================================================================================
unsigned char  GPSTimeHandler2::calculateChecksum( const char* txt)
{
    
    unsigned char chkSum= 0;
    for( int i=0; i< (int)strlen( txt) && txt[ i]!= checkSumMarker; i++)
    {
        chkSum^= txt[ i];
    }

   return( chkSum);
}

//=============================================================================================================
bool GPSTimeHandler2::isValidRecord( const char* txt)
{
    const char validRecordchar= 'N';

    char*Ptr2chkSumMarker= strchr( txt, checkSumMarker);
    assert( (Ptr2chkSumMarker!= NULL) &&"Missing checksum marker");

    return( *(Ptr2chkSumMarker-1)!= validRecordchar);
}

//=============================================================================================================
bool GPSTimeHandler2::isValidCheckSum( const char* txt)
{
    char*Ptr2chkSumMarker= strchr( txt, checkSumMarker);
    assert( (Ptr2chkSumMarker!= NULL) &&"Missing checksum marker");

    if(  !isValidRecord( txt))
    {
      return( false);
    }

    char chkSumToTestTxt[ 3];
    strcpy( chkSumToTestTxt, Ptr2chkSumMarker+1);

    int chkSum= calculateChecksum( txt);
    char chkSumTxt[ 3];

    itoa( chkSum,chkSumTxt,16);
    chkSumTxt[ 0]= toupper( chkSumTxt[ 0]);
    chkSumTxt[ 1]= toupper( chkSumTxt[ 1]);
    chkSumTxt[ 2]='\0';

    return( strcmp( chkSumToTestTxt, chkSumTxt) ==0);
}

//===================================================================================
void GPSTimeHandler2::updateTime( char* buffer)
{
    if( strstr( buffer, "GPRMC") && isValidCheckSum( buffer))
    {
        Serial.printf("| GPS... |");
        Serial.printf( "GPS: %s\n", buffer);

        signed char  delimeterMap[20]; // map of delimeter positions
        fillDelimeterMap( buffer, delimeterMap, sizeof( delimeterMap), delimeter);

        char field[20]; 
        unsigned char fieldNo;

        fieldNo= 1;
        getField( fieldNo, field, buffer, delimeter, delimeterMap, sizeof( delimeterMap));
        float timeAsFloatNumber= atoi( field);
//        Serial.printf( "Time: '%s' %6.2f ", field, timeAsFloatNumber);

        fieldNo= 3;
        getField( fieldNo, field, buffer, delimeter, delimeterMap, sizeof( delimeterMap));
//        latitude= (double)atof( field)/(double)100.0f;
//        Serial.printf( "| Loc: '%s' %f", field, latitude);

        fieldNo= 5;
        getField( fieldNo, field, buffer, delimeter, delimeterMap, sizeof( delimeterMap));
//        longitude= (double)atof( field)/(double)100.0f;
//        Serial.printf( "/'%s' %f", field, longitude);

        fieldNo= 9;
        getField( fieldNo, field, buffer, delimeter, delimeterMap, sizeof( delimeterMap));
        unsigned int dateAsNumber= atoi( field);
//        Serial.printf( "| Date: '%s' %d\n", field, dateAsNumber);

        buffer[ 0]='\0';
      
        unsigned int timeAsNumber= (unsigned int)timeAsFloatNumber;

        this->milliSecond= (uint8_t) (timeAsFloatNumber- (float)timeAsNumber)* 100.0f;
        unsigned int second= timeAsNumber %100;
        timeAsNumber /=100;
        unsigned int minute= timeAsNumber %100;
        timeAsNumber /=100;
        unsigned int hour=   timeAsNumber %100;
//        Serial.printf( "\nTimeAsString= %00d.%00d.%00d.%00d  \n",hour, minute, second, milliSecond);

        MyTime time;
        time.setHour( hour);
        time.setMinute( minute);
        time.setSecond( second);
//        char timeAsString[  time.getStringBufferSize()];
//        Serial.printf( "\nTimeAsString= %s  \n",time.toString( timeAsString ));

        unsigned int year= 2000+ dateAsNumber %100; 
        dateAsNumber /=100;
        unsigned char month= dateAsNumber %100;
        dateAsNumber /=100;
        unsigned char day= dateAsNumber %100;

        MyDate date;
        date.setYear(  year);
        date.setMonth( month);
        date.setDay(   day);
//        char dateAsString[  date.getStringBufferSize()];
//        Serial.printf( "\nDateAsString= %s  \n",date.toString( dateAsString ));

        GPSTimestamp.setDate( date);
        GPSTimestamp.setTime( time);
//        char timestampAsString[  GPSTimestamp.getStringBufferSize()];
//        Serial.printf( "\nTimestampAsString= %s  \n",GPSTimestamp.toString( timestampAsString ));

//        Serial.printf( "GPS: encoded\n");  
    }

    baseUpdateTime( GPSTimestamp);
}

//===================================================================================
void GPSTimeHandler2::updateTime( Timestamp &timestamp)
{

}

//===================================================================================
