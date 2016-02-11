#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "hero.h"
#include "constants.h"

void spend_points(hero hero);

void character_panel(hero hero) {
	char key;
	size_t iter;
	WINDOW* window;
	PANEL* panel;
	int exit = 0; 
	window = newwin(20, 40, 3, 40);
	box(window, 0 ,0);
	panel = new_panel(window);

	for(iter = 1; iter < getmaxx(window) - 1; iter++) {
		mvwprintw(window, 2, iter, "%s", "_");
		mvwprintw(window, 7, iter, "%s", "_");
		mvwprintw(window, 16, iter, "%s", "_");
	}
	

	mvwprintw(window, 1, 2, "%s                    %s %lu", "Player", LEVEL, hero.hero_level);
	mvwprintw(window, 4, 3, "%s          %lu/%lu", HEALTH, hero.current_hero_health, hero.max_hero_health);
	mvwprintw(window, 5, 3, "%s            %lu/%lu", MANA, hero.current_hero_mana, hero.max_hero_mana);
	mvwprintw(window, 6, 3, "%s       %lu/%lu", EXPERIENCE, hero.hero_exp, 100);
	mvwprintw(window, 9, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero.stats.unspent_points);
	mvwprintw(window, 11, 3, "%s               %lu", STRENGTH, hero.stats.strength);
	mvwprintw(window, 12, 3, "%s              %lu", INTELLECT, hero.stats.intellect);
	mvwprintw(window, 13, 3, "%s                %lu", AGILITY, hero.stats.agility);
	mvwprintw(window, 17, 22, "%s", SPEND_POINTS);
	
	
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
			
			case 's':
				spend_points(hero);
				top_panel(panel);
				update_panels();
				doupdate();
				break;
					
		}
	}
}
		
	
void paint_spend_window(WINDOW* spend_window, PANEL* spend_panel, hero hero) {
	size_t iter1, iter2;



	for(iter1 = 1; iter1 < getmaxx(spend_window) - 1; iter1++)
		for(iter2 = 1; iter2 < getmaxy(spend_window) - 1; iter2++)
			mvwprintw(spend_window, iter2, iter1, " ");


	mvwprintw(spend_window, 2, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero.stats.unspent_points);
        mvwprintw(spend_window, 4, 3, "%s               %lu", STRENGTH, hero.stats.strength);
        mvwprintw(spend_window, 5, 3, "%s              %lu", INTELLECT, hero.stats.intellect);
        mvwprintw(spend_window, 6, 3, "%s                %lu", AGILITY, hero.stats.agility);

	top_panel(spend_panel);
	update_panels();
	doupdate();

}	


void have_no_points() {
	WINDOW* window;
	PANEL* panel;
	
	window = newwin(10, 7, (getmaxx(stdscr) - 50) / 2, 10);
	box(window, 0, 0);

	panel = new_panel(window);
	
	mvwprintw(window, 1, 3, "%s", "You have 0 points");
	
	top_panel(panel);
	update_panels();
	doupdate();
	
	getch();
	
	delwin(window);
	del_panel(panel);	
	doupdate();
}

void spend_points(hero hero) {
	PANEL* spend_panel;
	WINDOW* spend_window;
	int end_of_spend = 0;
	char key;
	stat_t points , strength_added = 0, intellect_added = 0, agility_added = 0;
	points = hero.stats.unspent_points;
	
	
	spend_window = newwin(12, 38, 8, 41);
	spend_panel = new_panel(spend_window);
	box(spend_window, 0, 0);

	paint_spend_window(spend_window, spend_panel, hero);

	while(!end_of_spend) {
		if(hero.stats.unspent_points > 0) {
			key = getch();
			switch(key) {
			
				case 's':
					hero.stats.strength ++;
					hero.stats.unspent_points--;				
					strength_added ++;
					paint_spend_window(spend_window, spend_panel, hero);
					
					break;
	
				case 'a':
					hero.stats.agility ++;
					hero.stats.unspent_points--;				
					agility_added++;
					paint_spend_window(spend_window, spend_panel, hero);
					
					break;
	
				case 'i':
					hero.stats.intellect ++;
					hero.stats.unspent_points--;				
					intellect_added++;
					paint_spend_window(spend_window, spend_panel, hero);
					
					break;
				
				case 'o':
					end_of_spend = 1;
					break;
				
				case 'c':
					hero.stats.unspent_points = points;
					hero.stats.strength -= strength_added;
					hero.stats.intellect -= intellect_added;
					hero.stats.agility -= agility_added;
					end_of_spend = 1;
					break;
						
			}
		}
		else {		
			have_no_points();
		}

	}
	delwin(spend_window);
	del_panel(spend_panel);
	doupdate();
}









