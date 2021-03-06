#include <ncurses.h>
#include <stdlib.h>
#include "start_game.h"
#include "multiplayer.h"

void main_menu(){

	int exit = 0;
	int row, col;
	int menu_position = 0;
	const char* CONTROL_HELP = "Controls: K - up, J - down, E - choose selection";
	const char* NEW_GAME = "New Game";
	const char* OPTIONS = "Multiplayer";
	const char* QUIT = "Quit";
	char key = 0;

		
	initscr();
	noecho();	/* not show input symbols */
	curs_set(0);	/* kill cursor */
		
	/* color ini */
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	
	getmaxyx(stdscr, row, col);
	

	while(!exit){
		mvwprintw(stdscr, row-2, 2, "%s", CONTROL_HELP);	
		if(menu_position == 0) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2 - 1, col/2 - 5, "%s", NEW_GAME);
		attroff(COLOR_PAIR(1));

		if(menu_position == 1) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2, col/2 - 5, "%s", OPTIONS);
		attroff(COLOR_PAIR(1));

		if(menu_position == 2) attron(COLOR_PAIR(1));
		mvwprintw(stdscr, row/2 + 1, col/2 - 5, "%s", QUIT);
		attroff(COLOR_PAIR(1));
		
		refresh();
		
		key = getch();
		if((key == 'k') && (menu_position > 0)) menu_position--;
		if((key == 'j') && (menu_position < 2)) menu_position++;
		if((key == 'e') && (menu_position == 2)) exit = 1;
		if((key == 'e') && (menu_position == 0)) exit = start_game();
		if((key == 'e') && (menu_position == 1)) exit = mp_menu();
		
	}
	endwin();
}


int main(){
	main_menu();
 	return 0;
}
