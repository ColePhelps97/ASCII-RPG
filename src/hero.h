#ifndef _HERO_H_
#define _HERO_H_
#include <stdlib.h>

typedef unsigned int health_t;
typedef unsigned int mana_t;
typedef unsigned int level_t;
typedef unsigned int experience_t;
typedef unsigned int attack_t;



typedef struct hero {
	health_t max_hero_health;
        health_t current_hero_health;
        mana_t max_hero_mana;
        mana_t current_hero_mana;
        level_t hero_level;
	experience_t hero_exp;
	attack_t hero_attack;
} hero;

typedef struct enemy {
	 char* enemy_name;
         char** enemy_welcome_message;
         char** enemy_picture;
         size_t wm_length;
         size_t picture_length;
         health_t max_enemy_health;
         health_t current_enemy_health;
         experience_t exp;
         attack_t enemy_attack;

} enemy;

hero init_hero();

void hero_up(hero hero);
	
enemy parse_enemy(char* enemy_name);


#endif
