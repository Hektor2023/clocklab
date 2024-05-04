#pragma one

#include <Arduino.h>

constexpr uint8_t DEFAULT_MENU_SIZE = 8;

//===================================================================================
typedef enum
{
  eNOP,
  eShow_Time,
  eShow_Date,
  eState_3,
  eState_4,
  eState_5,
  eState_6,
  eState_7,
  eState_8,
  eState_9,
} State_t;

//===================================================================================
class Menu;
typedef struct MenuItem
{
  State_t state{State_t::eNOP};
  Menu *pSubmenu{nullptr};
} MenuItem_t;

//===================================================================================
class Menu
{
private:
  MenuItem_t items[DEFAULT_MENU_SIZE];
  Menu *pParentMenu{nullptr};
  uint8_t idx;
  uint8_t menuSize;

public:
  Menu(Menu *parentMenu);
  ~Menu(){};

  void firstItem(void);
  void nextItem(void);
  State_t getState(void);

  void addItem(State_t new_state, Menu *pNewSubmenu = nullptr);
  Menu *getSubmenu(void);
  Menu *getParentMenu(void);
};

//===================================================================================