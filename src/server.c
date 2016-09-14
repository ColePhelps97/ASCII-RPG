#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include "mystring.h"


void* start_server(void *arg) {
	int listener;
	int clients[3];
	int ready[3];
	int yes = 1;
	int bytes_recv;
	struct sockaddr_in addr;
	char buf[256];
	int max_fd;
	/*int LISTEN_ERROR = 4;*/
	/*int SETSOKOPT_ERROR = 2;*/
	/*int BIND_ERROR= 3;*/
	/*int SOCKET_ERROR = 1;*/
	/*int LISTEN_ERROR = 1;*/
	size_t i, plr_cnt = 0, j;
	

	fd_set master;
	fd_set read_fds;
	char* string;	
	char* start_msg = "start";	
	char* READY_MSG = "ready";	
	int start = 0;
	

	ready[0] = 0;
	ready[1] = 0;
	ready[2] = 0;
	ready[3] = 0;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return NULL;
	}

	if((setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
		perror("setsockopt");
		return NULL;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6666);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(addr.sin_zero, 0, sizeof addr.sin_zero);

	if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		return NULL;
	}
	
	if(listen(listener, 4) == -1) {
		perror("listen");	
		return NULL;
	}

	FD_SET(listener, &master);
	max_fd = listener;
	


	/*Start multiplayer, when all players are ready, send confirmation */
	while(!start) {
		read_fds = master;
		if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			return NULL;
		}

		for(i = 0; i <= max_fd; i++) {
			if(FD_ISSET(i, &read_fds)) {
				/*if listener*/
				if(i == listener) {
					clients[plr_cnt] = accept(listener, NULL, NULL);
					if(clients[plr_cnt] == -1) {
						perror("accept");
						return NULL;
					}
					else {
						FD_SET(clients[plr_cnt], &master);
						if(clients[plr_cnt] > max_fd) max_fd = clients[plr_cnt];
					}
					plr_cnt++;
					/* if connected 4th player, team is full*/
					if(plr_cnt == 4) FD_CLR(listener, &master);
				} 
				/*if not listener*/
				else {
					/*if recv error*/
					if((bytes_recv = recv(i, buf, sizeof buf, 0)) < 0) {
						perror("recv");
						close(i);
						plr_cnt--;
						FD_CLR(i, &master);
					} 
					/*not recv error*/
					else {
						buf[bytes_recv] = '\0';
						string = buf;
						if(is_equals(string, READY_MSG)) {
							for(j = 0; j < 4; j++) {
								if(clients[j] == i) ready[j] = 1;
							}
						}
					}
				}
			}
		}
		/*check if all is ready*/
		start = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!ready[i]) start = 0;
		}
		
	}
	
	/* All player ready, send start */
	for(i = 0; i <= max_fd; i++) 
		if(FD_ISSET(i, &master))
			if(i != listener)
				if(send(i, start_msg, strlen(start_msg), 0) == -1) perror("send");

	return NULL;
}
		
