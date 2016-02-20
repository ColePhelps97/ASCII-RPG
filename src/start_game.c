#include <stdlib.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include "hero.h"
#include "mystring.h"
#include "level.h"


int start_game(){
	hero* my_hero;
	my_hero = init_hero();
	load_level("level1", my_hero);
	load_level("level2", my_hero);
	load_level("level3", my_hero);
	load_level("level4", my_hero);
	return 1;	
}

int host_game() {
	int sock, listener, end_of_connection = 0;
        char* buf;
        int bytes_read;
	char* start = "start";
	char* connect = "connected";
	hero* host_hero;
        struct sockaddr_in addr;

	buf = (char*) malloc(9*sizeof(char));

        listener = socket(AF_INET, SOCK_STREAM, 0);

        addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if((bind(listener, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
		mvwprintw(stdscr, 10, 10, "%s", "bind error");
        }
        listen(listener, 1);
	

	while(!end_of_connection) {
		sock = accept(listener, NULL, NULL);
		while(1) {
			bytes_read = recv(sock, buf, 9, 0);
			if((bytes_read > 0) && (is_sub_string((char*)buf, connect))) {	
				send(sock, start, strlen(start), 0);
				end_of_connection = 1;
			}
			else break;
		}
		close(sock); 
	}
			
	free(buf);
	close(listener);
	
	host_hero = init_hero();
	load_multi_level("level1", host_hero, 1);
	load_multi_level("level2", host_hero, 1);
	load_multi_level("level3", host_hero, 1);
	load_multi_level("level4", host_hero, 1);

	return 1;
}

int connect_to_game() {
	int sock;
	hero* client_hero;
	char* buf;
	char* connect_message = "connected";
	char* start = "start";
	struct sockaddr_in addr;
	
	buf = (char*) malloc(5*sizeof(char));
	
	sock = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
        addr.sin_port = htons(1234);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		return 0;
	}		
	
	while(1) {
		send(sock, connect_message, strlen(connect_message), 0);
		recv(sock, buf, 5, 0);
		if(is_sub_string((char*)buf, start)) break;
	}
	close(sock);
	free(buf);

	client_hero = init_hero();

	load_multi_level("level1", client_hero, 2);
	load_multi_level("level2", client_hero, 2);
	load_multi_level("level3", client_hero, 2);
	load_multi_level("level4", client_hero, 2);

	return 1;
}
