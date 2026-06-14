#pragma once

#include <arpa/inet.h>

struct server {
	int fd;
	struct sockaddr_in addr;
};

struct client {
	int fd;
	struct sockaddr_in addr;
	socklen_t len;
};

void server_setup(struct server *server, int port);
void client_setup(struct client *client);
