#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "constants.h"

typedef struct {
	char* name;
	char** welcome_message;
    char** picture;
    size_t wm_length;
    size_t picture_length;
    health_t max_health;
    health_t health;
	evasion_t evasion;
	defense_t defense;
    experience_t exp;
    attack_t attack;
} enemy_t;

enemy_t* parse_enemy(char* enemy_name);

void destroy_enemy(enemy_t* enemy);

#endif
