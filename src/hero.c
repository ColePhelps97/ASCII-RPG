#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
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
	
	stats_t* stats;
} hero_t;



void hero_update_stats(hero_t* hero); 

	
hero_t* init_hero(){
	hero_t* hero;

	hero = (hero_t*)malloc(sizeof(hero_t));
	hero->stats = (stats_t*) malloc(sizeof(stats_t));

	hero->level = 1;

	hero->max_health = 100;
	hero->health = 100;

	hero->max_mana = 100;
	hero->mana = 100;

	hero->attack = 1;

	hero->exp = 0;
	
	hero->gold = 0;
	
	hero->defense = 0;
	
	hero->evasion = 1;

	hero->weapon_inventory = init_weapon_inventory();
	hero->armor_inventory = init_armor_inventory();

	hero->stats->strength = 3;
	hero->stats->agility = 3;
	hero->stats->intellect = 3;

	hero->stats->unspent_points = 0;

	hero_update_stats(hero);
	
	return hero;
}

void hero_up(hero_t* hero) {
	const experience_t level_up_step = 100; 
	while(hero->exp >= level_up_step) {
		hero->exp -= level_up_step; 
		hero->level ++;
		
		hero->health = hero->max_health;
		hero->mana = hero->max_mana;

		hero->stats->unspent_points+=5;
	}
}

void hero_update_stats(hero_t* hero) {
	const attack_t base_attack_value = 3;
	const health_t base_health_value = 70;
	const mana_t base_mana_value = 70;
	
	health_t prev_max_health = hero->max_health;
	mana_t prev_max_mana= hero->max_mana;

	hero->max_health = base_health_value + 10 * hero->stats->strength;
	hero->attack = base_attack_value + hero->stats->strength;
	hero->max_mana = base_mana_value + 10 * hero->stats->intellect;

	if(hero->health == prev_max_health) hero->health = hero->max_health;
	if(hero->mana == prev_max_mana) hero->mana= hero->max_mana;
}

/*void hero_update_equiped_items(*/
void equip_weapon(hero_t* hero, weapon_item_t* weapon) {
    if(!weapon->is_equiped) {
        /*hero->equiped_weapon->is_equiped = 0;*/
        weapon->is_equiped = 1;
        hero->attack += weapon->attack;
    
        /*add_to_equiped inventory*/
    }   
}

void equip_armor(hero_t* hero, armor_item_t* armor) {
    if(!armor->is_equiped) {
        /*hero->equiped_armor->is_equiped = 0;*/
        armor->is_equiped = 1;
        hero->defense += armor->defense;
    }   
}

void use_health_poition(hero_t* hero, health_poition_t* poition) {
    if(((health_t)hero->health + (health_t)poition->capacity >= (health_t)hero->max_health) && (hero->health != hero->max_health)) {
        hero->health = hero->max_health;
        /*delete poition from inventory*/
        /*destroy poition*/
    }
/*    if(hero->health == hero->max_health)*/
        /*warning! u tebya max hp, chto tyi tvorish???*/
        /*already_max_hp_warning();*/
    /*else {*/
        /*hero->health += poition->capacity;*/
        /*delete poition from inventory*/
        /*destroy poition*/
    /*}*/
}

void use_mana_poition(hero_t* hero, mana_poition_t* poition) {
    if(((mana_t)hero->mana+ (mana_t)poition->capacity >= (mana_t)hero->max_mana) && (hero->mana!= hero->max_mana)) {
        hero->mana= hero->max_mana;
        /*delete poition from inventory*/
        /*destroy poition*/
    }
    /*if(hero->mana == hero->max_mana)*/
        /*warning! u tebya max hp, chto tyi tvorish???*/
        /*already_max_mp_warning();*/
    /*else {*/
        /*hero->mana += poition->capacity;*/
        /*delete poition from inventory*/
        /*destroy poition*/
    /*}*/
}


