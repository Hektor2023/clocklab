#ifndef controller_h
#define controller_h


#include <cstddef>
#include <string>
#include "MyDate.h"
#include "Timestamp.h"
#include "TimeHandler.h"
#include "RTCSystemTimeHandler.h"


//===================================================================================
class Controller
{
  
   private: 
      enum class variable{ Hour, Minute, Second, Day, Month, Year};
      enum class direction{ Plus, Minus};
 
      RTCSystemTimeHandler&  systemTimeHandler;

   private:
      void adjust( Controller::variable var, Controller::direction dir);

   public:
      Controller( RTCSystemTimeHandler* systemTimeHandler);
      ~Controller( void)= default;
      
      const std::string execute( const std::string command);
};

//===================================================================================
#endif
