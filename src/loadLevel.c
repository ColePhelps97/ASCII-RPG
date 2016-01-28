#include <stdlib.h>
#include <ncurses.h>
#include "levelParse.h"


void loadLevel(char* levelName) {
	unsigned long iter;
	level level;
	level = parseLevel(levelName);
	for(iter = 0; iter < level.wm_length; iter++) 
		mvwprintw(stdscr, iter + 7, 10, "%s", level.welcome_message[iter]);
	for(iter = 0; iter < level.number_of_variants; iter++) 
		mvwprintw(stdscr,38, getmaxx(stdscr)/level.number_of_variants * iter + 10, "%lu.%s", level.variants_to_do[iter]);
	}
	



