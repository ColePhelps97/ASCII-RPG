#include <stdlib.h>
#include <stdio.h>
#include "read_file.h"
#include "mystring.h"
#include "constants.h"



typedef struct enemy {
	char* enemy_name;
	char** enemy_welcome_message;
	char** enemy_picture;
	size_t wm_length;
	size_t picture_length;
	health_t max_enemy_health;
	health_t current_enemy_health;
	experience_t exp;
	attack_t enemy_attack;
} enemy;




	/* Enemy file struct                          */
	/* -------------------------------------------*/
	/* enemy_name                                 */
	/* number of rows in enemy welcome_message    */
	/* enemy_welcome_message                      */
	/* number of rows in enemy ASCII picture      */
	/* enemy ascii picture                        */
	/* enemy max health                           */
	/* enemy cur health                           */
	/* enemy attack                               */
	/* enemy experience                           */

        enemy parse_enemy(char* enemy_name) {
                enemy enemy;
                char** text;
                char* end;
                size_t cur_parsing_row, iter;
                FILE* file = fopen(concat("../enemies/", enemy_name, ".enemy"), "r");

                text = read_file(file);
                cur_parsing_row = 0;


                /* enemy name */
                enemy.enemy_name = text[cur_parsing_row];
                cur_parsing_row++;

                /* welcome message length  */
                enemy.wm_length = strtoul(text[cur_parsing_row], &end, 10);
                cur_parsing_row++;


		/* Welcome message */
                enemy.enemy_welcome_message = (char**) malloc(sizeof(char*) * enemy.wm_length);
                for( iter = 0; iter < enemy.wm_length; iter++) {
                        enemy.enemy_welcome_message[iter] = text[cur_parsing_row];
                        cur_parsing_row++;
                }


                /* Picture length  */
                enemy.picture_length = strtoul(text[cur_parsing_row], &end, 10);
                cur_parsing_row++;

                /* Picture */
                enemy.enemy_picture = (char**) malloc(sizeof(char*) * enemy.picture_length);
                for(iter = 0; iter< enemy.picture_length; iter++) {
                        enemy.enemy_picture[iter] = text[cur_parsing_row];
                        cur_parsing_row++;
                }

                /* Max health */
                enemy.max_enemy_health = strtoul(text[cur_parsing_row], &end, 10);
                cur_parsing_row++;


                /* Cur health */
                enemy.current_enemy_health = strtoul(text[cur_parsing_row], &end, 10);
                cur_parsing_row++;

                /* enemy attack */
                enemy.enemy_attack = strtoul(text[cur_parsing_row], &end, 10);
                cur_parsing_row++;

                /* enemy experience */
                enemy.exp = strtoul(text[cur_parsing_row], &end, 10);

                fclose(file);

                return enemy;
        }

