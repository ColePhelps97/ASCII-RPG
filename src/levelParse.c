#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "read_file.h"

typedef struct level {
	char** welcome_message;
	size_t wm_length;
	char** variants_to_do;
	size_t number_of_variants;
	char*** variants_text;
	} level; 

/* level text file struct 						*/
/* number of welcome message rows					*/
/* welcome message							*/
/* Number of variants to do						*/
/* Variant 1 string							*/
/* Number of strings before next variant				*/
/* Variant 1 to do func (show_text, next_level, figth_with_enemy)	*/
/* Variant 2								*/
/*...									*/


level parseLevel() {
	level level1;
	char** text;
	char* str_end;
	size_t rows_num, cur_parsing_row = 0, variants_num;
	size_t iter, iter2;
	FILE* file = fopen("../levels/level1.level", "r");
	text = read_file(file);


	/* Welcome message length */
	rows_num = strtoul(text[cur_parsing_row], &str_end, 10);
	level1.wm_length = rows_num;
	cur_parsing_row++;


	/* Welcome message */
	level1.welcome_message = (char**) malloc(sizeof(char*) * rows_num);
	for(iter = 0; iter < rows_num; iter++) {
		level1.welcome_message[iter] = text[cur_parsing_row];	
		cur_parsing_row++;
	}
	
	/* Number of variants to do */
	variants_num = strtoul(text[cur_parsing_row], &str_end, 10);
	level1.number_of_variants = variants_num;
	cur_parsing_row++;

	/* Variants */
	level1.variants_to_do = (char**) malloc(sizeof(char*) * variants_num);
	level1.variants_text = (char***) malloc(sizeof(char*) * variants_num);
	for(iter = 0; iter < variants_num; iter++) {
		level1.variants_to_do[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	
		/*Variants text */
		rows_num = strtoul(text[cur_parsing_row], &str_end, 10);
		level1.variants_text[iter] = (char**) malloc(sizeof(char*) * (rows_num + 1));
		for(iter2 = 0; iter2 < rows_num	+ 1; iter2++){
			level1.variants_text[iter][iter2] = text[cur_parsing_row];
			cur_parsing_row++;	
		}
	}
	 
	fclose(file);
	return level1;

}
		
void destroyLevel(level level) {
	size_t iter, variants_num;
	variants_num = level.number_of_variants;
	for(iter = 0; iter < variants_num; iter++) 
		free(level.variants_text[iter]);	
	free(level.variants_text);
	free(level.variants_to_do);
	free(level.welcome_message);
}


















	
	
