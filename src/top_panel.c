#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "hero.h"
#include "constants.h"

void spend_points(WINDOW* spend_window, PANEL* spend_panel, WINDOW* zero_points_window, PANEL* zero_points_panel, hero* hero);

void character_panel(hero* hero) {
	char key;
	size_t iter;
	int exit = 0; 

	WINDOW* window;
	PANEL* panel;

	PANEL* spend_panel;
	WINDOW* spend_window;

	WINDOW* zero_points_window;
	PANEL* zero_points_panel;


	zero_points_window = newwin(10, 60, 10, 35);
	zero_points_panel = new_panel(zero_points_window);

	
	spend_window = newwin(12, 38, 8, 41);
	spend_panel = new_panel(spend_window);

	hide_panel(spend_panel);
	hide_panel(zero_points_panel);

	window = newwin(20, 40, 3, 40);
	box(window, 0 ,0);
	panel = new_panel(window);

	for(iter = 1; iter < getmaxx(window) - 1; iter++) {
		mvwprintw(window, 2, iter, "%s", "_");
		mvwprintw(window, 7, iter, "%s", "_");
		mvwprintw(window, 16, iter, "%s", "_");
	}
	

	mvwprintw(window, 1, 2, "%s                    %s %lu", "Player", LEVEL, hero->hero_level);
	mvwprintw(window, 4, 3, "%s          %lu/%lu", HEALTH, hero->current_hero_health, hero->max_hero_health);
	mvwprintw(window, 5, 3, "%s            %lu/%lu", MANA, hero->current_hero_mana, hero->max_hero_mana);
	mvwprintw(window, 6, 3, "%s       %lu/%lu", EXPERIENCE, hero->hero_exp, 100);
	mvwprintw(window, 9, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero->unspent_points);
	mvwprintw(window, 11, 3, "%s               %lu", STRENGTH, hero->strength);
	mvwprintw(window, 12, 3, "%s              %lu", INTELLECT, hero->intellect);
	mvwprintw(window, 13, 3, "%s                %lu", AGILITY, hero->agility);
	mvwprintw(window, 17, 22, "%s", SPEND_POINTS);
	
	
	update_panels();
	doupdate();
	while(!exit) {
		key = getch();
		switch(key) {
			case 'c': 
				exit = 1;
				break;
			
			case 's':
				spend_points(spend_window, spend_panel, zero_points_window, zero_points_panel, hero);
				top_panel(panel);
				break;
					
		}
	}


	
	hide_panel(panel);
	/*delwin(zero_points_window);*/
	/*del_panel(zero_points_panel);	*/

	/*delwin(window);*/
	/*del_panel(panel);*/

	/*delwin(spend_window);*/
	/*del_panel(spend_panel);*/

	update_panels();
	doupdate();
	
}
		
	
void paint_spend_window(WINDOW* spend_window, PANEL* spend_panel, hero* hero) {
	size_t iter1, iter2;

	box(spend_window, 0, 0);
	for(iter1 = 1; iter1 < getmaxx(spend_window) - 1; iter1++)
		for(iter2 = 1; iter2 < getmaxy(spend_window) - 1; iter2++)
			mvwprintw(spend_window, iter2, iter1, " ");


	mvwprintw(spend_window, 2, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero->unspent_points);
        mvwprintw(spend_window, 4, 3, "%s               %lu", STRENGTH, hero->strength);
        mvwprintw(spend_window, 5, 3, "%s              %lu", INTELLECT, hero->intellect);
        mvwprintw(spend_window, 6, 3, "%s                %lu", AGILITY, hero->agility);

	top_panel(spend_panel);
	update_panels();
	doupdate();

}	


void have_no_points(WINDOW* zero_points_window, PANEL* zero_points_panel) {
	
	box(zero_points_window, 0, 0);

	
	mvwprintw(zero_points_window, 1, 3, "%s", "You have 0 points");
	show_panel(zero_points_panel);
	top_panel(zero_points_panel);
	update_panels();
	doupdate();
	
	getch();
	
	hide_panel(zero_points_panel);
	update_panels();
	doupdate();
	
}

void spend_points(WINDOW* spend_window, PANEL* spend_panel, WINDOW* zero_points_window, PANEL* zero_points_panel, hero* hero) {
	int end_of_spend = 0;
	char key;
	stat_t points , strength_added = 0, intellect_added = 0, agility_added = 0;
	points = hero->unspent_points;
		
	
	show_panel(spend_panel);

	paint_spend_window(spend_window, spend_panel, hero);
		

	while(!end_of_spend) {
		key = getch();
		switch(key) {
			case 's':
				
				if(hero->unspent_points > 0) {
					hero->strength ++;
					hero->unspent_points--;				
					strength_added ++;
					paint_spend_window(spend_window, spend_panel, hero);
				}
				
				else {		
					have_no_points(zero_points_window, zero_points_panel);
					update_panels();
					doupdate();
				}
	
				break;
	
			case 'a':

				if(hero->unspent_points > 0) {
					hero->agility ++;
					hero->unspent_points--;				
					agility_added++;
					paint_spend_window(spend_window, spend_panel, hero);
				}
	
				else {		
					have_no_points(zero_points_window, zero_points_panel);
					update_panels();
					doupdate();
				}
					
				break;
	
			case 'i':

				if(hero->unspent_points > 0) {
					hero->intellect ++;
					hero->unspent_points--;				
					intellect_added++;
					paint_spend_window(spend_window, spend_panel, hero);
				}
	
				else {		
					have_no_points(zero_points_window, zero_points_panel);
					update_panels();
					doupdate();
				}
					
				break;
				
			case 'o':
				end_of_spend = 1;
				break;
			
			case 'c':
				hero->unspent_points = points;
				hero->strength -= strength_added;
				hero->intellect -= intellect_added;
				hero->agility -= agility_added;
				end_of_spend = 1;
				break;
					
		}
	}
	hide_panel(spend_panel);
	update_panels();
	doupdate();
}









