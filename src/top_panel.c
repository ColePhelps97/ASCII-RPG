#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "hero.h"
#include "constants.h"

void spend_points(WINDOW* spend_window, PANEL* spend_panel, WINDOW* zero_points_window, PANEL* zero_points_panel, WINDOW* info_window, PANEL* info_panel, hero_t* hero);

void paint_character_window(WINDOW* window, PANEL* panel, hero_t* hero); 

void character_panel(hero_t* hero) {
	char key;
	int exit = 0; 
	

	WINDOW* window;
	PANEL* panel;

	PANEL* spend_panel;
	WINDOW* spend_window;

	WINDOW* zero_points_window;
	PANEL* zero_points_panel;

	WINDOW* info_window;
	PANEL* info_panel;

	zero_points_window = newwin(10, 60, 20, 51);
	zero_points_panel = new_panel(zero_points_window);

	info_window = newwin(10, 60, 20, 35);
	info_panel = new_panel(info_window);
	
	spend_window = newwin(12, 38, 8, 79);
	spend_panel = new_panel(spend_window);

	hide_panel(spend_panel);
	hide_panel(zero_points_panel);
	hide_panel(info_panel);

	window = newwin(20, 40, 3, 40);
	box(window, 0 ,0);
	panel = new_panel(window);

	paint_character_window(window, panel, hero);
	
	update_panels();
	doupdate();
	while(!exit) {
		key = getch();
		switch(key) {
			case 'c': 
				exit = 1;
				break;
			
			case 's':
				spend_points(spend_window, spend_panel,
							zero_points_window, zero_points_panel,
							info_window, info_panel, hero);
				paint_character_window(window, panel, hero);
				top_panel(panel);
				break;
					
		}
	}

	/*hide_panel(panel);*/
	del_panel(zero_points_panel);	
	delwin(zero_points_window);

	del_panel(panel);
	delwin(window);

	del_panel(spend_panel);
	delwin(spend_window);

	update_panels();
	doupdate();
	
}

void paint_character_window(WINDOW* window, PANEL* panel, hero_t* hero) {
	size_t iter;

	for(iter = 1; iter < getmaxx(window) - 1; iter++) {
		mvwprintw(window, 2, iter, "%s", "_");
		mvwprintw(window, 7, iter, "%s", "_");
		mvwprintw(window, 16, iter, "%s", "_");
	}
	

	mvwprintw(window, 1, 2, "%s                    %s %lu", "Player", LEVEL, hero->level);
	mvwprintw(window, 4, 3, "%s          %lu/%lu", HEALTH, hero->health, hero->max_health);
	mvwprintw(window, 5, 3, "%s            %lu/%lu", MANA, hero->mana, hero->max_mana);
	mvwprintw(window, 6, 3, "%s       %lu/%lu", EXPERIENCE, hero->exp, 100);
	mvwprintw(window, 9, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero->stats->unspent_points);
	mvwprintw(window, 11, 3, "%s               %lu", STRENGTH, hero->stats->strength);
	mvwprintw(window, 12, 3, "%s              %lu", INTELLECT, hero->stats->intellect);
	mvwprintw(window, 13, 3, "%s                %lu", AGILITY, hero->stats->agility);
	mvwprintw(window, 17, 2, "%s", SPEND_POINTS);
	mvwprintw(window, 17, 27, "%s", CLOSE);
	
	update_panels();
	doupdate();
	/*refresh();*/

}
	
		
	
void paint_spend_window(WINDOW* spend_window, PANEL* spend_panel, hero_t* hero) {
	size_t iter1, iter2;

	box(spend_window, 0, 0);
	for(iter1 = 1; iter1 < getmaxx(spend_window) - 1; iter1++) {
		for(iter2 = 1; iter2 < getmaxy(spend_window) - 1; iter2++)
			mvwprintw(spend_window, iter2, iter1, " ");
		mvwprintw(spend_window, 2, iter1, "%s", "_");
		mvwprintw(spend_window, 7, iter1, "%s", "_");
		mvwprintw(spend_window, 9, iter1, "%s", "_");
	}

	
	mvwprintw(spend_window, 1, 3, "%s        %s%lu", STATS,UNSPENT_POINTS, hero->stats->unspent_points);
	mvwprintw(spend_window, 4, 3, "%s               %lu", STRENGTH, hero->stats->strength);
	mvwprintw(spend_window, 5, 3, "%s              %lu", INTELLECT, hero->stats->intellect);
	mvwprintw(spend_window, 6, 3, "%s                %lu", AGILITY, hero->stats->agility);
	mvwprintw(spend_window, 8, 5, "%s", OK_MESSAGE);
	mvwprintw(spend_window, 8, 22, "%s", CANCEL);
	mvwprintw(spend_window, 10, 15, "%s", HELP);

	

	top_panel(spend_panel);
	update_panels();
	doupdate();

}	


void have_no_points(WINDOW* zero_points_window, PANEL* zero_points_panel) {
	
	box(zero_points_window, 0, 0);

	
	mvwprintw(zero_points_window, 3, 20, "%s", "You have 0 points");
	mvwprintw(zero_points_window, 5, 16, "%s", "Press any key to continue");
	show_panel(zero_points_panel);
	top_panel(zero_points_panel);
	update_panels();
	doupdate();
	
	getch();
	
	hide_panel(zero_points_panel);
	update_panels();
	doupdate();
	
}

void show_info_window(WINDOW* info_window, PANEL* info_panel) {
	
	box(info_window, 0, 0);	
	mvwprintw(info_window, 3, 4, "%s", "S - strength, A - agility, I - intellect");
	show_panel(info_panel);
	top_panel(info_panel);
	update_panels();
	doupdate();

	getch();
	
	hide_panel(info_panel);
	update_panels();
	doupdate();
}

void spend_points(WINDOW* spend_window, PANEL* spend_panel, WINDOW* zero_points_window,
				PANEL* zero_points_panel, WINDOW* info_window,
				PANEL* info_panel, hero_t* hero) {
	int end_of_spend = 0;
	char key;
	stat_t points , strength_added = 0, intellect_added = 0, agility_added = 0;
	points = hero->stats->unspent_points;
		
	
	show_panel(spend_panel);

	paint_spend_window(spend_window, spend_panel, hero);
		

	while(!end_of_spend) {
		key = getch();
		switch(key) {
			case 's':
				
				if(hero->stats->unspent_points > 0) {
					hero->stats->strength ++;
					hero->stats->unspent_points--;
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

				if(hero->stats->unspent_points > 0) {
					hero->stats->agility ++;
					hero->stats->unspent_points--;	
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

				if(hero->stats->unspent_points > 0) {
				hero->stats->intellect ++;
				hero->stats->unspent_points--;
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
				hero_update_stats(hero);
				break;
			
			case 'c':
				hero->stats->unspent_points = points;
				hero->stats->strength -= strength_added;
				hero->stats->intellect -= intellect_added;
				hero->stats->agility -= agility_added;
				end_of_spend = 1;
				break;

			case 'h':
				show_info_window(info_window, info_panel);
				break;
					
		}
	}
	hide_panel(spend_panel);
	update_panels();
	doupdate();
}

