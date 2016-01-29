#include <stdlib.h>
#include <ncurses.h>
#include "levelParse.h"


void loadLevel(char* levelName) {
	size_t iter;
	char key;
	level level;
	level = parseLevel(levelName);
	for(iter = 0; iter < level.wm_length; iter++)											
		mvwprintw(stdscr, iter + 7, 10, "%s", level.welcome_message[iter]);						
	
	for(iter = 0; iter < level.number_of_variants; iter++) 										
		mvwprintw(stdscr,38, getmaxx(stdscr)/level.number_of_variants * iter + 10, "%lu.%s", iter + 1, level.variants_to_do[iter]);	
	
	refresh();
	while(1) {
		
}
	



