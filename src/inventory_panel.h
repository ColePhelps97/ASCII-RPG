#ifndef _INVENTORY_PANEL_H_
#define _INVENTORY_PANEL_H_
#include <curses.h>
#include <panel.h>
#include "inventory.h"
#include "hero.h"

void inventory_panel(hero_t* hero, weapon_inventory_t* weapon_inventory);

#endif
