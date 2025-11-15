#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

typedef struct client {
	int fd;
	socketaddr_t addr;
	socklen_t len;
} client_t;

void client_setup(client_t* client);

#endif
