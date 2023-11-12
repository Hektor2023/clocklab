#ifndef timeviewhandler_h
#define timeviewhandler_h


#include <cstddef>

#include "Timestamp.h"

typedef 
enum
{
    eTime,
    eDate,
}
displayMode_t;

//===================================================================================
class TimeDisplayHandler
{
   protected:
      TimeDisplayHandler*  ptr;
      displayMode_t     displayMode;

   protected:
      void baseUpdateTime( Timestamp &timestamp);

   public:
      TimeDisplayHandler( TimeDisplayHandler* ptr);
      virtual ~TimeDisplayHandler( void)= default;
      
      virtual void updateTime( Timestamp &timestamp);

      virtual const char* getClassName( void)=0;

      void setDisplayMode( displayMode_t mode);
      displayMode_t getDisplayMode( void);  

   private:
      TimeDisplayHandler(const TimeDisplayHandler&) = delete;
      const TimeDisplayHandler& operator=(const TimeDisplayHandler&) = delete;  
};

//===================================================================================
#endif
