#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "mystring.h"
#include "hero.h"
#include "constants.h"
#include "interface.h"
#include "enemy.h"
#include "character_panel.h"

/* Updates hero and enemy health and mana bars */

void update_info(hero_t* hero, enemy_t* enemy) {
	move(top_border + 5, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 5, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 5, getmaxx(stdscr) - 14, "%lu/%lu", hero->health, hero->max_health);

	move(top_border + 7, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 7, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 7, getmaxx(stdscr) - 14, "%lu/%lu", hero->mana, hero->max_mana);

	move(top_border + 15, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 15, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 15, getmaxx(stdscr) - 14, "%lu/%lu", enemy->health, enemy->max_health);
	
	move(top_border + 9, getmaxx(stdscr) - 14);
	clrtoeol();
	mvwprintw(stdscr, top_border + 9, getmaxx(stdscr) -1, "%s", "|");
	mvwprintw(stdscr, top_border + 9, getmaxx(stdscr) - 14, "Damage - %lu", hero->attack);
}



void prepare_screen_for_fight(hero_t* hero, enemy_t* enemy) {

	size_t iter;
	clear_game_screen();
	update_info(hero, enemy);

	for(iter = top_border + 1; iter < bottom_border; iter++) {
		mvwprintw(stdscr, iter, getmaxx(stdscr) - 20, "%s", "|");
	}
	
	/* Hero info */
	mvwprintw(stdscr, top_border + 3, getmaxx(stdscr) - 18, "%s", "You:");


	/* Enemy info */
	mvwprintw(stdscr, top_border + 13, getmaxx(stdscr) - 18, "%s:", enemy->name);
	mvwprintw(stdscr, top_border + 17, getmaxx(stdscr) - 14, "Damage - %lu", enemy->attack);

	for(iter = 0; iter < enemy->picture_length; iter++) {
		mvwprintw(stdscr, (bottom_border - top_border - enemy->picture_length)/2 + iter, 10, "%s", enemy->picture[iter]);
	}
	mvwprintw(stdscr, (bottom_border - top_border - enemy->picture_length)/2 + enemy->picture_length + 5, 10, "%s", enemy->welcome_message[0]);
	
	/* 38 line where variants to do in fight printed */
	move(38, 1);
	clrtoeol();

	mvwprintw(stdscr,38, getmaxx(stdscr) - 1, "%s", "|");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 0 + 10, "1.%s", "Attack");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 1 + 10, "2.%s", "Cast fffffireball (50 mana - 6 damage)");
	mvwprintw(stdscr,38, getmaxx(stdscr) / 3 * 2 + 10, "3.%s", "Heal yourself (50 mana - 10hp)");
}

int send_damage(int sock, int damage) {
	char buf[256];
	int recvd_bytes;
	int total_damage = 0;
	char* ATTACK_MSG = "attack";
	if((send(sock, ATTACK_MSG, strlen(ATTACK_MSG), 0)) < 0) {
		perror("send attack");
		return -1;
	}
	sprintf(buf, "%d", (int)damage);
	puts("send attack");
	if((send(sock, (char*)&buf, strlen((char*)&buf), 0)) < 0) {
		perror("send damage");
		return -1;
	}
	if((recvd_bytes = recv(sock, buf, sizeof(buf), 0)) < 0) {
		perror("recv total damage");
		return -1;
	}
	buf[recvd_bytes] = '\0';
	total_damage = strtoul((char*)&buf, NULL, 10);
	return total_damage;
}

/* If enemy not defeated player method returns int by value 1 */
int enemy_base_attack(enemy_t* enemy, hero_t* hero) {
	float damage;
	srand(clock());
	if(!(rand() % 100 < hero->evasion * 100)) {
		if(hero->defense >= enemy->attack)
			damage = 1 / (1 + (hero->defense - enemy->attack) * 0.02);
		else damage = 1 + (enemy->attack - hero->defense) * 0.02;
		damage *= enemy->attack;
		if((int)hero->health - (int)damage >= 0) {
			hero->health -= (int)damage;
			return 1;
		}

		else {
			hero->health = 0; 
			return 0;
		}
	}
	return 1;
}

/* If player not defeated enemy method returns int by value 1 */
int hero_base_attack(hero_t* hero, enemy_t* enemy) {
	float damage;
	srand(clock());
	if(!(rand() % 100 < enemy->evasion * 100)) {
		if(enemy->defense >= hero->attack)
			damage = 1 / (1 + (enemy->defense - hero->attack) * 0.02);
		else damage = 1 + (hero->attack - enemy->defense) * 0.02;
		damage *= hero->attack;
		if((int)enemy->health - (int)damage >= 0) {
			enemy->health -= (int)damage;
			return 1;
		}

		else {
			enemy->health = 0; 
			return 0;
		}
	}
	return 1;

}
int hero_mult_base_attack(hero_t* hero, enemy_t* enemy, int sock) {
	float damage;
	srand(clock());
	if(!(rand() % 100 < enemy->evasion * 100)) {
		if(enemy->defense >= hero->attack)
			damage = 1 / (1 + (enemy->defense - hero->attack) * 0.02);
		else damage = 1 + (hero->attack - enemy->defense) * 0.02;
		damage *= hero->attack;
		if((damage = send_damage(sock, (int)damage)) < 0) return -1;
		if((int)enemy->health - (int)damage >= 0) {
			enemy->health -= (int)damage;
			return 1;
		}

		else {
			enemy->health = 0; 
			return 0;
		}
	}
	return 1;

}
/* If player not defeated enemy method returns int by value 1 */
/* If defeated returns 0 */
/* If player have not enougth mana returns 2 */
int hero_fireball_attack(hero_t* hero, enemy_t* enemy) {
	const attack_t fireball_damage = 6;
	const mana_t fireball_cost = 20;
	if((int)hero->mana - (int)fireball_cost >= 0) {
		if((int)enemy->health - (int)fireball_damage >= 0) {
			enemy->health -= fireball_damage;
			hero->mana -= fireball_cost;		
			return 1;
		}
		else {
			enemy->health = 0;
			return 0;
		}
	}
	else {
		return 0;
	}
}

/* If method returns 1 - selfheal succseed */
/* If method returns 0 - not enought mana points */
int hero_selfheal(hero_t* hero) {
	const health_t selfheal_power = 10;
	const mana_t selfheal_cost = 50;
	if((int)hero->mana - (int)selfheal_cost >= 0) {
		hero->mana -= selfheal_cost;
		if((int)hero->health + (int)selfheal_power <= (int)hero->max_health) {
			hero->health += selfheal_power;	
		}
		else hero->health = hero->max_health;
		return 1;
	}
	else {
		return 0;
	}
}	

	
/* If hero wins fight method returns 1, else returns 0 */
int fight(hero_t* hero, enemy_t* enemy) {
	char key;

	prepare_screen_for_fight(hero, enemy);

	while(hero->health * enemy->health != 0) {
		key = getch();
		switch(key) {
			
			/* Base attack */
			case '1':
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				/* Player's turn */
				hero_base_attack(hero, enemy);
				break;
			
			/* Fireball attack */
			case '2':
				/* Player's turn */
				if(!hero_fireball_attack(hero, enemy)) break;
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				break;
			/* Selfheal */	
			case '3': 
				/* Player's turn */
				if(!hero_selfheal(hero)) break;
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				break;
			
			case 'c':
				character_panel(hero);
				break;

		}
		update_info(hero, enemy);
	}
	/* Take exp and try to lvl up*/
	
	clear_game_screen();
	move(38, 1);
	clrtoeol();
	mvwprintw(stdscr,38, getmaxx(stdscr) - 1, "%s", "|");
	if(hero-> health > 0) {
		hero->exp += enemy->exp;
		hero_up(hero);
		mvwprintw(stdscr, (bottom_border - top_border)/2, getmaxx(stdscr)/2, "%s", "YOU WIN!!!");
		mvwprintw(stdscr, (bottom_border - top_border)/2 + 1, getmaxx(stdscr)/2, "%s", "Press any key to go next level...");
		getch();
		return 1;
	}
	else { 
		mvwprintw(stdscr, (bottom_border - top_border)/2, getmaxx(stdscr)/2, "%s", "GAME OVER LOOSER");
		getch();
		return 0;
	}
}		
				
				
int mult_fight(hero_t* hero, enemy_t* enemy, int sock) {
	char key;

	prepare_screen_for_fight(hero, enemy);

	while(hero->health * enemy->health != 0) {
		key = getch();
		switch(key) {
			
			/* Base attack */
			case '1':
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				/* Player's turn */
				hero_mult_base_attack(hero, enemy, sock);
				break;
			
			/* Fireball attack */
			case '2':
				/* Player's turn */
				if(!hero_fireball_attack(hero, enemy)) break;
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				break;
			/* Selfheal */	
			case '3': 
				/* Player's turn */
				if(!hero_selfheal(hero)) break;
				/* Enemy's turn */
				if(!enemy_base_attack(enemy, hero)) break;
				break;
			
			case 'c':
				character_panel(hero);
				break;

		}
		update_info(hero, enemy);
	}
	/* Take exp and try to lvl up*/
	
	clear_game_screen();
	move(38, 1);
	clrtoeol();
	mvwprintw(stdscr,38, getmaxx(stdscr) - 1, "%s", "|");
	if(hero-> health > 0) {
		hero->exp += enemy->exp;
		hero_up(hero);
		mvwprintw(stdscr, (bottom_border - top_border)/2, getmaxx(stdscr)/2, "%s", "YOU WIN!!!");
		mvwprintw(stdscr, (bottom_border - top_border)/2 + 1, getmaxx(stdscr)/2, "%s", "Press any key to go next level...");
		getch();
		return 1;
	}
	else { 
		mvwprintw(stdscr, (bottom_border - top_border)/2, getmaxx(stdscr)/2, "%s", "GAME OVER LOOSER");
		getch();
		return 0;
	}
}
		
		



















		
