#include <stdlib.h>
#include "items.h"
#include "constants.h"
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


void add_weapon_to_inventory(weapon_inventory_t* inventory, weapon_item_t* item); 

weapon_inventory_t* init_weapon_inventory(void) {
	weapon_inventory_t* inventory = (weapon_inventory_t*)malloc(sizeof(weapon_inventory_t));
	inventory->capacity = 20;
	inventory->used_slots = 0;
	inventory->start_item = NULL;

	
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_1"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_2"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_3"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_4"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_5"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_6"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_7"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_8"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_9"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_10"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_11"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword_12"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword"));
	add_weapon_to_inventory(inventory, parse_weapon("wooden_sword"));

	return inventory;
}

weapon_inventory_item_t* get_last_weapon(weapon_inventory_t* inventory) {
	weapon_inventory_item_t* buf;
	buf = inventory->start_item;
	/*for(buf->next, buf = buf->next);*/
	while(buf->next) 
		buf = buf->next;
	return buf;
}
	


void add_weapon_to_inventory(weapon_inventory_t* inventory, weapon_item_t* item) {
	weapon_inventory_item_t* last, *adding_item;
	adding_item = (weapon_inventory_item_t*)malloc(sizeof(weapon_inventory_item_t));
	if((int)inventory->capacity - (int)inventory->used_slots > 0) {
		/* Empty iventory */
		if(inventory->start_item != NULL) {
			last = get_last_weapon(inventory);
			adding_item->weapon = item;
			adding_item->next = NULL;
			adding_item->prev = last;
			last->next = adding_item;
			inventory->used_slots++;
		}
		
		else {
			adding_item->weapon = item;
			adding_item->next = NULL;
			adding_item->prev = NULL;
			inventory->start_item = adding_item;
			inventory->used_slots++;
		}
	}
	else {
		/* INVENTORY FULL!!!! */
		puts("INVENTORY FULL");
	}
	
}

void delete_weapon_from_inventory(weapon_inventory_t* inventory, weapon_inventory_item_t* item) {
	if(item) {
		/* Not last and not first item */
		if(item->prev != NULL && item->next != NULL) {
			item->prev->next = item->next;
			item->next->prev = item->prev;
		}

		/* First item */
		if(item->prev == NULL && item->next != NULL) {
			inventory->start_item = item->next;
			item->next->prev = NULL;
		}

		/* Last item */
		if(item->prev != NULL && item->next == NULL) {
			item->prev->next = NULL;
		}

		/* single item */
		if(item->prev == NULL && item->next == NULL) {
			inventory->start_item = NULL;
		}

		inventory->used_slots--;
		free(item);
	} 
}

void sell_weapon(weapon_inventory_t* inventory, weapon_inventory_item_t* item) {
	/*hero->gold += item->weapon->price;*/
	delete_weapon_from_inventory(inventory, item);
}

void paint_inventory_panel(WINDOW* window, PANEL* panel, weapon_inventory_t* weapon_inventory) {
	size_t iter;

	for(iter = 1; iter < 23; iter++) {
		mvwprintw(window, iter, 40, "%s", "|");
	}

	for(iter = 1; iter < 69; iter++) {
		mvwprintw(window, 23, iter, "%s", "_");
		mvwprintw(window, 26, iter, "%s", "-");
	}

	for(iter = 41; iter < 69; iter++) {
		mvwprintw(window, 8, iter, "%s", "_");
		mvwprintw(window, 15, iter, "%s", "_");
	}

	mvwprintw(window, 4, 44, "%s", WEAPON_INVENTORY);
	mvwprintw(window, 12, 44, "%s", ARMOR_INVENTORY);
	mvwprintw(window, 19, 44, "%s", POITION_INVENTORY);
	mvwprintw(window, 25, 10, "%s", MORE);
	mvwprintw(window, 25, 30, "%s", DROP);
	mvwprintw(window, 25, 50, "%s", SELL);
	mvwprintw(window, 27, 20, "%s", EQUIP);
	mvwprintw(window, 27, 40, "%s", CLOSE);
}

void update_used_slots(WINDOW* window, weapon_inventory_t* weapon_inventory) {
	mvwprintw(window, 5, 52, "%d/%d", weapon_inventory->used_slots, weapon_inventory->capacity);
	update_panels();
	doupdate();
}

void clear_items(WINDOW* window) {
	size_t iter1, iter2;
	for(iter1 = 1; iter1 < 23; iter1++)
		for(iter2 = 1; iter2 < 40; iter2++) 
			mvwprintw(window, iter1, iter2, "%s", " ");
}
	
	

void inventory_panel(weapon_inventory_t* weapon_inventory) {
	WINDOW* window;
	PANEL* panel;
	char key;
	int exit = 0, list_position = 0, item_position;
	weapon_inventory_item_t* item;
	weapon_inventory_item_t* start_item;
	weapon_inventory_item_t* info_item;
	weapon_inventory_item_t* buf;
	size_t counter = 0;
	
	/*WINDOW* already_equiped_window;*/
	/*PANEL* already_equiped_panel;*/

	/*already_equiped_window = newwin(15, 30, 10, 25);*/
	/*already_equiped_panel = new_panel(already_equiped_window);*/
	/*box(already_equiped_window, 0, 0);*/

	window = newwin(30, 70, 3, 20);
	box(window, 0, 0);	
	panel = new_panel(window);
	top_panel(panel);
	
	paint_inventory_panel(window, panel, weapon_inventory);
	update_panels();
	doupdate();


/* test part */
	start_item = weapon_inventory->start_item;
	info_item = weapon_inventory->start_item;
	item_position = 0;

/*-------------------------------------*/
/* firstly print 10 first items */
/* if choosed weapon is last and next weapon isn't NULL -- reprint */
/* get 10 previous, get 10 next methods */
/* while(counter <= 10) */
	/*get next\previous, print*/
	/*counter ++*/
	
	while(!exit) {
		clear_items(window);
		item = start_item;
		counter = 0;
		while(counter <= 10 && counter <= weapon_inventory->used_slots && item != NULL) {
			mvwprintw(window, 2 + counter * 2, 4, "%s", item->weapon->name);
			item = item->next;
			if(counter == list_position) mvwprintw(window, 2 + counter * 2, 2, "%s", "*"); 
			counter++;
		}
		update_used_slots(window, weapon_inventory);
		update_panels();
		doupdate();
		key = getch();
		switch(key) {
			case 'c': 
					exit = 1;
					break;
			case 'j':
					if(list_position < 10 && list_position < weapon_inventory->used_slots - 1) {
						list_position++;
						info_item = info_item->next;
					}
					else {
						if(list_position + item_position < weapon_inventory->used_slots - 1) {
							start_item = start_item->next;
							info_item = info_item->next;
							item_position++;
						}
					}
					break;
			case 'k':
					if(list_position > 0) {
						list_position--;
						info_item = info_item->prev;
					}
					else {
						if(item_position > 0) {
							start_item = start_item->prev;
							info_item = info_item->prev;
							item_position--;
						}
					}
					break;
			case 'd':
					if(info_item->next != NULL) buf = info_item->next;
					else buf = info_item->prev;
			
					/*if(item_position > 0) list_position--;*/

					if(info_item == start_item) start_item = start_item->next;
					delete_weapon_from_inventory(weapon_inventory, info_item);
					info_item = buf;
					break;
			case 's':
					buf = info_item->next;
					sell_weapon(weapon_inventory, info_item);
					info_item = buf;
					break;
			default:
					break;
		}
	}

	del_panel(panel);
	delwin(window);

	/*del_panel(already_equiped_panel);*/
	/*delwin(already_equiped_window);*/
}


