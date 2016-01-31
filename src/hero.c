#include <stdlib.h>
		
 	typedef unsigned int health_t;
	typedef unsigned int mana_t;
	typedef unsigned int level_t;

	
	typedef struct hero { 
		health_t max_hero_health;
		health_t current_hero_health;
		mana_t max_hero_mana;
        	mana_t current_hero_mana;
		level_t hero_level;
	} hero;
	
	hero init_hero(){
		hero hero;
		hero.hero_level = 1;
		hero.max_hero_health = 100;
		hero.current_hero_health = 100;
		hero.max_hero_mana = 100;
     		hero.current_hero_mana = 100;
		return hero;
	}

