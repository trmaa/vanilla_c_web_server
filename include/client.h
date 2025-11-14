#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

extern struct {
	int fd;
	socketaddr_t addr;
	socklen_t len;
} client;

void client_setup();

#endif
