#include "Controller.h"

#include "Arduino.h"

//===================================================================================
Controller::Controller( void)
  :state( State_t::eNOP)
{};
  
//===================================================================================
void Controller::handle( Buttons_t buttons)
{
  if( buttons!= 0)  
  {
    Serial.printf("| Button..|  %x\n", buttons);
  }

  switch( buttons)
    {
      case  Buttons_t:: eButton1:
          break;

      case  Buttons_t:: eButton2:
          break;

      case  Buttons_t:: eButton3:
          break;

      case  Buttons_t:: eButton4:
          break;

      case  Buttons_t:: eButton5:
          break;

      case  Buttons_t:: eButton6:
          break;

      case  Buttons_t:: eButton7:
          break;

      case  Buttons_t:: eButton8:
          break;

      default:
        break;
    }

}

//===================================================================================
State_t Controller::getState( void)
{
  return state;
}

//===================================================================================