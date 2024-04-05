#pragma once

typedef unsigned char  uint8_t;
 
#include <cstddef>
#include <stdio.h>
#include <arduino.h>

//===================================================================================
class MyTime
{
  private:
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;

  public:
    MyTime( void);
    MyTime( uint8_t hour, uint8_t minute, uint8_t second);
    MyTime( const MyTime& time);
    ~MyTime( void);

    MyTime& operator =( const MyTime& time);

    friend bool operator==(MyTime const & time1, MyTime const & time2);
    friend bool operator!=(MyTime const & time1, MyTime const & time2);

    uint8_t getHour( void) const; 
    void setHour( uint8_t day);

    uint8_t getMinute( void) const;
    void setMinute( uint8_t minute);
   
    uint8_t getSecond( void) const;
    void setSecond( uint8_t second);

    uint16_t getInSeconds( void);
    void     setInSeconds( uint16_t timeInSecs);
    
    MyTime& operator++( void);
    MyTime  operator++(  int);

    void hourInc( void);
    void hourDec( void);
    void minuteInc( void);
    void minuteDec( void);
    void secondInc( void);
    void secondDec( void);

    
    static const char* getStringFormat( void);
    static size_t getStringBufferSize( void);   

    static const char* getStringShortFormat( void);
    static size_t getStringShortBufferSize( void);  

    char*  toString( char* Stringbuffer);   
    char*  toShortString( char* Stringbuffer);

    static bool isValidHour( uint8_t hour);
    static bool isValidMinute( uint8_t minute);
    static bool isValidSecond( uint8_t second);
};


//===================================================================================

