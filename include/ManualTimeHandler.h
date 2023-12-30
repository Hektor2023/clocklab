#ifndef ManualTimeHandler_h
#define ManualTimeHandler_h


#include <cstddef>
#include <string>
#include "MyDate.h"
#include "Timestamp.h"
#include "TimeHandler.h"
#include "RTCSystemTimeHandler.h"


//===================================================================================
class ManualTimeHandler
{
   private: 
      enum class variable{ Hour, Minute, Second, Day, Month, Year};
      enum class direction{ Plus, Minus};
 
      Timestamp adjTimestamp; 

   public:
      ManualTimeHandler( void);
      ~ManualTimeHandler( void)= default;
      
      const std::string execute( const std::string command);

      Timestamp getTimestamp( void);

   private:
      void adjust( ManualTimeHandler::variable var, ManualTimeHandler::direction dir);   
};

//===================================================================================
#endif // ManualTimeHandler
