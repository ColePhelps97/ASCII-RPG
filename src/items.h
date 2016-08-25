#ifndef _ITEMS_H_
#define _ITEMS_H_
#include <stdlib.h>
#include "constants.h"

typedef struct {
	char* name;
	attack_t attack;
	gold_t price;
	char** picture;
	int is_equiped;
} weapon_item_t;

typedef struct {
	char* name;
	defense_t defense;
	evasion_t evasion;
	gold_t price;
	char** picture;
	int is_equiped;
} armor_item_t;

typedef struct {
	char* name;
	health_t capacity;
	gold_t price;
	char** picture;
} health_poition_t;

typedef struct {
	char* name;
	mana_t capacity;
	gold_t price;
	char** picture;
} mana_poition_t;

weapon_item_t* parse_weapon(char* weapon_name);

#endif
