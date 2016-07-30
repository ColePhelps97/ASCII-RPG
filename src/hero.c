#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include "constants.h"


typedef struct { 
	health_t max_health;
	health_t health;

	mana_t max_mana;
    mana_t mana;

	person_level_t level;

	attack_t attack;

	experience_t exp;

} hero_t;



	
hero_t* init_hero(){
	hero_t* hero;

	hero = (hero_t*)malloc(sizeof(hero_t));

	hero->level = 1;

	hero->max_health = 100;
	hero->health = 100;

	hero->max_mana = 100;
	hero->mana = 100;

	hero->attack = 1;

	hero->exp = 0;

	return hero;
}

void hero_up(hero_t* hero) {
	const experience_t level_up_step = 100; 
	while(hero->exp >= level_up_step) {
		hero->exp -= level_up_step; 
		hero->level ++;
	}
}

