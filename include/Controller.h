#pragma once


#include <cstddef>
#include <string>

typedef enum  // counting from the left
{ 
  eButton1 = 0x01,
  eButton2 = 0x02,
  eButton3 = 0x04,
  eButton4 = 0x08,

  eButton5 = 0x10,
  eButton6 = 0x20,
  eButton7 = 0x40,
  eButton8 = 0x80,
}  Buttons_t;

typedef enum
{
   eNOP,
}  State_t;

//===================================================================================
class Controller
{
   private:
      State_t  state;

   public:
      Controller( void);
      ~Controller( void)= default;

      void handle( Buttons_t button);

      State_t getState( void);
};

//===================================================================================

