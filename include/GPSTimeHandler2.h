#ifndef GPSTimeHandler_h
#define  GPSTimeHandler_h

#include <stdint.h>
#include "TimeHandler.h"
#include "Timestamp.h"
#include <TinyGPSPlus.h>

//===================================================================================
class GPSTimeHandler2:public TimeHandler
{
   protected:
      TimeHandler*  ptr2timeHandler;
        // The TinyGPSPlus 
      Timestamp     GPSTimestamp;
      
      uint8_t       milliSecond;

   private:
      void updateTime( Timestamp &timestamp);
      void getField( const unsigned char fieldNo, char* field, const char* text, const char delimeter, const signed char* delimeterMap, const unsigned int delimeterMapSize);
      bool isValidRecord( const char* txt);
      bool isValidCheckSum( const char* txt);
      unsigned char calculateChecksum( const char* txt);
      unsigned char countDelimeter( const signed char* delMap, const unsigned int delMapSize, const char delimeter);
      void fillDelimeterMap( char *text, signed char* delMap, const unsigned int delMapSize, const char delimeter);

   public:
      GPSTimeHandler2( TimeHandler* ptr2timeHandler= nullptr);
      virtual ~GPSTimeHandler2( void)= default;

      uint8_t getCentiSecond( void);   
     
      virtual Timestamp&  getTimestamp( void);
      uint8_t getMilliSecond( void);
      virtual const char* getClassName( void);

      bool collectRecord( char c, char* buffer, uint8_t bufferSize);
      void updateTime( char* buffer);

      virtual void updateTime( void);
};


//===================================================================================
#endif 