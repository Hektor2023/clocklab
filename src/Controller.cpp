#include "Controller.h"

#include "Arduino.h"

//===================================================================================
Controller::Controller( RTCSystemTimeHandler* systemTimeHandler)
  :systemTimeHandler( *systemTimeHandler) {};
  
//===================================================================================
void Controller::adjust( Controller::variable var, Controller::direction dir)
{
  Timestamp& systemTimestamp= systemTimeHandler.getTimestamp();

  switch( var)
   {  
      case variable::Hour:
          {
            MyTime my_time( systemTimestamp.getTime());      
            (dir== direction::Plus)?  my_time.hourInc(): my_time.hourDec();
            systemTimestamp.setTime( my_time);
            
          }
          break;

      case variable::Minute:
          {
            MyTime my_time( systemTimestamp.getTime());
            (dir== direction::Plus)?  my_time.minuteInc(): my_time.minuteDec();
            systemTimestamp.setTime( my_time);
          }
          break;
          
      case variable::Second:
          {
            MyTime my_time( systemTimestamp.getTime());
            (dir== direction::Plus)?  my_time.secondInc(): my_time.secondDec();
            systemTimestamp.setTime( my_time);
          }      
          break;

      case variable::Day:
          {
            MyDate my_date( systemTimestamp.getDate());
            (dir== direction::Plus)?  my_date.dayInc(): my_date.dayDec();
            systemTimestamp.setDate( my_date);           
          }
          break;

      case variable::Month:
          {
            MyDate my_date( systemTimestamp.getDate());
            (dir== direction::Plus)?  my_date.monthInc(): my_date.monthDec();
            systemTimestamp.setDate( my_date); 
          }
          break;

      case variable::Year:
          {
            MyDate my_date( systemTimestamp.getDate());
            (dir== direction::Plus)?  my_date.yearInc(): my_date.yearDec();
            systemTimestamp.setDate( my_date); 
          }
          break;

      default: 
          break;
    };
   
  systemTimeHandler.setTimestamp( systemTimestamp);
  systemTimeHandler.forceUpdateTime();
}

//===================================================================================
const std::string Controller::execute( const std::string command) 
{
  
  if(( command.find("stop") ==0) ||( command.find("enter") ==0))
  {
//    systemTimeHandler.manualMode( true);
    return("Enter manual mode");
  }
    
  if(( command.find("start") ==0) ||( command.find("exit") ==0))
  {
 //   systemTimeHandler.manualMode( false);
    return("Exit from manual mode");
  }

  
  static Controller::variable  var;
  if( command.find("hour")==0)
  {
    var= Controller::variable::Hour;   
    return("Adjust hour");      
  } 

  if( command.find("minute")==0)
  {
    var= Controller::variable::Minute;
    return("Adjust minute"); 
  } 
        
  if( command.find("second")==0)
  {
    var= Controller::variable::Second;
    return("Adjust second"); 
  } 

  if( command.find("day")==0)
  {
    var= Controller::variable::Day;
    return("Adjust day");    
  }
         
  if( command.find("month")==0)
  {
    var=Controller::variable::Month;
    return("Adjust month");    
  } 
        
  if( command.find("year")==0)
  {
    var= Controller::variable::Year;
    return("Adjust year");    
  }    

  if( command.find("+")==0)
  {
    adjust( var, Controller::direction::Plus);
    return("Adjusted");   
  }
  else  
  if( command.find("-")==0)
  {
    adjust( var, Controller::direction::Minus);
    return("Adjusted"); 
  }
  

  return( "Error");           
}; 

//===================================================================================
