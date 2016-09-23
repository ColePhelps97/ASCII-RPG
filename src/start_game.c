#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <string.h>
#include "hero.h"
#include "level.h"
#include "inventory.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



int start_game(){
	hero_t* hero;
	hero = init_hero();
	if(!load_level("level1", hero)) return 0;
	if(!load_level("level2", hero)) return 0;
	if(!load_level("level3", hero)) return 0;
	if(!load_level("level4", hero)) return 0;
	free(hero->stats);
	free_weapon_inventory(hero->weapon_inventory);
	free(hero->armor_inventory);
	free(hero);
	return 0;	
}

int start_mult_game(int sock) {
	hero_t* hero;
	hero = init_hero();
	if(!load_mult_level("level1", hero, sock)) return 0;
	if(!load_mult_level("level2", hero, sock)) return 0;
	if(!load_mult_level("level3", hero, sock)) return 0;
	if(!load_mult_level("level4", hero, sock)) return 0;
	free(hero->stats);
	free_weapon_inventory(hero->weapon_inventory);
	free(hero->armor_inventory);
	free(hero);
	return 0;	
}

	
