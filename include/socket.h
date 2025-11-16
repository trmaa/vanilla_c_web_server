#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

typedef struct server{
	int fd;
	socketaddr_t addr;
} server_t;

void server_setup(server_t* server, int port);

typedef struct client {
	int fd;
	socketaddr_t addr;
	socklen_t len;
} client_t;

void client_setup(client_t* client);

#endif
