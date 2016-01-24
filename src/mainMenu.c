#include <ncurses.h>
#include <stdlib.h>
#include "startGame.c"



void mainMenu(){

/*	VARS HERE */

	int exit = 0;
	int row, col;
	int menuPosition = 0;
	const char* CONTROL_HELP = "Controls: W - up, S - down, E - choose selection";
	const char* NEW_GAME = "New Game";
	const char* OPTIONS = "Options";
	const char* QUIT = "Quit";
	char key = 0;

		
/* 	CODE HERE */

	
	initscr();
	noecho();	/* not show input symbols */
	curs_set(0);	/* kill cursor */
		
	/* color ini */
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	
	getmaxyx(stdscr, row, col);
	

	/* navigation loop */
	while(!exit){
		mvwprintw(stdscr, row-2, 52, "rows: %i, columns: %i",row, col );	
		mvwprintw(stdscr, row-2, 2, "%s", CONTROL_HELP);	
		if(menuPosition == 0) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2 - 1, col/2 - 5, "%s", NEW_GAME);
		attroff(COLOR_PAIR(1));

		if(menuPosition == 1) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2, col/2 - 5, "%s", OPTIONS);
		attroff(COLOR_PAIR(1));

		if(menuPosition == 2) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2 + 1, col/2 - 5, "%s", QUIT);
		attroff(COLOR_PAIR(1));
		
		refresh();
		
		key = getch();
		if((key == 'w') && (menuPosition > 0)) menuPosition--;
		if((key == 's') && (menuPosition < 2)) menuPosition++;
		if((key == 'e') && (menuPosition == 2)) exit = 1;
		if((key == 'e') && (menuPosition == 0)) start();
		
	}
	endwin();
}


int main(){
	mainMenu();
 	return 0;
}
