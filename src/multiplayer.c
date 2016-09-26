#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <panel.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "start_game.h"
#include "server.h"
#include "mystring.h"

int host_game(void) {
	WINDOW* window;
	PANEL* panel;
	int sock;
	struct sockaddr_in addr;
	int yes = 1;
	char buf[256];
	char* string_buf;
	int bytes_recv;
	char *ready= "ready";
	char *start= "start";
	char* cancel = "cancel";
	char key;

	pthread_t server;

	if(pthread_create(&server, NULL, start_server, NULL));

	window = newwin(10, 40, 10, 60);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	

	mvwprintw(window, 4, 5, "%s", "Waiting for connections");

	update_panels();
	doupdate();
	
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 1;
	}

	if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
		perror("setsockopt");
		return 2;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(addr.sin_zero, 0, sizeof addr.sin_zero);
	
	if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		return 3;
	}

	key = getch();
	if(key == 'c') {
		if(send(sock, cancel, strlen(cancel), 0) < 0) perror("send");
		pthread_cancel(server);	
		close(sock);
		/*del_panel(panel);*/
		/*delwin(window);*/
		return 0;
	} else 
		if(key == '\n')
			send(sock, ready, strlen(ready), 0);

	while(1) {
		if((bytes_recv = recv(sock, &buf, 256, 0)) != -1) {
			buf[bytes_recv] = '\0';
			mvwprintw(window, 1, 1, "%s", buf);
			string_buf = buf;

			if(is_equals(string_buf, start)) break;

			update_panels();
			doupdate();
		} else {
			perror("recv");
			return 1;
		}
	}

	start_mult_game(sock);
	close(sock);
	del_panel(panel);
	delwin(window);
	return 13;
}

int connect_to_game();


int mp_menu() {
	WINDOW* window;
	PANEL* panel;
	int menu_position = 0;
	int maxx, maxy;
	int exit = 0;
	char key;

	getmaxyx(stdscr, maxx, maxy);

	window = newwin(maxx, maxy, 0, 0);
	panel = new_panel(window);

	top_panel(panel);

	while(!exit) {
		mvwprintw(window, maxx/2 - 1, maxy/2 - 5, "%s", "Host game");

		mvwprintw(window, maxx/2, maxy/2 - 5, "%s", "Connect");

		mvwprintw(window, maxx/2 + 1, maxy/2 - 5, "%s", "Back");
		mvwprintw(window, maxx/2 - 1 + menu_position, maxy/2 - 8, "%s", "*");
	
		update_panels();
		doupdate();

		key = getch();

		mvwprintw(window, maxx/2 - 1 + menu_position, maxy/2 - 8, "%s", " ");
		clear();

		switch(key) {
			case 'j':
					if(menu_position < 2) menu_position++;
					break;
			case 'k':
					if(menu_position > 0) menu_position--;
					break;
			case 'e':
					if(menu_position == 2) {
						exit = 1;
						break;
					}
					if(menu_position == 1) {
						exit = connect_to_game();
						break;
					}
					if(menu_position == 0) {
						exit = host_game();
						break;
					}
		}
	}
	del_panel(panel);
	delwin(window);
	return !exit;
}
	
int connect_to_game(void) {
	WINDOW* window;
	PANEL* panel;
	char buf[256];
	int yes = 1;
	struct sockaddr_in sockaddr;
	int sock, bytes_recv;
	char* ready = "ready";
	char* start = "start";
	char* stop = "stop";
	char key;
	char* string_buf;
	
	window = newwin(10, 40, 10, 60);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	mvwprintw(window, 4, 5, "%s", "Waiting other players");

	update_panels();
	doupdate();

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("client socket");
		return 1;
	}

	if((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
		perror("setsockopt");
		return 1;
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(6666);
	sockaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


	if(connect(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
		perror("connect");
		return 1;
	}

	key = getch();
	if(key == 'c') {
		del_panel(panel);
		delwin(window);
		close(sock);
		return 0;
	} else 
		if(key == '\n')
			if(send(sock, ready, strlen(ready), 0) < 0) {
				perror("send");
				return 1;
			}
	while(1) {
		if((bytes_recv = recv(sock, &buf, 256, 0)) != -1) {
			buf[bytes_recv] = '\0';
			mvwprintw(window, 1, 1, "%s", buf);
			string_buf = buf;

			if(is_equals(string_buf, start)) break;
			if(is_equals(string_buf, stop)) {
				del_panel(panel);
				delwin(window);
				close(sock);
				return 0;
			}
			update_panels();
			doupdate();
		} else {
			perror("recv");
			return 1;
		}
	}

	start_mult_game(sock);
	del_panel(panel);
	delwin(window);
	close(sock);
	return 0;
}
