#ifndef _LEVEL_H_
#define _LEVEL_H_
#include <stdlib.h>
#include "hero.h"
#include "enemy.h"

typedef struct {
	char** welcome_message;
	size_t wm_length;

	char** variants_to_do;
	size_t number_of_variants;
	char*** variants_text;

	enemy_t enemy;
} level_t;
 
level_t* parse_level(char* level_name);
void destroy_level(level_t* level);
int load_level(char* level_name, hero_t* hero);
void update_variants(level_t level);
void prepare_screen_for_level(level_t level);

#endif
