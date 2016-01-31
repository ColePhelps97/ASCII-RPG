#include <stdlib.h>
#include <ncurses.h>
#include "constants.h"

void clear_game_screen() {
	size_t iter;
	for(iter = top_border; iter < bottom_border; iter++) {
		move(iter, 1);
		clrtoeol();
		mvwprintw(stdscr, iter, getmaxx(stdscr) - 1, "%s", "|");
	}
}
