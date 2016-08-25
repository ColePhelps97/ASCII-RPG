#ifndef _INVENTORY_H_
#define _INVENTORY_H_
#include <stdlib.h>
#include "constants.h"
#include "items.h"
#include <ncurses.h>
#include <panel.h>


typedef struct weapon_inventory_item_t{
	weapon_item_t* weapon;
	struct weapon_inventory_item_t* next;
	struct weapon_inventory_item_t* prev;
} weapon_inventory_item_t;

typedef struct {
	weapon_inventory_item_t* start_item;
	size_t capacity;
	size_t used_slots;
} weapon_inventory_t;

weapon_inventory_t* init_weapon_inventory(void);
weapon_inventory_item_t* get_last_weapon(weapon_inventory_t* inventory);
void add_weapon_to_inventory(weapon_inventory_t* inventory, weapon_item_t* item);
void inventory_panel(weapon_inventory_t* weapon_inventory);
void paint_inventory_panel(WINDOW* window, PANEL* panel, weapon_inventory_t* weapon_inventory);

#endif
