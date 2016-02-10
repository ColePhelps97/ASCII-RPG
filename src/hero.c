#include <stdlib.h>
#include <stdio.h>
#include "constants.h"

		
typedef struct hero { 
	health_t max_hero_health;
	health_t current_hero_health;
	mana_t max_hero_mana;
       	mana_t current_hero_mana;
	level_t hero_level;
	experience_t hero_exp;
	attack_t hero_attack;
} hero;


	
hero init_hero(){
	hero hero;
	hero.hero_level = 1;
	hero.max_hero_health = 100;
	hero.current_hero_health = 100;
	hero.max_hero_mana = 100;
	hero.current_hero_mana = 100;
	hero.hero_exp = 0;
	hero.hero_attack = 2;
	return hero;
}

void hero_up(hero hero) {
	const experience_t level_up_step = 100; 
	while(hero.hero_exp >= level_up_step) {
		hero.hero_exp -= level_up_step; 
		hero.hero_level += 1;
	}
}

