#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>
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

hero_t* init_hero();

void hero_up(hero_t* hero);
	
#endif
