#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>
#include "constants.h"

typedef struct stats {
        stat_t strength;
        stat_t intellect;
        stat_t agility;
        stat_t unspent_points;
} stats_t;


typedef struct {
	health_t max_hero_health;
        health_t current_hero_health;
        mana_t max_hero_mana;
        mana_t current_hero_mana;
        level_t hero_level;
        stat_t strength;
        stat_t intellect;
        stat_t agility;
        stat_t unspent_points;
	attack_t hero_attack;
	experience_t hero_exp;
        /*stats_t* stats;*/

} hero;

hero* init_hero();

void hero_up(hero* hero);
	

#endif
