#include "Other/Menu.h"
#include <assert.h>

//===================================================================================
Menu::Menu(Menu *parentMenu)
    : pParentMenu(parentMenu), idx(0), menuSize(1){};

//===================================================================================
void Menu::firstItem(void)
{
    idx = 0;
}

//===================================================================================
Menu *Menu::getSubmenu(void)
{
    return (items[idx].pSubmenu);
}

//===================================================================================
Menu *Menu::getParentMenu(void)
{
    return (pParentMenu);
}

//===================================================================================
void Menu::nextItem(void)
{
    idx++;
    idx = idx % menuSize;
}

//===================================================================================
State_t Menu::getState(void)
{
    return items[idx].state;
}

//===================================================================================
void Menu::addItem(State_t new_state, Menu *pNewSubmenu)
{
    assert((idx < DEFAULT_MENU_SIZE) && "Menu size exceeded");

    items[idx].state = new_state;
    items[idx].pSubmenu = pNewSubmenu;
    menuSize = idx + 1;
    idx++;
}

//===================================================================================