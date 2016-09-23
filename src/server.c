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
#include <pthread.h>

int sync_level(int clients[], size_t plr_cnt); 

void* start_server(void *arg) {
	int listener;
	int clients[4];
	int ready[4];
	int yes = 1;
	int bytes_recv;
	struct sockaddr_in addr;
	char buf[256];
	int max_fd;
	int end = 0;
	size_t i, plr_cnt = 0, j;
	int closestatus;
	

	fd_set master;
	fd_set read_fds;
	char* string;	
	char* START_MSG = "start";	
	char* READY_MSG = "ready";	
	char* CANCEL_MSG = "cancel";	
	char* STOPPED_MSG = "stop";
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
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
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
						puts("one new client");
						FD_SET(clients[plr_cnt], &master);
						if(clients[plr_cnt] > max_fd) max_fd = clients[plr_cnt];
					}
					if((setsockopt(clients[plr_cnt], SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))) == -1) {
						perror("setsockopt");
						return NULL;
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
					/*RECV MESSAGE*/
					else {
						puts("server recv message");
						buf[bytes_recv] = '\0';
						string = buf;
						if(is_equals(string, READY_MSG)) {
							for(j = 0; j < 4; j++) {
								if(clients[j] == i) ready[j] = 1;
							}
						}
						printf("%s", string);
						if(is_equals(string, CANCEL_MSG)) {
							end = 1;
							puts("end");
							for(j = 0; j < plr_cnt; j++) {
								puts("send stop to clients");
								if(clients[j] != i) {
									if((send(clients[j], STOPPED_MSG, strlen(STOPPED_MSG), 0)) < 0) {
										perror("SEND STOP ERROR");
									}
									puts("send close");
									FD_CLR(j, &master);
									/*close(j);*/
									puts("send close");
								}
							}
							FD_CLR(i, &master);
							close(i);
							puts("i closed");
						}
					}
				}
			}
	
			/* if host canceled */
			if(end) {
				if((closestatus = close(listener)) != 0) {
					perror("CANT CLOSE");
					puts("Can't close SOCKET");
				}
				else puts("listening socket closed");
				/*server_running = 0;*/
				pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
				pthread_testcancel();
				return NULL;
			}
		}
		/*check if all is ready*/
		start = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!ready[i]) start = 0;
		}
		
	}
	FD_CLR(listener, &master);
	/* All player ready, send start */
	for(i = 0; i <= max_fd; i++) 
		if(FD_ISSET(i, &master))
			if(i != listener)
				if(send(i, START_MSG , strlen(START_MSG), 0) == -1) perror("send");


	sync_level(clients, plr_cnt);
	sync_level(clients, plr_cnt);
	sync_level(clients, plr_cnt);
	sync_level(clients, plr_cnt);
	return (void*)1;
}
		
int sync_level(int clients[], size_t plr_cnt) {
	int next_level = 0;
	int ready[4] = {0};
	int ready_fight[4] = {0};
	int max_fd = 0;
	int recvd_bytes;
	char buf[256];
	char* string;
	char* READY_MSG = "ready";
	char* NEXT_MSG = "next";
	char* FIGHT = "figth";
	size_t i, j;
	fd_set master;
	fd_set read_fds;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	for(i = 0; i< plr_cnt; i++) {
		if(clients[i] > max_fd)
			max_fd = clients[i];
		FD_SET(clients[i], &master);
		puts("synclevel client");
	}
	


	while(!next_level) {
		read_fds = master;
		if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			return 1;
		}
		puts("some action");
		for(i = 0; i <= max_fd; i++) {
			if(FD_ISSET(i, &read_fds)) {
				if((recvd_bytes = recv(i, buf, sizeof(buf), 0)) < 0) {
					perror("recv:server");
					return 1;
				} else {
					buf[recvd_bytes] = '\0';
					string = buf;
					if(is_equals(string, READY_MSG)) {
						for(j = 0; j < plr_cnt; j++) {
							if(clients[j] == i) ready[j] = 1;
						}
					}
		
				}
			}
		}

		next_level = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!ready[i]) next_level = 0;
		}
		
	}

	for(i = 0; i < plr_cnt; i++) 
		if(send(clients[i], NEXT_MSG , strlen(NEXT_MSG), 0) == -1) perror("send");
	
	return 0;
}
	
	
