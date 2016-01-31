#include "main_interface.h"
#include <stdlib.h>
#include "hero.h"
#include "load_level.h"
#include "mystring.h"




void start(){
	hero my_hero;
	my_hero = init_hero();
	load_level("level1", my_hero);
	load_level("level2", my_hero);
	
	
	

}
