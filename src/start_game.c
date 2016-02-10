#include <stdlib.h>
#include "hero.h"
#include "level.h"


int start(){
	hero my_hero;
	my_hero = init_hero();
	load_level("level1", my_hero);
	load_level("level2", my_hero);
	load_level("level3", my_hero);
	load_level("level4", my_hero);
	return 1;	
}
