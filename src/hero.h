#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>
#include "constants.h"
#include "inventory.h"

typedef struct {
	stat_t strength;
	stat_t agility;
	stat_t intellect;
	stat_t unspent_points;
} stats_t;

typedef struct {

	health_t max_health;
	health_t health;

   	mana_t max_mana;
    mana_t mana;

    person_level_t level;

	attack_t attack;

	experience_t exp;
	
	gold_t gold;
	
	defense_t defense;
	evasion_t evasion;

	weapon_inventory_t* weapon_inventory;
	armor_inventory_t* armor_inventory;

	weapon_item_t* equipped_weapon;
	armor_item_t* equipped_armor;
	
	stats_t* stats;

} hero_t;

hero_t* init_hero();

void hero_up(hero_t* hero);

void hero_update_stats(hero_t* hero);
	
#endif
