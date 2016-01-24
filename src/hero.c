#include <stdlib.h>
		
 	typedef unsigned int health_t;
	typedef unsigned int mana_t;
	typedef unsigned int level_t;

	
	struct hero { 
		health_t maxHeroHealth;
		health_t currentHeroHealth;
		mana_t maxHeroMana;
        	mana_t currentHeroMana;
		level_t heroLevel;
	} hero;
	
	void initHero(){
		hero.heroLevel = 1;
		hero.maxHeroHealth = 100;
		hero.currentHeroHealth = 100;
		hero.maxHeroMana = 100;
     		hero.currentHeroMana = 100;
	}

