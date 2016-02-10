#include <ncurses.h>
#include <stdlib.h>
#include "constants.h"


void load_interface(){
	size_t i=0;

	clear();
	
	
	move(0,0);
	printw(TOP_MENU_PANEL_UP);
	printw("\n");
	printw(TOP_MENU_PANEL_CENTER);
	printw("\n");
	printw(TOP_MENU_PANEL_DOWN);
	printw("\n");

	for(i = TOP_MENU_LENGTH; i < getmaxx(stdscr); i++) {
		move(top_border - 2, i);
		printw("_");
	}

	for(i=0; i< getmaxy(stdscr) ; i++){
		move(i,0);
		printw("|");
		move(i,getmaxx(stdscr) - 1);
		printw("|");
	}
	
	for(i=1; i< getmaxx(stdscr) - 1; i++){
		move(3, i);
		printw("_");
		move(36, i);
		printw("_");
	}
	
	
	refresh();
}
	

void clear_game_screen() {
        size_t iter;
        for(iter = top_border; iter < bottom_border; iter++) {
                move(iter, 1);
                clrtoeol();
                mvwprintw(stdscr, iter, getmaxx(stdscr) - 1, "%s", "|");
        }
}

	
