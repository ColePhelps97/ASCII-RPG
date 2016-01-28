#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"

struct level {
	char** welcome_message;
	char** variants_to_do;
	char*** variants_text;
	}

/* level text file struct 						*/
/* number of welcome message rows					*/
/* welcome message							*/
/* Number of variants to do						*/
/* Variant 1 string							*/
/* Variant 1 to do func (show_text, next_level, figth_with_enemy)	*/
/* if to do is show_text here must be number of text's rows to show	*/
/* Variant 2								*/
/*...									*/






struct level parseLevel(char* levelName) {
	
	char** file_text;
	struct level level;
	unsigned long rows_num, cur_parsing_row = 0, variants_num;
	char *ulstr_end;
	size_t iter, iter2;
	FILE *file = fopen(levelName, "r");
	
	file_text = read_file(file);

	rows_num = strtoul(file_text[cur_parsing_row], &ulstr_end; 10);
	level.welcome_message = (char*) malloc(sizeof(char*) * rows_num);
	
	cur_parsing_row ++;

	for(iter = 0; iter < rows_num; iter++) {
		level.welcome_message[iter] = file_text [cur_parsing_row];
		cur_parsing_row++;
	}
	
	variants_num = strtoul(file_text[cur_parsing_row], &ulstr_end, 10);
	level.variants_text = (char*) malloc(sizeof(char*)*variants_num);
	
	cur_parsing_row++;
	
	for(iter = 0; iter < variants_num; iter++) {
		level.variants_to_do[iter] = file_text[cur_parsing_row];
		cur_parsing_row++;

		rows_num = strtoul(file_text[cur_parsing_row], &ulstr_end, 10);
		level.variants_text[iter] = (char*) malloc(sizeof(char*)*rows_num);

		cur_parsing_row++;
		
		for(iter2 = 0; iter2 < rows_num; iter2++) {
			level.variants_text[iter][iter2] = file_text[cur_parsing_row];
			cur_parsing_row++;
		} 
	}
		




















	
	
