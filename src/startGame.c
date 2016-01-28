#include "mainInterface.h"
#include <stdlib.h>
#include "hero.h"
#include "loadLevel.h"




void start(){
	hero myHero;
	myHero = initHero();
	loadInterface(myHero);
	loadLevel("../levels/level1.level");

}
