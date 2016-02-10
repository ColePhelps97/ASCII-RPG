#ifndef _LEVEL_H_
#define _LEVEL_H_
#include <stdlib.h>
#include "hero.h"
#include "enemy.h"

typedef struct level {
	char** welcome_message;
	size_t wm_length;
	char** variants_to_do;
	size_t number_of_variants;
	char*** variants_text;
	enemy level_enemy;
} level;
 
level parse_level(char* level_name);
void destroy_level(level level);
void load_level(char* level_name, hero hero);
void update_variants(level level);
void prepare_screen_for_level(level level);

#endif
