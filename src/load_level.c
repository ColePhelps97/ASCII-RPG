#include <stdlib.h>
#include <ncurses.h>
#include "level.h"
#include "mystring.h"
#include "main_interface.h"
#include "hero.h"
#include "clear_screen.h"
#include "fight.h"

void update_variants(level level) {
	size_t iter;
	for(iter = 0; iter < level.number_of_variants; iter++)
 	       mvwprintw(stdscr,38, getmaxx(stdscr)/level.number_of_variants * iter + 10, "%lu.%s", iter + 1, level.variants_to_do[iter]);
}
	

void prepare_screen_for_level(level level, hero hero) {	
	size_t iter;
	clear();
        load_interface(hero);

        for(iter = 0; iter < level.wm_length; iter++)
                mvwprintw(stdscr, iter + 7, 10, "%s", level.welcome_message[iter]);
	
	update_variants(level);

        refresh();
}

	


void load_level(char* level_name, hero hero) {
	size_t iter;
	char key;	
	char* end;
	int is_next_level = 0;
	level level;
	
	level = parse_level(level_name);
	prepare_screen_for_level(level, hero);	

	while(!is_next_level) {
		key = getch();
		switch(key) {
		
			case '1': 
				clear_game_screen();	
				if(is_sub_string(level.variants_text[0][1], "show_text") == 1) {
					for(iter = 0; iter < strtoul(level.variants_text[0][0], &end, 10) - 1; iter++){
						mvwprintw(stdscr, iter + 7, 10, "%s", level.variants_text[0][iter+2]);
					}
				}
				
				if(is_sub_string(level.variants_text[0][1], "next_level")) {		
					is_next_level = 1;
					destroy_level(level);
				}
				
				if(is_sub_string(level.variants_text[0][1], "fight")) {
                                        fight(hero, level.level_enemy);
					getch();
					is_next_level = 1;
                                }

				break;

			case '2':
				if(is_sub_string(level.variants_text[1][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level.variants_text[1][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level.variants_text[1][iter+2]);
                                        }             
                                }
				
				if(is_sub_string(level.variants_text[1][1], "fight")) {
                                	fight(hero, level.level_enemy); 
					getch();
					is_next_level = 1;
                                }


                                if(is_sub_string(level.variants_text[1][1], "next_level")) {
                                        is_next_level = 1;
                                        destroy_level(level);
                                }

                                break;



			case '3':
                                if(is_sub_string(level.variants_text[2][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level.variants_text[2][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level.variants_text[2][iter+2]);
                                        }
                                }

                                if(is_sub_string(level.variants_text[2][1], "fight")) {
                                        fight(hero, level.level_enemy);
					getch();
					is_next_level = 1;
                                }


                                if(is_sub_string(level.variants_text[2][1], "next_level")) {
                                        is_next_level = 1;
                                        destroy_level(level);
                                }

                                break;

			case 'q':
				is_next_level = 1;
				break;	
		}
	}
	clear();
		
}
	



