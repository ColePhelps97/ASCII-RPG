#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "constants.h"

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

enemy parse_enemy(char* enemy_name);

#endif
