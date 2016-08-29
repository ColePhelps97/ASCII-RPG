#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "inventory.h"
#include "constants.h"
#include "hero.h"


void equip_weapon(hero_t* hero, weapon_item_t* weapon) {
	if(hero->equipped_weapon != NULL) {
		hero->attack -= hero->equipped_weapon->attack;
		hero->equipped_weapon->is_equipped = 0;
	}
	hero->attack += weapon->attack;
	weapon->is_equipped = 1;
	hero->equipped_weapon = weapon;
}

void equip_armor(hero_t* hero, armor_item_t* armor) {
	if(hero->equipped_armor != NULL) {
		hero->defense -= hero->equipped_armor->defense;
		hero->evasion -= hero->equipped_armor->evasion;
		hero->equipped_armor->is_equipped = 0;
	}
	hero->defense += armor->defense;
	hero->evasion += armor->evasion;
	armor->is_equipped = 1;
	hero->equipped_armor= armor;
}
	
void paint_inventory_panel(WINDOW* window, PANEL* panel) {
    size_t iter;

    for(iter = 1; iter < 23; iter++) {
        mvwprintw(window, iter, 40, "%s", "|");
    }   

    for(iter = 1; iter < 69; iter++) {
        mvwprintw(window, 23, iter, "%s", "_");
        mvwprintw(window, 28, iter, "%s", "_");
        mvwprintw(window, 26, iter, "%s", "-");
    }   

    for(iter = 41; iter < 69; iter++) {
        mvwprintw(window, 8, iter, "%s", "_");
        mvwprintw(window, 15, iter, "%s", "_");
    }

    mvwprintw(window, 4, 44, "%s", WEAPON_INVENTORY);
    mvwprintw(window, 12, 44, "%s", ARMOR_INVENTORY);
    mvwprintw(window, 19, 44, "%s", POITION_INVENTORY);
    mvwprintw(window, 25, 10, "%s", MORE);
    mvwprintw(window, 25, 30, "%s", DROP);
    mvwprintw(window, 25, 50, "%s", SELL);
    mvwprintw(window, 27, 20, "%s", EQUIP);
    mvwprintw(window, 27, 40, "%s", CLOSE);
    mvwprintw(window, 30, 40, "%s", GOLD);
}

void update_inventory_panel(WINDOW* window, hero_t* hero) {
    mvwprintw(window, 5, 52, "%s", "     ");
    mvwprintw(window, 5, 52, "%d/%d", hero->weapon_inventory->used_slots, hero->weapon_inventory->capacity);
    mvwprintw(window, 13, 52, "%s", "     ");
    mvwprintw(window, 13, 52, "%d/%d", hero->armor_inventory->used_slots, hero->armor_inventory->capacity);
	
    mvwprintw(window, 30, 46, "%s", "     ");
    mvwprintw(window, 30, 46, "%d", hero->gold);
    update_panels();
    doupdate();
}

void clear_items(WINDOW* window) {
    size_t iter1, iter2;
    for(iter1 = 1; iter1 < 23; iter1++)
        for(iter2 = 1; iter2 < 40; iter2++)
            mvwprintw(window, iter1, iter2, "%s", " ");
}


void show_weapon_info(weapon_inventory_item_t* item, hero_t* hero);

/* returning values: */
/* 0 - exit inventory panel */
/* 2 - start armor list */
/* 3 - start poition list */
int weapon_list(WINDOW* window, hero_t* hero) {
    char key;
    int exit = 0, list_position = 0, item_position;
	weapon_inventory_t* weapon_inventory;
    weapon_inventory_item_t* item;
    weapon_inventory_item_t* start_item;
    weapon_inventory_item_t* info_item;
    weapon_inventory_item_t* buf;
    size_t counter = 0;

	weapon_inventory = hero->weapon_inventory;
    start_item = weapon_inventory->start_item;
    info_item = weapon_inventory->start_item;
    item_position = 0;
    while(!exit) {
        clear_items(window);
        item = start_item;
        counter = 0;
        while(counter <= 10 && counter <= weapon_inventory->used_slots && item != NULL) {
            mvwprintw(window, 2 + counter * 2, 5, "%s", item->weapon->name);
            if(item->weapon->is_equipped) mvwprintw(window, 2 + counter * 2, 2, "%s", "+");
            item = item->next;
            if(counter == list_position) mvwprintw(window, 2 + counter * 2, 3, "%s", "*");
            counter++;
        }
        update_inventory_panel(window, hero);
        update_panels();
        doupdate();
        key = getch();
        switch(key) {
            case 'c':
                    exit = 1;
                    break;
            case 'j':
                    if(list_position < 10 && list_position < weapon_inventory->used_slots - 1) {
                        list_position++;
                        info_item = info_item->next;
                    }
                    else {
                        if(list_position + item_position < weapon_inventory->used_slots - 1) {
                            start_item = start_item->next;
                            info_item = info_item->next;
                            item_position++;
                        }
                    }
                    break;
            case 'k':
                    if(list_position > 0) {
                        list_position--;
                        info_item = info_item->prev;
                    }
                    else {
                        if(item_position > 0) {
                            start_item = start_item->prev;
                            info_item = info_item->prev;
                            item_position--;
                        }
                    }
                    break;
            case 's':
                    if(info_item)
						hero->gold += info_item->weapon->price;
            case 'd':
                    if(weapon_inventory->used_slots != 0) {
                        if(item_position + 11 == weapon_inventory->used_slots && item_position != 0) {
                            item_position--;
                            buf = info_item->prev;
                            start_item = start_item->prev;
                        }
                        else {
                            if(list_position == 0) start_item = start_item->next;
                            if(info_item->next == NULL) {
                                buf = info_item->prev;
                                list_position--;
                            }
                            else buf = info_item->next;
                        }

						if(info_item->weapon == hero->equipped_weapon) {
							info_item->weapon->is_equipped = 0;
							hero->attack -= hero->equipped_weapon->attack;
							hero->equipped_weapon = NULL;
						}
                        delete_weapon_from_inventory(weapon_inventory, info_item);
                        info_item = buf;
                    }
                    break;
			case 'm':
					if(info_item) 
						show_weapon_info(info_item, hero);
					break;
			case 'a':
					return 2;
			case 'e':
					if(info_item)
						equip_weapon(hero, info_item->weapon);
            default:
                    break;
        }
    }
    return 0;
}


void show_armor_info(armor_inventory_item_t* item, hero_t* hero);

int armor_list(WINDOW* window, hero_t* hero) {
    char key;
    int exit = 0, list_position = 0, item_position;
    armor_inventory_item_t* item;
    armor_inventory_item_t* start_item;
    armor_inventory_item_t* info_item;
    armor_inventory_item_t* buf;
    size_t counter = 0;
	
	armor_inventory_t* armor_inventory = hero->armor_inventory;
    start_item = armor_inventory->start_item;
    info_item = armor_inventory->start_item;
    item_position = 0;
    while(!exit) {
        clear_items(window);
        item = start_item;
        counter = 0;
        while(counter <= 10 && counter <= armor_inventory->used_slots && item != NULL) {
            mvwprintw(window, 2 + counter * 2, 5, "%s", item->armor->name);
            if(item->armor->is_equipped) mvwprintw(window, 2 + counter * 2, 2, "%s", "+");
            item = item->next;
            if(counter == list_position) mvwprintw(window, 2 + counter * 2, 3, "%s", "*");
            counter++;
        }
        update_inventory_panel(window, hero);
        update_panels();
        doupdate();
        key = getch();
        switch(key) {
            case 'c':
                    exit = 1;
                    break;
            case 'j':
                    if(list_position < 10 && list_position < armor_inventory->used_slots - 1) {
                        list_position++;
                        info_item = info_item->next;
                    }
                    else {
                        if(list_position + item_position < armor_inventory->used_slots - 1) {
                            start_item = start_item->next;
                            info_item = info_item->next;
                            item_position++;
                        }
                    }
                    break;
            case 'k':
                    if(list_position > 0) {
                        list_position--;
                        info_item = info_item->prev;
                    }
                    else {
                        if(item_position > 0) {
                            start_item = start_item->prev;
                            info_item = info_item->prev;
                            item_position--;
                        }
                    }
                    break;
            case 's':
                    if(info_item)
						hero->gold += info_item->armor->price;
            case 'd':
                    if(armor_inventory->used_slots != 0) {
                        if(item_position + 11 == armor_inventory->used_slots && item_position != 0) {
                            item_position--;
                            buf = info_item->prev;
                            start_item = start_item->prev;
                        }
                        else {
                            if(list_position == 0) start_item = start_item->next;
                            if(info_item->next == NULL) {
                                buf = info_item->prev;
                                list_position--;
                            }
                            else buf = info_item->next;
                        }


                        delete_armor_from_inventory(armor_inventory, info_item);
                        info_item = buf;
                    }
                    break;
			case 'w':
					return 1;
			case 'm':
					if(info_item)
						show_armor_info(info_item, hero);
					break;
			case 'e':
					if(info_item)
						equip_armor(hero, info_item->armor);
            default:
                    break;
        }
    }
    return 0;
}



void inventory_panel(hero_t* hero, weapon_inventory_t* weapon_inventory) {
    WINDOW* window;
    PANEL* panel;

    int value;

    /*WINDOW* already_equipped_window;*/
    /*PANEL* already_equipped_panel;*/

    /*already_equipped_window = newwin(15, 30, 10, 25);*/
    /*already_equipped_panel = new_panel(already_equipped_window);*/
    /*box(already_equipped_window, 0, 0);*/

    window = newwin(33, 70, 3, 20);
    box(window, 0, 0);
    panel = new_panel(window);
    top_panel(panel);


    paint_inventory_panel(window, panel);
    update_panels();
    doupdate();


    value = weapon_list(window, hero);
	while(value != 0) {
        if(value == 2) value = armor_list(window, hero);
        /*if(value == 3) value = poition_list(window, poition_inventory);*/
        if(value == 1) value = weapon_list(window, hero);
    }

    del_panel(panel);
    delwin(window);
}
void show_weapon_difference(hero_t* hero, weapon_item_t* weapon);

void show_weapon_info(weapon_inventory_item_t* item, hero_t* hero) {
	WINDOW* window;
	PANEL* panel;

	size_t iter;
	int exit = 0;
	char key;
	
	window = newwin(29, 55, 10, 40);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	for(iter = 1; iter < 54; iter++) {
		mvwprintw(window, 17, iter, "%s", "_");
		mvwprintw(window, 22, iter, "%s", "_");
		mvwprintw(window, 3, iter, "%s", "-");
	}

	for(iter = 4; iter < 17; iter++) {
		mvwprintw(window, iter, 16, "%s", "|");
	}
	
	mvwprintw(window, 2, 3, "%s", item->weapon->name);
	mvwprintw(window, 5, 3, "%s", ATTACK);
	mvwprintw(window, 7, 3, "%s", PRICE);
	mvwprintw(window, 5, 11, "%d", item->weapon->attack);
	mvwprintw(window, 7, 10, "%d", item->weapon->price);
	mvwprintw(window, 26, 39, "%s", CLOSE);
	mvwprintw(window, 26, 5, "%s", EQUIP);
	mvwprintw(window, 24, 10, "%s", "Show difference with equipped (D)");

	for(iter = 0; iter < item->weapon->picture_length; iter++) {
		mvwprintw(window, iter + 11 - item->weapon->picture_length/2, 17, "%s", item->weapon->picture[iter]);
	}
	
	for(iter = 0; iter < item->weapon->description_length; iter++) {
		mvwprintw(window, iter + 19, 2, "%s", item->weapon->description[iter]);
	}

	update_panels();
	doupdate();

	while(!exit) {
		key = getch();
		switch(key) {
			case 'c': 
					exit = 1;
					break;
			case 'e':
					if(item) 
						equip_weapon(hero, item->weapon);
					break;
			case 'd':
					show_weapon_difference(hero, item->weapon);
					update_panels();
					doupdate();
					break;
		}
	}

	del_panel(panel);
	delwin(window);
}

void show_weapon_difference(hero_t* hero, weapon_item_t* weapon) {
	WINDOW* window;
	PANEL* panel;
	size_t iter;
	
	window = newwin(20, 50, 3, 50);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);
	
	for(iter = 1; iter < 16; iter++)
		mvwprintw(window, iter, 25, "%s", "|");
	
	for(iter = 1; iter < 49; iter++)
		mvwprintw(window, 16, iter, "%s", "_");

	mvwprintw(window, 2, 3, "%s", THIS_WEAPON);
	mvwprintw(window, 2, 28, "%s", EQUIPPED);
	mvwprintw(window, 5, 3, "%s", ATTACK);
	mvwprintw(window, 5, 28, "%s", ATTACK);
	mvwprintw(window, 7, 3, "%s", PRICE);
	mvwprintw(window, 7, 28, "%s", PRICE);
	mvwprintw(window, 18, 35, "%s", CLOSE);

	if(hero->equipped_weapon) {
		mvwprintw(window, 5, 40, "%d + (%d)", hero->attack - hero->equipped_weapon->attack, hero->equipped_weapon->attack);
		mvwprintw(window, 5, 16, "%d + (%d)", hero->attack - hero->equipped_weapon->attack, weapon->attack);
		mvwprintw(window, 7, 40, "%d", hero->equipped_weapon->price);
	}
	else { 
		mvwprintw(window, 5, 40, "%d", hero->attack);
		mvwprintw(window, 5, 16, "%d + (%d)", hero->attack, weapon->attack);
	}
	
	mvwprintw(window, 7, 16, "%d", weapon->price);
	
	update_panels();
	doupdate();
	
	while(getch() != 'c') {
	}
	del_panel(panel);
	delwin(window);
}

void show_armor_difference(hero_t* hero, armor_item_t* armor);

void show_armor_info(armor_inventory_item_t* item, hero_t* hero) {
	WINDOW* window;
	PANEL* panel;

	size_t iter;
	int exit = 0;
	char key;
	
	window = newwin(29, 55, 10, 40);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	for(iter = 1; iter < 54; iter++) {
		mvwprintw(window, 17, iter, "%s", "_");
		mvwprintw(window, 22, iter, "%s", "_");
		mvwprintw(window, 3, iter, "%s", "-");
	}

	for(iter = 4; iter < 17; iter++) {
		mvwprintw(window, iter, 16, "%s", "|");
	}
	
	mvwprintw(window, 2, 3, "%s", item->armor->name);
	mvwprintw(window, 5, 3, "%s", DEFENSE);
	mvwprintw(window, 7, 3, "%s", EVASION);
	mvwprintw(window, 9, 3, "%s", PRICE);
	mvwprintw(window, 5, 11, "%d", item->armor->defense);
	mvwprintw(window, 7, 11, "%.2f", item->armor->evasion);
	mvwprintw(window, 9, 10, "%d", item->armor->price);
	mvwprintw(window, 26, 39, "%s", CLOSE);
	mvwprintw(window, 26, 5, "%s", EQUIP);
	mvwprintw(window, 24, 10, "%s", "Show difference with equipped (D)");

	for(iter = 0; iter < item->armor->picture_length; iter++) {
		mvwprintw(window, iter + 11 - item->armor->picture_length/2, 17, "%s", item->armor->picture[iter]);
	}
	
	for(iter = 0; iter < item->armor->description_length; iter++) {
		mvwprintw(window, iter + 19, 2, "%s", item->armor->description[iter]);
	}

	update_panels();
	doupdate();

	while(!exit) {
		key = getch();
		switch(key) {
			case 'c': 
					exit = 1;
					break;
			case 'e':
					if(item) 
						equip_armor(hero, item->armor);
					break;
			case 'd':
					show_armor_difference(hero, item->armor);
					update_panels();
					doupdate();
					break;
		}
	}

	del_panel(panel);
	delwin(window);
}

void show_armor_difference(hero_t* hero, armor_item_t* armor) {
	WINDOW* window;
	PANEL* panel;
	size_t iter;
	
	window = newwin(20, 60, 3, 50);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);
	
	for(iter = 1; iter < 16; iter++)
		mvwprintw(window, iter, 30, "%s", "|");
	
	for(iter = 1; iter < 59; iter++) {
		mvwprintw(window, 16, iter, "%s", "_");
		mvwprintw(window, 3, iter, "%s", "-");
	}

	mvwprintw(window, 2, 3, "%s", THIS_WEAPON);
	mvwprintw(window, 2, 33, "%s", EQUIPPED);
	mvwprintw(window, 5, 3, "%s", DEFENSE);
	mvwprintw(window, 5, 33, "%s", DEFENSE);
	mvwprintw(window, 7, 3, "%s", EVASION);
	mvwprintw(window, 7, 33, "%s", EVASION);
	mvwprintw(window, 9, 3, "%s", PRICE);
	mvwprintw(window, 9, 33, "%s", PRICE);
	mvwprintw(window, 18, 40, "%s", CLOSE);

	if(hero->equipped_armor) {
		mvwprintw(window, 5, 45, "%d + (%d)", hero->defense - hero->equipped_armor->defense, hero->equipped_armor->defense);
		mvwprintw(window, 5, 16, "%d + (%d)", hero->defense - hero->equipped_armor->defense, armor->defense);
		mvwprintw(window, 7, 45, "%.2f + (%.2f)", (float)hero->evasion - (float)hero->equipped_armor->evasion, (float)hero->equipped_armor->evasion);
		mvwprintw(window, 7, 16, "%.2f + (%.2f)", hero->evasion- hero->equipped_armor->evasion, armor->evasion);
		mvwprintw(window, 9, 45, "%d", hero->equipped_armor->price);
	}
	else { 
		mvwprintw(window, 5, 45, "%d", hero->defense);
		mvwprintw(window, 5, 16, "%d + (%d)", hero->defense, armor->defense);
		mvwprintw(window, 7, 45, "%.2f", hero->evasion);
		mvwprintw(window, 7, 16, "%.2f + (%.2f)", hero->evasion, armor->evasion);
	}
	
	mvwprintw(window, 9, 16, "%d", armor->price);
	
	update_panels();
	doupdate();
	
	while(getch() != 'c') {
	}
	del_panel(panel);
	delwin(window);
}

