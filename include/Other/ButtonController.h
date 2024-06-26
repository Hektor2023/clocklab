#pragma once

#include <cstddef>
#include <string>
#include "Menu.h"

typedef enum // counting from the left
{
   eButton1 = 0x01,
   eButton2 = 0x02,
   eButton3 = 0x04,
   eButton4 = 0x08,

   eButton5 = 0x10,
   eButton6 = 0x20,
   eButton7 = 0x40,
   eButton8 = 0x80,
} Buttons_t;

//===================================================================================
class ButtonController
{
private:
   State_t state;
   Menu menuView;

public:
   ButtonController(void);
   ~ButtonController(void) = default;

   void handle(Buttons_t button);

   State_t getState(void);
};

//===================================================================================
