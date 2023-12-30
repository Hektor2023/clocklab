#ifndef timestamp_h
#define timestamp_h

#include <cstddef>
#include <string.h>

#include "MyDate.h"
#include "MyTime.h"

//===================================================================================
/* Useful Constants */
#define SECS_PER_MIN  ((uint32_t)(60UL))
#define SECS_PER_HOUR ((uint32_t)(3600UL))
#define SECS_PER_DAY  ((uint32_t)(SECS_PER_HOUR * 24UL))

#define BASE_YEAR  1970
#define BASE_MONTH    1
#define BASE_DAY      1

//===================================================================================
class Timestamp
{
   private:
      uint32_t epoch;
 

   public:
      Timestamp( uint32_t epoch=0L);
      Timestamp( const Timestamp& t);
      ~Timestamp( void);
      
      uint32_t  getEpochTime( void) const;
      void      setEpochTime( const uint32_t newEpochTime);

      int getDayOfWeek( void);

      Timestamp& operator=(const Timestamp& t);
      Timestamp& operator++( void);
      Timestamp  operator++(  int);

      bool operator==( const Timestamp& t);
      bool operator!=( const Timestamp& t);
      bool operator<( const Timestamp& t);
      bool operator<=( const Timestamp& t);
      bool operator>( const Timestamp& t);
      bool operator>=( const Timestamp& t);

      Timestamp operator+( const Timestamp& t);
      Timestamp operator-( const Timestamp& t);
      Timestamp& operator+=( const Timestamp& t);
      Timestamp& operator-=( const Timestamp& t);  
      
      MyDate getDate( void) const;
      void   setDate( MyDate& date);

      MyTime getTime( void) const;
      void   setTime( MyTime& time);

      
      static size_t getStringBufferSize( void);    
      char*  toString( char* Stringbuffer);
      
  private:
      uint32_t sumDaysOfFullYearsSinceBase( uint32_t year) const;
      uint16_t sumDaysOfFullMonths( uint8_t month) const;
};


//===================================================================================

//===================================================================================
#endif
