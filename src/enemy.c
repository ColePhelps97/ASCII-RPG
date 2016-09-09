#include <stdlib.h>
#include <stdio.h>
#include "read_file.h"
#include "mystring.h"
#include "constants.h"



typedef struct {
	char* name;
	char** welcome_message;
	char** picture;
	size_t wm_length;
	size_t picture_length;
	health_t max_health;
	health_t health;
	evasion_t evasion;
	defense_t defense;
	experience_t exp;
	attack_t attack;
} enemy_t;




/* Enemy file struct                          */
/* -------------------------------------------*/
/* enemy_name                                 */
/* number of rows in enemy welcome_message    */
/* enemy_welcome_message                      */
/* number of rows in enemy ASCII picture      */
/* enemy ascii picture                        */
/* enemy max health                           */
/* enemy cur health                           */
/* enemy evasion							  */
/* enemy defense							  */
/* enemy attack                               */
/* enemy experience                           */

enemy_t* parse_enemy(char* enemy_name) {
	enemy_t* enemy; 
	char** text;
	char* end;
	size_t cur_parsing_row, iter;
	FILE* file = fopen(concat("../enemies/", enemy_name, ".enemy"), "r");
	enemy = (enemy_t*) malloc(sizeof(enemy_t));

	text = read_file(file);
	cur_parsing_row = 0;


	/* enemy name */
	enemy->name = text[cur_parsing_row];
	cur_parsing_row++;

	/* welcome message length  */
	enemy->wm_length = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;

	/* Welcome message */
	enemy->welcome_message = (char**) malloc(sizeof(char*) * enemy->wm_length);
	for( iter = 0; iter < enemy->wm_length; iter++) {
			enemy->welcome_message[iter] = text[cur_parsing_row];
			cur_parsing_row++;
	}


	/* Picture length  */
	enemy->picture_length = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;

	/* Picture */
	enemy->picture = (char**) malloc(sizeof(char*) * enemy->picture_length);
	for(iter = 0; iter< enemy->picture_length; iter++) {
			enemy->picture[iter] = text[cur_parsing_row];
			cur_parsing_row++;
	}

	/* Max health */
	enemy->max_health = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;


	/* Cur health */
	enemy->health = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;

	enemy->evasion = atof(text[cur_parsing_row]);
	cur_parsing_row++;

	enemy->defense = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;

	/* enemy attack */
	enemy->attack = strtoul(text[cur_parsing_row], &end, 10);
	cur_parsing_row++;

	/* enemy experience */
	enemy->exp = strtoul(text[cur_parsing_row], &end, 10);

	fclose(file);
	free(text);

	return enemy;
}

void destroy_enemy(enemy_t* enemy) {
	/*free(enemy->picture);*/
	/*free(enemy->welcome_message);*/ /* SEGFAULT*/
	free(enemy);
}
