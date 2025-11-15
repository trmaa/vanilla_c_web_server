#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

typedef struct server{
	int fd;
	socketaddr_t addr;
} server_t;

void server_setup(server_t* server, int port);

#endif
