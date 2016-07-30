#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <unistd.h>
#include <string.h>
#include "hero.h"
#include "level.h"


int start_game(){
	hero_t* my_hero;
	my_hero = init_hero();
	if(!load_level("level1", my_hero)) return 1;
	if(!load_level("level2", my_hero)) return 1;
	if(!load_level("level3", my_hero)) return 1;
	if(!load_level("level4", my_hero)) return 1;
	return 1;	
}

