#ifndef _PARSELEVEL_H_
#define _PARSELEVEL_H_
#include <stdlib.h>

typedef struct level {
	char** welcome_message;
	size_t wm_length;
	char** variants_to_do;
	size_t number_of_variants;
	char*** variants_text;
/*	char* next_level; */
} level;
 
level parseLevel();

#endif
