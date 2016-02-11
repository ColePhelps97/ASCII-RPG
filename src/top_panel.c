#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "hero.h"
#include "constants.h"


void character_panel(hero hero) {
	char key;
	WINDOW* window;
	PANEL* panel;
	int exit = 0;
	window = newwin(20, 40, 3, 40);
	box(window, 0 ,0);
	panel = new_panel(window);
	mvwprintw(window, 1, 2, "%s                    %s %lu", "Player", LEVEL, hero.hero_level);
	mvwprintw(window, 3, 3, "%s          %lu/%lu", HEALTH, hero.current_hero_health, hero.max_hero_health);
	mvwprintw(window, 4, 3, "%s            %lu/%lu", MANA, hero.current_hero_mana, hero.max_hero_mana);
	mvwprintw(window, 5, 3, "%s       %lu/%lu", EXPERIENCE, hero.hero_exp, 100);
	
	update_panels();
	doupdate();
	while(!exit) {
		key = getch();
		switch(key) {
			case 'c': 
				delwin(window);
				del_panel(panel);
				doupdate();
				exit = 1;
				break;
		}
	}
}
		
	
	
