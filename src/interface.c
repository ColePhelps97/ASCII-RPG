#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include "constants.h"


void load_interface(){
	size_t i=0;

	clear();
	
	box(stdscr, 0, 0);
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


void already_max_hp_warning(void) {
    WINDOW* warning_window;
    PANEL* warning_panel;

    warning_window = newwin(10, 20, 10, 20);
    warning_panel = new_panel(warning_window);

    box(warning_window, 0, 0);
    mvwprintw(warning_window, 1, 1, "%s", "You have already maximum health");
    top_panel(warning_panel);

    getch();

    del_panel(warning_panel);
    delwin(warning_window);
}

void already_max_mp_warning(void) {
    WINDOW* warning_window;
    PANEL* warning_panel;

    warning_window = newwin(10, 20, 10, 20);
    warning_panel = new_panel(warning_window);

    box(warning_window, 0, 0);
    mvwprintw(warning_window, 1, 1, "%s", "You have already maximum mana");
    top_panel(warning_panel);

    getch();

    del_panel(warning_panel);
    delwin(warning_window);
}

	
