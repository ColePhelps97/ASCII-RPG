#ifndef _PARSELEVEL_H_
#define _PARSELEVEL_H_
#include <stdlib.h>
#include "hero.h"

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

#endif
