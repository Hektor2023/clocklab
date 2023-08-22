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
      uint8_t       centiSecond;

   private:
      void updateTime( Timestamp &timestamp);
   
   public:
      GPSTimeHandler( TimeHandler* ptr2timeHandler= nullptr);
      virtual ~GPSTimeHandler( void)= default;

      bool encode( char c);
      uint8_t getCentiSecond( void);   
     
      virtual Timestamp&  getTimestamp( void);
      virtual const char* getClassName( void);

      virtual void updateTime( void);
};


//===================================================================================
#endif 