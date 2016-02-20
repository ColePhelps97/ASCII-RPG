#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "read_file.h"
#include "mystring.h"
#include "fight.h"
#include "interface.h"
#include "top_panel.h"

typedef struct level {
	char** welcome_message;
	size_t wm_length;
	char** variants_to_do;
	size_t number_of_variants;
	char*** variants_text;
	enemy level_enemy;
	} level; 

/* level text file struct 						*/
/* number of welcome message rows					*/
/* welcome message							*/
/* Number of variants to do						*/
/* Variant 1 string							*/
/* Number of strings before next variant				*/
/* Variant 1 to do func (show_text, next_level, figth_with_enemy)	*/
/* Variant 2								*/
/*...									*/
/* Enemy_name								*/


level* parse_level(char* level_name) {
	level* level1;
	char** text;
	char* str_end, *enemy_name;
	size_t rows_num, cur_parsing_row = 0, variants_num;
	size_t iter, iter2;
	FILE* file = fopen(concat("../levels/", level_name,".level"), "r");
	text = read_file(file);
	level1 = (level*) malloc(sizeof(level));


	/* Welcome message length */
	rows_num = strtoul(text[cur_parsing_row], &str_end, 10);
	level1->wm_length = rows_num;
	cur_parsing_row++;


	/* Welcome message */
	level1->welcome_message = (char**) malloc(sizeof(char*) * rows_num);
	for(iter = 0; iter < rows_num; iter++) {
		level1->welcome_message[iter] = text[cur_parsing_row];	
		cur_parsing_row++;
	}
	
	/* Number of variants to do */
	variants_num = strtoul(text[cur_parsing_row], &str_end, 10);
	level1->number_of_variants = variants_num;
	cur_parsing_row++;

	/* Variants */
	level1->variants_to_do = (char**) malloc(sizeof(char*) * variants_num);
	level1->variants_text = (char***) malloc(sizeof(char*) * variants_num);
	for(iter = 0; iter < variants_num; iter++) {
		level1->variants_to_do[iter] = text[cur_parsing_row];
		cur_parsing_row++;
	
		/*Variants text */
		rows_num = strtoul(text[cur_parsing_row], &str_end, 10);
		level1->variants_text[iter] = (char**) malloc(sizeof(char*) * (rows_num + 1));
		for(iter2 = 0; iter2 < rows_num	+ 1; iter2++){
			level1->variants_text[iter][iter2] = text[cur_parsing_row];
			cur_parsing_row++;	
		}
	}
	
	enemy_name = text[cur_parsing_row];
	if(!is_sub_string(enemy_name , "none")) {  
		level1->level_enemy = parse_enemy(enemy_name);
	}
	fclose(file);
	return level1;

}
		
void destroy_level(level* level) {
	size_t iter, variants_num;
	variants_num = level->number_of_variants;
	for(iter = 0; iter < variants_num; iter++) 
		level->variants_text[iter] = realloc(level->variants_text[iter], 0);	
	level->variants_text = realloc(level->variants_text, 0);
	level->variants_to_do = realloc(level->variants_to_do, 0);
	level->welcome_message = realloc(level->welcome_message, 0);
}

void update_variants(level level) {
        size_t iter;
        for(iter = 0; iter < level.number_of_variants; iter++)
               mvwprintw(stdscr,38, getmaxx(stdscr)/level.number_of_variants * iter + 10, "%lu.%s", iter + 1, level.variants_to_do[iter]);
}


void prepare_screen_for_level(level level) {
        size_t iter;
        clear();
        load_interface();

        for(iter = 0; iter < level.wm_length; iter++)
                mvwprintw(stdscr, iter + 7, 10, "%s", level.welcome_message[iter]);

        update_variants(level);

        refresh();
}


void load_level(char* level_name, hero* hero) {
        size_t iter;
        char key;
        char* end;
        int is_next_level = 0;
        level* level;

        level = parse_level(level_name);
        prepare_screen_for_level(*level);

        while(!is_next_level) {
                key = getch();
                switch(key) {

                        case '1':
                                clear_game_screen();
                                if(is_sub_string(level->variants_text[0][1], "show_text") == 1) {
                                        for(iter = 0; iter < strtoul(level->variants_text[0][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[0][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[0][1], "next_level")) {
                                        is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

                                if(is_sub_string(level->variants_text[0][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }

                                break;

                        case '2':
                                if(is_sub_string(level->variants_text[1][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level->variants_text[1][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[1][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[1][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }


                                if(is_sub_string(level->variants_text[1][1], "next_level")) {
                                        is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

                                break;


                        case '3':
                                if(is_sub_string(level->variants_text[2][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level->variants_text[2][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[2][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[2][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }


                                if(is_sub_string(level->variants_text[2][1], "next_level")) {
                                        is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

                                break;

                        case 'q':
                                is_next_level = 1;
                                break;
				
			case 'c':
				character_panel(hero); 
				break;
			
			default: 
				break;
                }
        }
        clear();

}


int next_host_level() {
	char* ready = "ready";
	int bytes_read, sock, end_of_connection = 0, listener;
        char* buf;
	struct sockaddr_in addr;

	buf = (char*) malloc(sizeof(char)*5);
	
	listener = socket(AF_INET, SOCK_STREAM, 0);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if((bind(listener, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
                mvwprintw(stdscr, 10, 10, "%s", "bind error");
		refresh();
        }
        listen(listener, 1);
	puts("im listen");

	while(!end_of_connection) {
        	sock = accept(listener, NULL, NULL);
                while(1) {
                	bytes_read = recv(sock, buf, 5, 0);
			puts(buf);
                        if((bytes_read > 0) && (is_sub_string((char*)buf, ready))) {
                        	send(sock, ready, strlen(ready), 0);
                                end_of_connection = 1;
                        }
                        else break;
		}
                close(sock);
	}
	free(buf);
	close(listener);
	return 1;
}
	
int next_client_level() {
	char* ready = "ready";
        int sock; 
        char* buf;
        struct sockaddr_in addr;

        buf = (char*) malloc(sizeof(char)*5);

        sock = socket(AF_INET, SOCK_STREAM, 0);
	
	addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	while(1) {	
		if(!(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)) break;
	} 

	while(1) {
                send(sock, ready, strlen(ready), 0);
                recv(sock, buf, 5, 0);
                if(is_sub_string((char*)buf, ready)) break;
        }
        close(sock);
        free(buf);
	return 1;
}




void load_multi_level(char* level_name, hero* hero, int role) {
        size_t iter;
        char key;
        char* end;
        int is_next_level = 0;
        level* level;


        level = parse_level(level_name);
        prepare_screen_for_level(*level);

        while(!is_next_level) {
                key = getch();
                switch(key) {

                        case '1':
                                clear_game_screen();
                                if(is_sub_string(level->variants_text[0][1], "show_text") == 1) {
                                        for(iter = 0; iter < strtoul(level->variants_text[0][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[0][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[0][1], "next_level")) {
					if(role == 1) is_next_level = next_host_level();
					if(role == 2) is_next_level = next_client_level();
					if(role == 0) is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

                                if(is_sub_string(level->variants_text[0][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }

                                break;

			case '2':
                                if(is_sub_string(level->variants_text[1][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level->variants_text[1][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[1][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[1][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }


                                if(is_sub_string(level->variants_text[1][1], "next_level")) {
					
					if(role == 1) is_next_level = next_host_level();
					if(role == 2) is_next_level = next_client_level();
					if(role == 0) is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

                                break;


                        case '3':
                                if(is_sub_string(level->variants_text[2][1], "show_text")) {
                                        for(iter = 0; iter < strtoul(level->variants_text[2][0], &end, 10) - 1; iter++){
                                                mvwprintw(stdscr, iter + 7, 10, "%s", level->variants_text[2][iter+2]);
                                        }
                                }

                                if(is_sub_string(level->variants_text[2][1], "fight")) {
                                        fight(hero, level->level_enemy);
                                        getch();
                                        is_next_level = 1;
                                }


                                if(is_sub_string(level->variants_text[2][1], "next_level")) {
	
					if(role == 1) is_next_level = next_host_level();
					if(role == 2) is_next_level = next_client_level();
					if(role == 0) is_next_level = 1;
                                        /*destroy_level(&level);*/
                                }

				break;

                        case 'q':
                                is_next_level = 1;
                                break;

                        case 'c':
                                character_panel(hero);
                                break;

                        default:
                                break;
                }
        }
        clear();

}












	
	
