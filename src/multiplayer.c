#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "start_game.h"

int host_game() {
	WINDOW* window;
	PANEL* panel;
	int listener, client;
	struct sockaddr_in addr;
	int yes = 1;
	char buf[256];
	int len, bytes_sent, bytes_recv;
	char *messg = "Hello!";

	window = newwin(10, 40, 30, 40);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);
	update_panels();
	doupdate();
	
	if((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		perror("listener socket");
		return 1;
	}

	if((setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
		perror("setsockopt");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(addr.sin_zero, 0, sizeof addr.sin_zero);

	if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind:");
		return 1;
	}
	
	if(listen(listener, 1) == -1) {
		perror("listen");
		return 1;
	}
	
	/*if((client = accept(listener, (struct sockaddr *)&client_addr, &client_addr_size)) == -1 ) {*/
	if((client = accept(listener, NULL, NULL)) == -1 ) {
		perror("accept");	
		return 1;
	}
	
	len = strlen(messg);
	bytes_sent = send(client, messg, len, 0);
	printf("%d", len - bytes_sent);

	if((bytes_recv = recv(client, &buf, 256, 0)) != -1) {
		buf[bytes_recv] = '\0';
		mvwprintw(window, 1, 1, "%s", buf);
		update_panels();
		doupdate();
	} else {
		perror("recv");
		return 1;
	}

	start_mult_game(1, client);
	close(client);
	close(listener);
	del_panel(panel);
	delwin(window);
	return 0;
}

int connect_to_game(WINDOW* window);


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
						exit = connect_to_game(window);
						break;
					}
					if(menu_position == 0) {
						exit = host_game(window);
						break;
					}
		}
	}
	del_panel(panel);
	delwin(window);
	return !exit;
}
	
int connect_to_game(WINDOW* window) {
	char buf[256];
	struct sockaddr_in sockaddr;
	int sock, bytes_recv, bytes_sent;
	char* msg = "hellO";

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("client socket");
		return 1;
	}

	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(6666);
	sockaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


	if(connect(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1) {
		perror("connect");
		return 1;
	}

	if((bytes_recv = recv(sock, &buf, 256, 0)) != -1) {
		buf[bytes_recv] = '\0';
		printf("%s", buf);
	} else {
		perror("recv");
		return 1;
	}

	if((bytes_sent = send(sock, msg, strlen(msg), 0)) == -1) {
		perror("send");
		return 1;
	}

	start_mult_game(2, sock);

	return 0;
}
