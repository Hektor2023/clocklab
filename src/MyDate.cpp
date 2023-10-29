#include "MyDate.h"

#include <cstddef>

//===================================================================================
MyDate::MyDate( void):day( BASE_DAY), month( BASE_MONTH), year( BASE_YEAR) {}

//===================================================================================
MyDate::MyDate( uint8_t  day, uint8_t  month, uint16_t year):day( day), month( month), year( year) {}

//===================================================================================
MyDate::MyDate( const MyDate& date):day( date.day), month( date.month), year( date.year) {}

//===================================================================================
MyDate::~MyDate( void) {};

//===================================================================================
uint8_t MyDate::getDay( void) const 
{
  return( day);    
}

//===================================================================================
void MyDate::setDay(  const uint8_t day)
{
   this->day= day;  
}


//===================================================================================
uint8_t MyDate::getMonth( void) const 
{
  return( month);    
}

//===================================================================================
void MyDate::setMonth(  const uint8_t month)
{
  this->month= month;  
}
    
//===================================================================================
uint16_t MyDate::getYear( void) const 
{
  return( year);    
}    

//===================================================================================
void MyDate::setYear(  const uint16_t year)
{
  this->year= year;  
}

//===================================================================================
void MyDate::dayInc( void)
{
  day =( day< 31)? day+1:1; 
}

//===================================================================================
void MyDate::dayDec( void)
{
  day =( day> 1)? day-1:31; 
}

//===================================================================================
void MyDate::monthInc( void)
{
  month =( month< 12)? month+1:1;   
}

//===================================================================================
void MyDate::monthDec( void)
{
  month =( month> 1)? month-1:12; 
}

//===================================================================================
void MyDate::yearInc( void)
{
  year =year+1;  
}

//===================================================================================
void MyDate::yearDec( void)
{
  year =( year> BASE_YEAR)? year-1:BASE_YEAR;  
}
    
//===================================================================================
uint8_t MyDate::getYearCode( void) const
{
    uint16_t y= year% 100;

    return( (y + (y / 4)));
}

//===================================================================================
inline uint8_t MyDate::getDayCode( void) const
{
    return( day);
}

//===================================================================================
uint8_t MyDate::getMonthCode( void) const
{
  //                J  F  M    A  M  J    J  A  S    O  N  D
    uint8_t code[]= {1, 4, 4,   0, 2, 5,   0, 3, 6,   1, 4, 6};

    return( code[ month-1]);
}

//===================================================================================
uint8_t MyDate::getLeapCode( void) const
{
    uint8_t  result=0;

    if( IS_LEAP_YEAR( year) && ( month <3))
    {
        result= 1;
    }

    return( result);
}

//===================================================================================
uint8_t MyDate::getCenturyCode( void) const
{   // Gegorian Calendar
    //                    1700  1800  1900  2000  2100  2200 2300
    uint8_t idx, code[]={    4,    2,    0,    6,    4,    2,   0};
    idx= (year/100) -17;
    
    return( code[ idx]);
}

//===================================================================================
uint8_t MyDate::getDayOfWeek() const
{
    constexpr uint8_t offsetForFirstDayInMonday= 5;
    //            #1 #2  #8            #3         #4                   #5                   #6
    uint8_t code= getYearCode()+ getDayCode()+ getMonthCode()- getLeapCode()+  getCenturyCode()+offsetForFirstDayInMonday;
    return( code %7);
}

//===================================================================================
bool MyDate::isValidYear( uint16_t year)
{
  return( year >= 1970);
}

//===================================================================================
bool MyDate::isValidMonth( uint8_t month)
{
  return( month <= 13);
}
  
//===================================================================================
bool MyDate::isValidDay( uint8_t day, uint8_t month, uint16_t year )
{
   //                      1   2   3   4   5   6   7   8   9   10  11  12
   const uint8_t dofm[]={ 31, 28, 31, 30, 31, 30, 31, 31, 30,  31, 30, 31};

  return( day<= dofm[ month -1]+ ( IS_LEAP_YEAR( year)? 1:0));  
}

//===================================================================================
MyDate& MyDate::operator =( const MyDate& date)
{
    if( &date != this )
    {
        day=   date.day;
        month= date.month;
        year=  date.year;
    }
    return( *this);
}

//===================================================================================
MyDate MyDate::getDSTStart( uint16_t year) 
{
    const uint8_t SUNDAY= 6;
    MyDate   date( 31, 03, year);

    auto dofw= date.getDayOfWeek();
    if( dofw != SUNDAY)
    {
      date.setDay( 31- (dofw+1));
    };

    return( date);
}    
    
//===================================================================================
MyDate MyDate::getDSTEnd( uint16_t year)
{
    const uint8_t SUNDAY= 6;
    MyDate   date( 31, 10, year);

    // 31.10.2023  dofw = 1  TUESDAY //  DSTEnd 29.10.23 dofw = 6
    auto dofw= date.getDayOfWeek();
    if( dofw != SUNDAY)
    {
      date.setDay( 31- (dofw+1));
    };
    
    return( date);
}           

//===================================================================================
const char* MyDate::getStringFormat( void)
{
  return( "%02d/%02d/%04d");  
}

//===================================================================================
size_t MyDate::getStringBufferSize( void)
{
  return( strlen( MyDate::getStringFormat())+1);      
}

//===================================================================================
char* MyDate::toString( char* buffer) 
{     
  sprintf(buffer, MyDate::getStringFormat(), day, month, year);
  return( buffer);
}
  
//===================================================================================
