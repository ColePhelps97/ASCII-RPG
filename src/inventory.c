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

typedef struct armor_inventory_item_t{
	armor_item_t* armor;
	struct armor_inventory_item_t* next;
	struct armor_inventory_item_t* prev;
} armor_inventory_item_t;

typedef struct {
	armor_inventory_item_t* start_item;
	size_t capacity;
	size_t used_slots;
} armor_inventory_t;

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

void add_armor_to_inventory(armor_inventory_t* inventory, armor_item_t* item); 

armor_inventory_t* init_armor_inventory(void) {
	armor_inventory_t* inventory = (armor_inventory_t*)malloc(sizeof(weapon_inventory_t));
	inventory->capacity = 20;
	inventory->used_slots = 0;
	inventory->start_item = NULL;
	
	add_armor_to_inventory(inventory, parse_armor("wooden_armor"));

	return inventory;
}

weapon_inventory_item_t* get_last_weapon(weapon_inventory_t* inventory) {
	weapon_inventory_item_t* buf;
	buf = inventory->start_item;
	while(buf->next) 
		buf = buf->next;
	return buf;
}
	
armor_inventory_item_t* get_last_armor(armor_inventory_t* inventory) {
	armor_inventory_item_t* buf;
	buf = inventory->start_item;
	while(buf->next) 
		buf = buf->next;
	return buf;
}

void add_weapon_to_inventory(weapon_inventory_t* inventory, weapon_item_t* item) {
	weapon_inventory_item_t* last, *adding_item;
	puts("add_weapon");
	adding_item = (weapon_inventory_item_t*)malloc(sizeof(weapon_inventory_item_t));
	puts("add_weapon2");
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

void free_weapon_inventory(weapon_inventory_t* weapon_inventory) {
	weapon_inventory_item_t* item2;
	weapon_inventory_item_t* item = weapon_inventory->start_item;
	while(item->next) {
		item->next->prev = NULL;
		item2 = item;
		item = item->next;
		free(item2->weapon);
		free(item2);
	}
	free(item);
	free(weapon_inventory);
}	
void add_armor_to_inventory(armor_inventory_t* inventory, armor_item_t* item) {
	armor_inventory_item_t* last, *adding_item;
	adding_item = (armor_inventory_item_t*)malloc(sizeof(weapon_inventory_item_t));
	if((int)inventory->capacity - (int)inventory->used_slots > 0) {
		/* Empty iventory */
		if(inventory->start_item != NULL) {
			last = get_last_armor(inventory);
			adding_item->armor = item;
			adding_item->next = NULL;
			adding_item->prev = last;
			last->next = adding_item;
			inventory->used_slots++;
		}
		
		else {
			adding_item->armor = item;
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

void delete_armor_from_inventory(armor_inventory_t* inventory, armor_inventory_item_t* item) {
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



