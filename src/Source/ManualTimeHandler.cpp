#include "Source/ManualTimeHandler.h"

#include "Arduino.h"

//===================================================================================
ManualTimeHandler::ManualTimeHandler( void)
{};
  

//===================================================================================
Timestamp ManualTimeHandler::getTimestamp( void)
{
  return adjTimestamp;
}

//===================================================================================
void ManualTimeHandler::adjust( ManualTimeHandler::variable var, ManualTimeHandler::direction dir)
{
 
  switch( var)
   {  
      case variable::Hour:
          {
            MyTime my_time;
            
            adjTimestamp.getTime( my_time);      
            (dir== direction::Plus)?  my_time.hourInc(): my_time.hourDec();
            adjTimestamp.setTime( my_time);
          }
          break;

      case variable::Minute:
          {
            MyTime my_time;
            
            adjTimestamp.getTime( my_time);
            (dir== direction::Plus)?  my_time.minuteInc(): my_time.minuteDec();
            adjTimestamp.setTime( my_time);
          }
          break;
          
      case variable::Second:
          {
            MyTime my_time;
            
            adjTimestamp.getTime( my_time);
            (dir== direction::Plus)?  my_time.secondInc(): my_time.secondDec();
            adjTimestamp.setTime( my_time);
          }      
          break;

      case variable::Day:
          {
            MyDate my_date;
            
            adjTimestamp.getDate( my_date);
            (dir== direction::Plus)?  my_date.dayInc(): my_date.dayDec();
            adjTimestamp.setDate( my_date);           
          }
          break;

      case variable::Month:
          {
            MyDate my_date;
            
            adjTimestamp.getDate( my_date);
            (dir== direction::Plus)?  my_date.monthInc(): my_date.monthDec();
            adjTimestamp.setDate( my_date); 
          }
          break;

      case variable::Year:
          {
            MyDate my_date;
            
            adjTimestamp.getDate( my_date);
            (dir== direction::Plus)?  my_date.yearInc(): my_date.yearDec();
            adjTimestamp.setDate( my_date); 
          }
          break;

      default: 
          return;
    };
   
 
}

//===================================================================================
const std::string ManualTimeHandler::execute( const std::string command) 
{
  static ManualTimeHandler::variable  var;

  if( command.find("hour")==0)
  {
    var= ManualTimeHandler::variable::Hour;   
    return("Adjust hour");      
  } 

  if( command.find("minute")==0)
  {
    var= ManualTimeHandler::variable::Minute;
    return("Adjust minute"); 
  } 
        
  if( command.find("second")==0)
  {
    var= ManualTimeHandler::variable::Second;
    return("Adjust second"); 
  } 

  if( command.find("day")==0)
  {
    var= ManualTimeHandler::variable::Day;
    return("Adjust day");    
  }
         
  if( command.find("month")==0)
  {
    var=ManualTimeHandler::variable::Month;
    return("Adjust month");    
  } 
        
  if( command.find("year")==0)
  {
    var= ManualTimeHandler::variable::Year;
    return("Adjust year");    
  }    

  if( command.find("+")==0)
  {
    adjust( var, ManualTimeHandler::direction::Plus);
    return("Adjusted");   
  }
  else  
  if( command.find("-")==0)
  {
    adjust( var, ManualTimeHandler::direction::Minus);
    return("Adjusted"); 
  }
  

  return( "Error");           
}; 

//===================================================================================
