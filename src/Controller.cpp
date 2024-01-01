#include "Controller.h"

#include "Arduino.h"

//===================================================================================
Controller::Controller( void)
  :state( State_t::eNOP),menuView( nullptr)
{
  menuView.addItem( State_t::eShow_Time);
  menuView.addItem( State_t::eShow_Date);
};
  
//===================================================================================
void Controller::handle( Buttons_t buttons)
{

/*
  if( buttons!= 0)  
  {
    Serial.printf("| Button..|  %x\n", buttons);
  }
*/
  switch( buttons)
    {
      case  Buttons_t:: eButton1:
        Serial.print("Button_1\n");
        menuView.nextItem();
        state= menuView.getState();
        break;

      case  Buttons_t:: eButton2:
        Serial.print("Button_2\n");
        break;

      case  Buttons_t:: eButton3:
        Serial.print("Button_3\n");
        break;

      case  Buttons_t:: eButton4:
        Serial.print("Button_4\n");
        break;

      case  Buttons_t:: eButton5:
        Serial.print("Button_5\n");
        break;

      case  Buttons_t:: eButton6:
        Serial.print("Button_6\n");
        break;

      case  Buttons_t:: eButton7:
        Serial.print("Button_7\n");
        break;

      case  Buttons_t:: eButton8:
        Serial.print("Button_8\n");
        break;

      default:
        state= State_t::eNOP;
        break;
    }

}

//===================================================================================
State_t Controller::getState( void)
{
  return state;
}

//===================================================================================