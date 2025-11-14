#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

extern struct {
	int fd;
	socketaddr_t addr;
} server;

void server_setup(int port);

#endif
