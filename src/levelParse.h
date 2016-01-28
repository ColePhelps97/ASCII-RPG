#ifndef _PARSELEVEL_H_
#define _PARSELEVEL_H_
#include <stdlib.h>

typedef struct level {
	char** welcome_message;
	unsigned long wm_length;
        char** variants_to_do;
	unsigned long number_of_variants;
        char*** variants_text;
} level;
 
level parseLevel(char* levelName);

#endif
