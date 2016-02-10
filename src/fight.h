#ifndef _FIGHT_H_
#define _FIGHT_H_
#include "hero.h"
#include "enemy.h"



void update_info(hero hero, enemy enemy);

void prepare_screen_for_fight(hero hero, enemy enemy);

void fight(hero hero, enemy enemy);


#endif

