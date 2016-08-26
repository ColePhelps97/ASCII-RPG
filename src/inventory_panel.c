#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include "inventory.h"
#include "constants.h"
#include "hero.h"


void sell_weapon(hero_t* hero, weapon_inventory_t* inventory, weapon_inventory_item_t* item) {
    hero->gold += item->weapon->price;
    delete_weapon_from_inventory(inventory, item);
}

void paint_inventory_panel(WINDOW* window, PANEL* panel, weapon_inventory_t* weapon_inventory) {
    size_t iter;

    for(iter = 1; iter < 23; iter++) {
        mvwprintw(window, iter, 40, "%s", "|");
    }   

    for(iter = 1; iter < 69; iter++) {
        mvwprintw(window, 23, iter, "%s", "_");
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
}

void update_used_slots(WINDOW* window, hero_t* hero) {
    mvwprintw(window, 5, 52, "%s", "     ");
    mvwprintw(window, 5, 52, "%d/%d", hero->weapon_inventory->used_slots, hero->weapon_inventory->capacity);
    mvwprintw(window, 13, 52, "%s", "     ");
    mvwprintw(window, 13, 52, "%d/%d", hero->armor_inventory->used_slots, hero->armor_inventory->capacity);
    update_panels();
    doupdate();
}

void clear_items(WINDOW* window) {
    size_t iter1, iter2;
    for(iter1 = 1; iter1 < 23; iter1++)
        for(iter2 = 1; iter2 < 40; iter2++)
            mvwprintw(window, iter1, iter2, "%s", " ");
}


void show_info(weapon_inventory_item_t* item);

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
            mvwprintw(window, 2 + counter * 2, 4, "%s", item->weapon->name);
            item = item->next;
            if(counter == list_position) mvwprintw(window, 2 + counter * 2, 2, "%s", "*");
            counter++;
        }
        update_used_slots(window, hero);
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


                        delete_weapon_from_inventory(weapon_inventory, info_item);
                        info_item = buf;
                    }
                    break;
			case 'm':
					if(info_item != NULL) show_info(info_item);
					break;
			case 'a':
					return 2;
            default:
                    break;
        }
    }
    return 0;
}

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
            mvwprintw(window, 2 + counter * 2, 4, "%s", item->armor->name);
            item = item->next;
            if(counter == list_position) mvwprintw(window, 2 + counter * 2, 2, "%s", "*");
            counter++;
        }
        update_used_slots(window, hero);
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

    /*WINDOW* already_equiped_window;*/
    /*PANEL* already_equiped_panel;*/

    /*already_equiped_window = newwin(15, 30, 10, 25);*/
    /*already_equiped_panel = new_panel(already_equiped_window);*/
    /*box(already_equiped_window, 0, 0);*/

    window = newwin(30, 70, 3, 20);
    box(window, 0, 0);
    panel = new_panel(window);
    top_panel(panel);

    paint_inventory_panel(window, panel, weapon_inventory);
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

void show_info(weapon_inventory_item_t* item) {
	WINDOW* window;
	PANEL* panel;

	size_t iter;
	
	window = newwin(22, 55, 20, 40);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	for(iter = 1; iter < 54; iter++) {
		mvwprintw(window, 17, iter, "%s", "_");
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

	

	update_panels();
	doupdate();

	getch();

	del_panel(panel);
	delwin(window);
}








