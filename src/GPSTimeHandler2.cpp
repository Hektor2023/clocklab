
#include "GPSTimeHandler2.h"

const signed char endOfValidPosition= -1;
const char delimeter=',';
const char checkSumMarker='*';

//===================================================================================
GPSTimeHandler2::GPSTimeHandler2( TimeHandler* ptr2timeHandler):
    TimeHandler( ptr2timeHandler), GPSTimestamp( 0L), milliSecond( 0) {}

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
void GPSTimeHandler2::fillDelimeterMap( char *buffer)
{
    // clear map
    const size_t delimeterSize=  sizeof( delimeterMap);
    memset( delimeterMap, endOfValidPosition, delimeterSize);

    int j=0;
    for( int i=0; i< (int)strlen(buffer)&& j<(int)delimeterSize; i++)
    {
        if( buffer[ i]== delimeter)
        {
            delimeterMap[ j++]= i;
        }

    }

}

//=============================================================================================================
unsigned char GPSTimeHandler2::countDelimeter( void)
{
    const size_t delimeterSize=  sizeof( delimeterMap);
    unsigned char counter=0;

    for( int i=0; i< (int)delimeterSize; i++)
    {
        if(  delimeterMap[ i]== endOfValidPosition)
        {
            break;
        }

        counter++;
    }

    return( counter);
}

//=============================================================================================================
bool GPSTimeHandler2::collectRecord( char c)
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
            if( idx< sizeof( buffer))  buffer[ idx++]= c;
            break;
      }

  return( next);
}

//=============================================================================================================
void GPSTimeHandler2::getField( const unsigned char fieldNo, char* field, const char* buffer)
{
    switch( fieldNo)
    {
        case 0:
            if( delimeterMap[ fieldNo] !=0)
            {
                size_t len= delimeterMap[ fieldNo] -1;
                strncpy( field, buffer+0, len);
                field[ len]='\0';
            }
            else
            {
                strcpy( field, " ");
            }
            break;

        default:
            unsigned char delimeterQty= countDelimeter();
//            printf("\n%d - %d\n", delimeterMap[ fieldNo-1],delimeterMap[ fieldNo]);

            if( fieldNo>= delimeterQty) // last field or greater
            {

                if(( fieldNo== delimeterQty) &&( (delimeterMap[ fieldNo-1] +1) != delimeterMap[ fieldNo]))
                {
                    size_t len= strlen(buffer) -(delimeterMap[ fieldNo-1]+1);
                    strncpy( field, buffer+ delimeterMap[ fieldNo-1]+1, len);
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
                    strncpy( field, buffer+ delimeterMap[ fieldNo-1]+1, len);
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
unsigned char  GPSTimeHandler2::calculateChecksum( void)
{
    
    unsigned char chkSum= 0;
    for( int i=0; i< (int)strlen( buffer) && buffer[ i]!= checkSumMarker; i++)
    {
        chkSum^= buffer[ i];
    }

   return( chkSum);
}

//=============================================================================================================
bool GPSTimeHandler2::isValidRecord( void)
{
    constexpr char validRecordchar= 'N';

    char*Ptr2chkSumMarker= strchr( buffer, checkSumMarker);
    assert( (Ptr2chkSumMarker!= NULL) &&"Missing checksum marker");

    return( *(Ptr2chkSumMarker-1)!= validRecordchar);
}

//=============================================================================================================
bool GPSTimeHandler2::isValidCheckSum( void)
{
    char*Ptr2chkSumMarker= strchr( buffer, checkSumMarker);
    assert( (Ptr2chkSumMarker!= NULL) &&"Missing checksum marker");

    if(  !isValidRecord())
    {
      return( false);
    }

    char chkSumToTestTxt[ 3];
    strcpy( chkSumToTestTxt, Ptr2chkSumMarker+1);

    int chkSum= calculateChecksum();
    char chkSumTxt[ 3];

    itoa( chkSum,chkSumTxt,16);
    chkSumTxt[ 0]= toupper( chkSumTxt[ 0]);
    chkSumTxt[ 1]= toupper( chkSumTxt[ 1]);
    chkSumTxt[ 2]='\0';

    return( strcmp( chkSumToTestTxt, chkSumTxt) ==0);
}

//===================================================================================
bool GPSTimeHandler2::updateTime( void)
{
    bool updated= false;

    if( strstr( buffer, "GPRMC") && isValidCheckSum())
    {
//        Serial.printf("| GPS... |");
//        Serial.printf( "GPS: %s\n", buffer);
        fillDelimeterMap( buffer);

        char field[20]; 
        unsigned char fieldNo;

        fieldNo= 1;
        getField( fieldNo, field, buffer);
        float timeAsFloatNumber= atoi( field);
//        Serial.printf( "Time: '%s' %6.2f ", field, timeAsFloatNumber);

        fieldNo= 3;
        getField( fieldNo, field, buffer);
//        latitude= (double)atof( field)/(double)100.0f;
//        Serial.printf( "| Loc: '%s' %f", field, latitude);

        fieldNo= 5;
        getField( fieldNo, field, buffer);
//        longitude= (double)atof( field)/(double)100.0f;
//        Serial.printf( "/'%s' %f", field, longitude);

        fieldNo= 9;
        getField( fieldNo, field, buffer);
        unsigned int dateAsNumber= atoi( field);
//        Serial.printf( "| Date: '%s' %d\n", field, dateAsNumber);

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
        updated= true;
        
    }

    if( updated) 
    {
        baseUpdateTime( GPSTimestamp);
    } 
    return( updated);
}

//===================================================================================
void GPSTimeHandler2::updateTime( Timestamp &timestamp)
{

}

//===================================================================================
