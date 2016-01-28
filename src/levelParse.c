#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "read_file.h"

typedef struct level {
	char** welcome_message;
	unsigned long wm_length;
	char** variants_to_do;
	unsigned long number_of_variants;
	char*** variants_text;
	} level; 

/* level text file struct 						*/
/* number of welcome message rows					*/
/* welcome message							*/
/* Number of variants to do						*/
/* Variant 1 string							*/
/* Number of strings before next variant				*/
/* Variant 1 to do func (show_text, next_level, figth_with_enemy)	*/
/* if to do is show_text here must be number of text's rows to show	*/
/* Variant 2								*/
/*...									*/






level parseLevel(char* levelName) {
	
	char** file_text;
	level level;
	unsigned long rows_num, cur_parsing_row = 0;
	char *ulstr_end;
	size_t iter, iter2;
	FILE *file = fopen(levelName, "r");
	
	file_text = read_file(file);
	
	print_file(file);	

	printf("%lu", count_frows(file));

	level.wm_length = strtoul(file_text[cur_parsing_row], &ulstr_end, 10);
	
	/* welcome message */
	level.welcome_message = malloc(sizeof(char*) * level.wm_length);
	
	cur_parsing_row ++;

	for(iter = 0; iter < level.wm_length; iter++) {
		level.welcome_message[iter] = file_text[cur_parsing_row];
		cur_parsing_row++;
	}
	
	/* variants */
	level.number_of_variants= strtoul(file_text[cur_parsing_row], &ulstr_end, 10);
	level.variants_text = malloc(sizeof(char*)*level.number_of_variants);
	
	cur_parsing_row++;
	
	for(iter = 0; iter < level.number_of_variants; iter++) {
		level.variants_to_do[iter] = file_text[cur_parsing_row];
		cur_parsing_row++;

		rows_num = strtoul(file_text[cur_parsing_row], &ulstr_end, 10);
		level.variants_text[iter] = malloc(sizeof(char*)*rows_num);

		cur_parsing_row++;
		
		for(iter2 = 0; iter2 < rows_num; iter2++) {
			level.variants_text[iter][iter2] = file_text[cur_parsing_row];
			cur_parsing_row++;
		} 
	}
	return level;
}
		




















	
	
