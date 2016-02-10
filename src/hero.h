#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>
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

hero init_hero();

void hero_up(hero hero);
	

#endif
