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

int sync_fight(int clients[], size_t plr_cnt); 

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
							for(j = 0; j < plr_cnt; j++) {
								if(clients[j] != i) {
									if((send(clients[j], STOPPED_MSG, strlen(STOPPED_MSG), 0)) < 0) {
										perror("SEND STOP ERROR");
									}
									FD_CLR(j, &master);
									/*close(j);*/
								}
							}
							FD_CLR(i, &master);
							close(i);
						}
					}
				}
			}
	
			/* if host canceled */
			if(end) {
				if((closestatus = close(listener)) != 0) {
					perror("CANT CLOSE");
				}
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
	int next_level = 0, next_fight = 0;
	int ready[4] = {0};
	int ready_fight[4] = {0};
	int max_fd = 0;
	int recvd_bytes;
	char buf[256];
	char* string;
	char* READY_MSG = "ready";
	char* NEXT_MSG = "next";
	char* FIGHT_MSG = "fight";
	size_t i, j;
	fd_set master;
	fd_set read_fds;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	for(i = 0; i< plr_cnt; i++) {
		if(clients[i] > max_fd)
			max_fd = clients[i];
		FD_SET(clients[i], &master);
	}
	


	while(1) {
		read_fds = master;
		if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			return 1;
		}
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
					if(is_equals(string, FIGHT_MSG)) {
						for(j = 0; j < plr_cnt; j++) {
							if(clients[j] == i) ready_fight[j] = 1;
						}
					}
				}
			}
		}

		next_level = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!ready[i]) next_level = 0;
		}

		next_fight = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!ready_fight[i]) next_fight = 0;
		}

		if(next_level == 1) {
			for(i = 0; i < plr_cnt; i++) 
				if(send(clients[i], NEXT_MSG , strlen(NEXT_MSG), 0) == -1) perror("send");
			break;
		}

		if(next_fight == 1) {
			for(i = 0; i < plr_cnt; i++) 
				if(send(clients[i], FIGHT_MSG , strlen(FIGHT_MSG), 0) == -1) perror("send");
			sync_fight(clients, plr_cnt);
			break;
		}

	}

		
	/*in fight sync*/
	
	return 0;
}

int sync_fight(int clients[], size_t plr_cnt) {
	int damage[4] = {0};
	int next[4] = {0};
	char* ATTACK_MSG = "attack";
	char* string;
	char buf[256];
	int next_turn = 0;
	int i, j, recvd_bytes;
	int total_damage;
	int max_fd;
	
	fd_set master;
	fd_set read_fds;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	for(i = 0; i< plr_cnt; i++) {
		if(clients[i] > max_fd)
			max_fd = clients[i];
		FD_SET(clients[i], &master);
	}
	


	while(1) {
		read_fds = master;
		if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			return 1;
		}
		for(i = 0; i <= max_fd; i++) {
			if(FD_ISSET(i, &read_fds)) {
				if((recvd_bytes = recv(i, buf, sizeof(buf), 0)) < 0) {
					perror("recv:server");
					return 1;
				} else {
					puts("recvd_message");
					buf[recvd_bytes] = '\0';
					string = buf;
					if(is_equals(string, ATTACK_MSG)) {
						for(j = 0; j < plr_cnt; j++) {
							if(clients[j] == i) {
								next[j] = 1;
								puts("next ready");
							}
						}
						if((recvd_bytes = recv(i, buf, sizeof(buf), 0)) < 0) {
							perror("recv damage");
							return 1;
						}
						for(j = 0; j < plr_cnt; j++) {
							if(clients[j] == i) damage[j] = strtoul(string, NULL, 10);
						}
					}
				}
			}
		}
		next_turn = 1;
		for(i = 0; i < plr_cnt; i++) {
			if(!next[i]) {
				next_turn = 0;
			}
		}

		if(next_turn) {
			puts("next turn");
			for(i = 0; i < plr_cnt; i++) 
				total_damage += damage[i];
			if((sprintf(buf, "%d", total_damage)) > 0)
				puts("sprint ok");
			for(i = 0; i < plr_cnt; i++) {
				puts("send total damage");
				if(send(clients[i], "10", 2, 0) < 0) {
					perror("send");
					return 1;
				}
			}
			break;
		}

	}
	return 0;
}
	
	
