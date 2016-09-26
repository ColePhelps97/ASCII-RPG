#ifndef _FIGHT_H_
#define _FIGHT_H_
#include "hero.h"
#include "enemy.h"



void update_info(hero_t* hero, enemy_t* enemy);

void prepare_screen_for_fight(hero_t* hero, enemy_t* enemy);

int fight(hero_t* hero, enemy_t* enemy);
int mult_fight(hero_t* hero, enemy_t* enemy, int sock);

int enemy_base_attack(enemy_t* enemy, hero_t* hero);

int hero_base_attack(hero_t* hero, enemy_t* enemy);
int hero_fireball_attack(hero_t* hero, enemy_t* enemy);
int hero_selfheal(hero_t* hero); 


#endif

