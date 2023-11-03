#ifndef GPSTimeHandler_h
#define  GPSTimeHandler_h

#include <stdint.h>
#include "TimeHandler.h"
#include "Timestamp.h"
#include <TinyGPSPlus.h>

constexpr unsigned char delimeterSizeMap=  20;
constexpr unsigned char bufferSize=  100;

//===================================================================================
class GPSTimeHandler2:public TimeHandler
{
   protected:
      TimeHandler*  ptr2timeHandler;

      Timestamp     GPSTimestamp;
      uint8_t       milliSecond;

      signed char   delimeterMap[  delimeterSizeMap]; // map of delimeter positions
      char          buffer[ bufferSize];  
    

   private:
      void updateTime( Timestamp &timestamp);
      void getField( const unsigned char fieldNo, char* field, const char* text);
      bool isValidRecord( void);
      bool isValidCheckSum( void);
      unsigned char calculateChecksum( void);
      unsigned char countDelimeter( void);
      void fillDelimeterMap( char *buffer);

      
   public:
      GPSTimeHandler2( TimeHandler* ptr2timeHandler);
      virtual ~GPSTimeHandler2( void)= default;

      uint8_t getCentiSecond( void);   
     
      virtual Timestamp&  getTimestamp( void);
      uint8_t getMilliSecond( void);
      virtual const char* getClassName( void);

      bool collectRecord( char c);
      
      virtual bool updateTime( void);

};


//===================================================================================
#endif 