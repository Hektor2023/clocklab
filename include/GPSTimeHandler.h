#ifndef GPSTimeHandler_h
#define  GPSTimeHandler_h

#include <stdint.h>
#include "TimeHandler.h"
#include "Timestamp.h"
#include <TinyGPSPlus.h>

//===================================================================================
class GPSTimeHandler:public TimeHandler
{
   protected:
      TimeHandler*  ptr2timeHandler;
        // The TinyGPSPlus 
      Timestamp     GPSTimestamp;
      TinyGPSPlus   gps;
      
   private:
      void updateTime( Timestamp &timestamp);
   
   public:
      GPSTimeHandler( TimeHandler* ptr2timeHandler= nullptr);
      virtual ~GPSTimeHandler( void)= default;

      bool encode( char c);

     
      virtual Timestamp&  getTimestamp( void);
      virtual const char* getClassName( void);

      virtual void updateTime( void);
};


//===================================================================================
#endif 