#include <stdlib.h>
#include <ncurses.h>
#include "hero.h"
#include "constants.h"
#include "interface.h"
#include "enemy.h"







void update_info(hero hero, enemy enemy) {
	move(top_border + 5, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 5, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 5, getmaxx(stdscr) - 14, "%lu/%lu", hero.current_hero_health, hero.max_hero_health);

	move(top_border + 7, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 7, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 7, getmaxx(stdscr) - 14, "%lu/%lu", hero.current_hero_mana, hero.max_hero_mana);

	move(top_border + 15, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 15, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 15, getmaxx(stdscr) - 14, "%lu/%lu", enemy.current_enemy_health, enemy.max_enemy_health);
	
}



void prepare_screen_for_fight(hero hero, enemy enemy) {

	size_t iter;
	clear_game_screen();
	update_info(hero, enemy);

	for(iter = top_border + 1; iter < bottom_border; iter++) {
		mvwprintw(stdscr, iter, getmaxx(stdscr) - 20, "%s", "|");
	}
	
	/* Hero info */
	mvwprintw(stdscr, top_border + 3, getmaxx(stdscr) - 18, "%s", "You:");
	mvwprintw(stdscr, top_border + 9, getmaxx(stdscr) - 14, "Damage - %lu", hero.hero_attack);


	/* Enemy info */
	mvwprintw(stdscr, top_border + 13, getmaxx(stdscr) - 18, "%s:", enemy.enemy_name);
	mvwprintw(stdscr, top_border + 17, getmaxx(stdscr) - 14, "Damage - %lu", enemy.enemy_attack);

	for(iter = 0; iter < enemy.picture_length; iter++) {
		mvwprintw(stdscr, (bottom_border - top_border - enemy.picture_length)/2 + iter, 10, "%s", enemy.enemy_picture[iter]);
	}
	mvwprintw(stdscr, (bottom_border - top_border - enemy.picture_length)/2 + enemy.picture_length + 5, 10, "%s", enemy.enemy_welcome_message[0]);
	move(38, 1);
	clrtoeol();
	mvwprintw(stdscr,38, getmaxx(stdscr) - 1, "%s", "|");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 0 + 10, "1.%s", "Attack");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 1 + 10, "2.%s", "Cast fffffireball (50 mana - 6 damage)");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 2 + 10, "3.%s", "Heal yourself (50 mana - 10hp)");
}






	
void fight(hero hero, enemy enemy) {
	
	char key;
	attack_t fireball = 6;
	health_t heal = 10;

	prepare_screen_for_fight(hero, enemy);

	while(hero.current_hero_health * enemy.current_enemy_health != 0) {
		key = getch();
		switch(key) {
			case '1':
				if((int)hero.current_hero_health - (int)enemy.enemy_attack >= 0) hero.current_hero_health -= enemy.enemy_attack;
				else { hero.current_hero_health = 0; break;}
				if((int)enemy.current_enemy_health - (int)hero.hero_attack >= 0) enemy.current_enemy_health -= hero.hero_attack;
				else { enemy.current_enemy_health = 0; }
				break;
			
			case '2':
				if((int)hero.current_hero_health - (int)enemy.enemy_attack >= 0) hero.current_hero_health -= enemy.enemy_attack;
				else { hero.current_hero_health = 0; break;}
				if((int)enemy.current_enemy_health - (int)fireball >= 0) {
					enemy.current_enemy_health -= fireball;
					hero.current_hero_mana -= 50;
				}
				else { enemy.current_enemy_health = 0; }
				break;
			
			case '3': 
				if((int)hero.current_hero_health - (int)enemy.enemy_attack >= 0) hero.current_hero_health -= enemy.enemy_attack;
				else { hero.current_hero_health = 0; break; }
				hero.current_hero_mana -= 50;
				if(hero.current_hero_health + heal <= hero.max_hero_health) hero.current_hero_health += heal;
				else hero.current_hero_health = hero.max_hero_health;
				break;

		}
		update_info(hero, enemy);
		
	}
	clear_game_screen();
	move(38, 1);
	clrtoeol();
	mvwprintw(stdscr,38, getmaxx(stdscr) - 1, "%s", "|");
	mvwprintw(stdscr, (bottom_border - top_border)/2, getmaxx(stdscr)/2, "%s", "YOU WIN!!!");
	mvwprintw(stdscr, (bottom_border - top_border)/2 + 1, getmaxx(stdscr)/2, "%s", "Press any key to go next level...");
}		
				
				
				
		
		



















		
