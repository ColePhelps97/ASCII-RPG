#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include "constants.h"

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
