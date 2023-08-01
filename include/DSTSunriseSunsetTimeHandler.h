#ifndef DSTSunriseSunsetTimeHandler_h
#define  DSTSunriseSunsetTimeHandler_h

#include <stdint.h>
#include "Timehandler.h"
#include "Timestamp.h"

constexpr  auto RadToGrad= 180.0/PI;
constexpr  auto GradToRad= PI/180.0;

//===================================================================================
class DSTSunriseSunsetTimeHandler:public TimeHandler
{
   private:
      Timestamp        dSTStartTimestamp;
      Timestamp        dSTEndTimestamp;
      Timestamp        localTimestamp;

      MyTime &sunriseTime;
      MyTime &sunsetTime;

      double &longitude; 
      double &latitude;
      const uint16_t    standardTimeOffset;


    public:
       DSTSunriseSunsetTimeHandler( TimeHandler* ptr, uint16_t standardTimeOffset, \
                                      double& longitude, double& latitude, MyTime& sunrise, MyTime& sunset);
       virtual ~DSTSunriseSunsetTimeHandler( void);
      
      virtual void updateTime( Timestamp &timestamp);
      virtual const char* getClassName( void);

    private:  
      void calculateDST( MyDate &date);
      void calculateSunriseSunset(Timestamp &timestamp);
      uint16_t getOffset( Timestamp& current);

      long modd( double a, double b);
};

//===================================================================================


#endif