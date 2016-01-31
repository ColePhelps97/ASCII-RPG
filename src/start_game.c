#include "main_interface.h"
#include <stdlib.h>
#include "hero.h"
#include "load_level.h"




void start(){
	hero my_hero;
	my_hero = init_hero();
	load_level("../levels/level1.level", my_hero);

}
