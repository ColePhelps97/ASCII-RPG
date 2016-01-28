#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>

typedef unsigned int health_t;
typedef unsigned int mana_t;
typedef unsigned int level_t;


typedef struct hero {
	health_t maxHeroHealth;
        health_t currentHeroHealth;
        mana_t maxHeroMana;
        mana_t currentHeroMana;
        level_t heroLevel;
} hero;

hero initHero();

#endif
