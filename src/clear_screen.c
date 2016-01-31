#include <stdlib.h>
#include <ncurses.h>

void clear_game_screen() {
	size_t iter;
	const size_t top_border = 5, bottom_border = 36;
	for(iter = top_border; iter < bottom_border; iter++) {
		move(iter, 1);
		clrtoeol();
		mvwprintw(stdscr, iter, getmaxx(stdscr) - 1, "%s", "|");
	}
}
