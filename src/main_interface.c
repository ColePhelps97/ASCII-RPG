#include <ncurses.h>
#include <stdlib.h>
#include "hero.h"


void load_interface(hero hero){
	size_t i=0;
	const char* MENU_PANEL_UP =      "|              |           |                 |                 |              |";
	const char* MENU_PANEL_CENTER =  "|  Menu (Esc)  |  Map (M)  |  Inventory (I)  |  Character (C)  |  Skills (K)  |";
	const char* MENU_PANEL_DOWN =    "|______________|___________|_________________|_________________|______________|________________________________________________________________________________________";
/*	const char* HEALTH = "Health:";*/	
/*	const char* MANA = "Mana:";*/
/*	const char* LEVEL = "Level:";*/

	clear();
	move(0,0);
	printw(MENU_PANEL_UP);
	printw("\n");
	printw(MENU_PANEL_CENTER);
	printw("\n");
	printw(MENU_PANEL_DOWN);
	printw("\n");
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
	
/*	mvwprintw(stdscr, 3, 3, "%s", HEALTH);*/
/*	mvwprintw(stdscr, 3, 11, "%u/%u", hero.current_hero_health, hero.max_hero_health);*/
/*	mvwprintw(stdscr, 3, 22, "%s", MANA);*/
/*	mvwprintw(stdscr, 3, 28, "%u/%u", hero.current_hero_mana, hero.max_hero_mana);*/
/*	mvwprintw(stdscr, 3, 38, "%s", LEVEL);*/
/*	mvwprintw(stdscr, 3, 45, "%u", hero.hero_level);*/
	
	refresh();
}
	


	

