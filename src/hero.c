#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include "constants.h"


const health_t start_health = 50;
const health_t health_per_stat = 10;
const mana_t start_mana = 25;
	
typedef struct stats { 
	stat_t strength;
	stat_t intellect;
	stat_t agility;	
	stat_t unspent_points;
} stats_t;

typedef struct { 
	health_t max_hero_health;
	health_t current_hero_health;
	mana_t max_hero_mana;
       	mana_t current_hero_mana;
	level_t hero_level;
	stat_t strength;
	stat_t intellect;
	stat_t agility;	
	stat_t unspent_points;
	attack_t hero_attack;
	experience_t hero_exp;
/*	stats_t* stats;*/
} hero;



	
hero* init_hero(){
	hero* hero;
	hero = malloc(sizeof(hero));
	/*hero->stats = malloc(sizeof(stats_t));*/
	hero->hero_level = 1;
	hero->max_hero_health = 100;
	hero->current_hero_health = 100;
	hero->max_hero_mana = 100;
	hero->current_hero_mana = 100;
	hero->hero_attack = 1;
	hero->strength = 5;
	hero->intellect = 5;
	hero->agility = 5;
	hero->unspent_points = 10;
	hero->hero_exp = 0;
	return hero;
}

void hero_up(hero* hero) {
	const experience_t level_up_step = 100; 
	while(hero->hero_exp >= level_up_step) {
		hero->hero_exp -= level_up_step; 
		hero->hero_level ++;
		hero->unspent_points++;
	}
}

void update_hero_info(hero* hero) {
	mvwprintw(stdscr, 3, 3, "%s", HEALTH);
	mvwprintw(stdscr, 3, 11, "%u/%u", hero->current_hero_health, hero->max_hero_health);
	mvwprintw(stdscr, 3, 22, "%s", MANA);
	mvwprintw(stdscr, 3, 28, "%u/%u", hero->current_hero_mana, hero->max_hero_mana);
	mvwprintw(stdscr, 3, 38, "%s", LEVEL);
	mvwprintw(stdscr, 3, 45, "%u", hero->hero_level);
}
