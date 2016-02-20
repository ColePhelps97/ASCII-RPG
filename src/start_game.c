#include <stdlib.h>
#include <ncurses.h>
#include "hero.h"
#include "level.h"


int start(){
	hero* my_hero;
	my_hero = init_hero();
	load_level("level1", my_hero);
	load_level("level2", my_hero);
	load_level("level3", my_hero);
	load_level("level4", my_hero);
	return 1;	
}

int host_game() {
	int sock, listener;
        char buf[1024];
        int bytes_read;
        struct sockaddr_in addr;

        listener = socket(AF_INET, SOCK_STREAM, 0);
        if(listener < 0) {
                return -1;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(3425);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if((bind(listener, (struct sockaddr*)&addr, sizeof(addr))) < 0) {
                puts("bind error");
                return -1;
        }
        listen(listener, 1);
	
	hero* host_hero;
	host_hero = init_hero();
	load_multi_level
