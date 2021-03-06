#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "constants.h"
#include "read_file.h"
#include "mystring.h"

typedef struct {
    char* name;
    attack_t attack;
    gold_t price;
	size_t picture_length;
    char** picture;
	size_t description_length;
	char** description;
    int is_equipped;
} weapon_item_t;

typedef struct {
    char* name;
    defense_t defense;
    evasion_t evasion;
    gold_t price;
	size_t picture_length;
    char** picture;
	size_t description_length;
	char** description;
    int is_equipped;
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

/* WEAPON ITEM FILE STRUCT */
/* Weapon name */
/* Attack */
/* Price */
/* number of picture lines */
/* picture */
/* number of description length */
/* description */

weapon_item_t* parse_weapon(char* weapon_name) {
	weapon_item_t* weapon;
	char** text;
	char* str_end;
	size_t cur_parsing_row = 0, iter;

	FILE* file = fopen(concat("../weapons/", weapon_name, ".weapon"), "r");
	
	text = read_file(file);

	weapon = (weapon_item_t*) malloc(sizeof(weapon_item_t));
	
	weapon->name = text[cur_parsing_row];
	cur_parsing_row++;

	weapon->attack = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	weapon->price = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	weapon->picture_length = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	weapon->picture = (char**) malloc(sizeof(char*) * weapon->picture_length);

	for(iter = 0; iter < weapon->picture_length; iter++) {
		weapon->picture[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	}

	weapon->description_length = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	weapon->description = (char**) malloc(sizeof(char*) * weapon->description_length);
	for(iter = 0; iter < weapon->description_length; iter++) {
		weapon->description[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	}
	free(text);
	fclose(file);
	return weapon;
}

/* ARMOR file struct */
/* name */
/* defense */
/* evasion */
/* price */
/* number of picture length */
/* picture */


armor_item_t* parse_armor(char* armor_name) {
	armor_item_t* armor;
	char** text;
	char* str_end;
	size_t cur_parsing_row = 0, iter;

	FILE* file = fopen(concat("../armors/", armor_name, ".armor"), "r");
	
	text = read_file(file);

	armor = (armor_item_t*) malloc(sizeof(armor_item_t));
	
	armor->name = text[cur_parsing_row];
	cur_parsing_row++;

	armor->defense = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	armor->evasion = atof(text[cur_parsing_row]);
	cur_parsing_row++;

	armor->price = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	armor->picture_length = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	armor->picture = (char**) malloc(sizeof(char*) * armor->picture_length);

	for(iter = 0; iter < armor->picture_length; iter++) {
		armor->picture[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	}

	armor->description_length = strtoul(text[cur_parsing_row], &str_end, 10);
	cur_parsing_row++;

	armor->description = (char**) malloc(sizeof(char*) * armor->description_length);
	for(iter = 0; iter < armor->description_length; iter++) {
		armor->description[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	}
	fclose(file);
	free(text);
	return armor;
}
