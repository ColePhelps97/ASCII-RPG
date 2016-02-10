#include <stdlib.h>
#include <ncurses.h>

typedef unsigned int health_t;
typedef unsigned int mana_t;
typedef unsigned int level_t;
typedef unsigned int experience_t;
typedef unsigned int attack_t;

const int top_border = 4;
int bottom_border = 36;



const char* HEALTH = "Health:";
const char* MANA = "Mana:";
const char* LEVEL = "Level:";

const int TOP_MENU_LENGTH = 79;
const char* TOP_MENU_PANEL_UP =      "|              |           |                 |                 |              |";
const char* TOP_MENU_PANEL_CENTER =  "|  Menu (Esc)  |  Map (M)  |  Inventory (I)  |  Character (C)  |  Skills (K)  |";
const char* TOP_MENU_PANEL_DOWN =    "|______________|___________|_________________|_________________|______________|";


