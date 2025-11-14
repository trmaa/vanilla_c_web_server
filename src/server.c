#include <stdlib.h>
#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

#include "log.h"

void cleanup(int err);

extern int port;

struct {
	int fd;
	socketaddr_t addr;
} server;

void
server_setup()
{
	(server.fd = socket(AF_INET, SOCK_STREAM, 0)) ? 0 : fatal("Failed to create socket", exit, 1); 

	server.addr.sin_family = AF_INET;
	server.addr.sin_addr.s_addr = INADDR_ANY;
	server.addr.sin_port = htons(port);

	bind(server.fd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0 ? fatal("Failed to reach socket", cleanup, 1) : 0;

	listen(server.fd, 5) < 0 ? fatal("Failed to listen", cleanup, 1) : 0;

	debugf("Server is listening on port:  %d\n", port);
}

