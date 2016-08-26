#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include "read_file.h"
#include "mystring.h"
#include "interface.h"
#include "enemy.h"
#include "hero.h"
#include "fight.h"
#include "character_panel.h"
#include "inventory_panel.h"

typedef struct {
	char** welcome_message;
	size_t wm_length;

	char** variants_to_do;
	size_t number_of_variants;

	char*** variants_text;
	enemy_t* enemy;
	} level_t; 

/* level text file struct 						*/
/* number of welcome message rows					*/
/* welcome message							*/
/* Number of variants to do						*/
/* Variant 1 string					(it's first stirng in level->variants_to_do)		*/
/* Number of strings before next variant				*/
/* Variant 1 to do func (show_text, next_level, figth_with_enemy)(it's first string array in level->variants_text)	*/
/* Variant 2								*/
/*...									*/
/* Enemy_name								*/


level_t* parse_level(char* level_name) {
	
	/* Returninng level */
	level_t* level;

	/* Level file content */
	char** text;

	char* str_end, *enemy_name;
	size_t rows_num, cur_parsing_row = 0;
	size_t iter, iter2;
	FILE* file = fopen(concat("../levels/", level_name,".level"), "r");
	text = read_file(file);
	level = (level_t*) malloc(sizeof(level_t));


	/* Welcome message length */
	level->wm_length = strtoul(text[cur_parsing_row], &str_end, 10);

	cur_parsing_row++;


	/* Welcome message */
	level->welcome_message = (char**) malloc(sizeof(char*) * level->wm_length);
	for(iter = 0; iter < level->wm_length; iter++) {
		level->welcome_message[iter] = text[cur_parsing_row];	
		cur_parsing_row++;
	}
	
	/* Number of variants to do */
	level->number_of_variants = strtoul(text[cur_parsing_row], &str_end, 10);

	cur_parsing_row++;

	/* Variants */
	level->variants_to_do = (char**) malloc(sizeof(char*) * level->number_of_variants);
	level->variants_text = (char***) malloc(sizeof(char*) * level->number_of_variants);

	for(iter = 0; iter < level->number_of_variants; iter++) {
		level->variants_to_do[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	
		/*Variants text */
		rows_num = strtoul(text[cur_parsing_row], &str_end, 10);
		level->variants_text[iter] = (char**) malloc(sizeof(char*) * (rows_num + 1));
		for(iter2 = 0; iter2 < rows_num	+ 1; iter2++){
			level->variants_text[iter][iter2] = text[cur_parsing_row];
			cur_parsing_row++;	
		}
	}
	
	enemy_name = text[cur_parsing_row];
	if(!is_sub_string(enemy_name , "none")) {  
		level->enemy = parse_enemy(enemy_name);
	}
	fclose(file);
	free(text);
	return level;
	
}
		
void destroy_level(level_t* level) {
	size_t iter, variants_num;
	variants_num = level->number_of_variants;
	for(iter = 0; iter < variants_num; iter++) 
		free(level->variants_text[iter]);	
	free(level->variants_text);
	free(level->variants_to_do);
	free(level->welcome_message);
	destroy_enemy(level->enemy);
	free(level);
}


void update_variants(level_t* level) {
        size_t iter;
        for(iter = 0; iter < level->number_of_variants; iter++)
               mvwprintw(stdscr,38, getmaxx(stdscr)/level->number_of_variants * iter + 10, "%lu.%s", iter + 1, level->variants_to_do[iter]);
}


void prepare_screen_for_level(level_t* level) {
        size_t iter;
        clear();
        load_interface();

        for(iter = 0; iter < level->wm_length; iter++)
                mvwprintw(stdscr, iter + 7, 10, "%s", level->welcome_message[iter]);

        update_variants(level);

        refresh();
}


int load_level(char* level_name, hero_t* hero) {
        size_t iter;
        char key;
        char* end;
        int is_next_level = 0;
		int game_over = 0;
        level_t* level;
        level = parse_level(level_name);
        prepare_screen_for_level(level);

        while(!is_next_level) {
                key = getch();
                switch(key) {
                	case '1':
							if(level->number_of_variants >= 1) {
								clear_game_screen();

								if(is_sub_string(level->variants_text[0][1], "show_text") == 1) {
										for(iter = 0; iter < strtoul(level->variants_text[0][0], &end, 10) - 1; iter++){
												mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[0][iter+2]);
										}
								}

								if(is_sub_string(level->variants_text[0][1], "next_level")) {
										is_next_level = 1;
										destroy_level(level);
								}

								if(is_sub_string(level->variants_text[0][1], "fight")) {
										game_over = !fight(hero, (enemy_t*)level->enemy);
										is_next_level = 1;
								}
							}

							break;

					case '2':
							if(level->number_of_variants >= 2) {

								if(is_sub_string(level->variants_text[1][1], "show_text")) {
										for(iter = 0; iter < strtoul(level->variants_text[1][0], &end, 10) - 1; iter++){
												mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[1][iter+2]);
										}
								}

								if(is_sub_string(level->variants_text[1][1], "fight")) {
										game_over = !fight(hero, (enemy_t*)level->enemy);
										is_next_level = 1;
								}


								if(is_sub_string(level->variants_text[1][1], "next_level")) {
										is_next_level = 1;
										destroy_level(level);
								}

							}

							break;


					case '3':
							if(level->number_of_variants >= 3) {

								if(is_sub_string(level->variants_text[2][1], "show_text")) {
										for(iter = 0; iter < strtoul(level->variants_text[2][0], &end, 10) - 1; iter++){
												mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[2][iter+2]);
										}
								}

								if(is_sub_string(level->variants_text[2][1], "fight")) {
										game_over = !fight(hero, (enemy_t*)level->enemy);
										is_next_level = 1;
								}


								if(is_sub_string(level->variants_text[2][1], "next_level")) {
										is_next_level = 1;
										destroy_level(level);
								}
		
							}

							break;

					case 'q':
							is_next_level = 1;
							break;
			
			case 'c':
				character_panel(hero);
				break;
			
			case 'i':
				inventory_panel(hero, hero->weapon_inventory);
				break;
			
			default: 
				break;
                }
        }
		
        clear();
		if(!game_over) return 1;
		else return 0;
}
