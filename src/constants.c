#include <stdlib.h>

typedef unsigned int health_t;
typedef unsigned int mana_t;
typedef unsigned int person_level_t;
typedef unsigned int experience_t;
typedef unsigned int attack_t;
typedef unsigned int stat_t;
typedef unsigned int gold_t;
typedef unsigned int defense_t;
typedef float evasion_t;

const int top_border = 4;
int bottom_border = 36;



const char* HEALTH = "Health:";
const char* MANA = "Mana:";
const char* LEVEL = "Level:";
const char* UNSPENT_POINTS = "Unspent points:";
const char* EXPERIENCE = "Experience:";
const char* STATS = "STATS";
const char* STRENGTH = "Strength:";
const char* INTELLECT = "Intellect:";
const char* AGILITY = "Agility:";
const char* SPEND_POINTS = "Spend points (S)";
const char* SAVE_POINTS = "Save points (S)";
const char* HELP = "Help (H)";
const char* CANCEL = "Cancel (C)";
const char* OK_MESSAGE = "Ok (O)";
const char* CLOSE = "Close (C)";
const char* WEAPON_INVENTORY = "Weapon inventory (W)";
const char* ARMOR_INVENTORY = "Armor inventory (A)";
const char* POITION_INVENTORY = "Poition inventory (P)";
const char* SELL = "Sell (S)";
const char* DROP = "Drop (D)";
const char* EQUIP = "Equip (E)";
const char* USE = "Use (U)";
const char* MORE = "More (M)";

const int TOP_MENU_LENGTH = 79;
const char* TOP_MENU_PANEL_UP =      "|              |           |                 |                 |              |";
const char* TOP_MENU_PANEL_CENTER =  "|  Menu (Esc)  |  Map (M)  |  Inventory (I)  |  Character (C)  |  Skills (K)  |";
const char* TOP_MENU_PANEL_DOWN =    "|______________|___________|_________________|_________________|______________|";


