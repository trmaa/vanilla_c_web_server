#include <arpa/inet.h>

typedef struct sockaddr_in socketaddr_t;

#include "log.h"

void cleanup(int err);

struct {
	int fd;
	socketaddr_t addr;
} server;

void
server_setup(int port)
{
	(server.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ? fatal("Failed to create socket", cleanup, 1) : 0;

	int opt = 1;
	setsockopt(server.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	server.addr.sin_family = AF_INET;
	server.addr.sin_addr.s_addr = INADDR_ANY;
	server.addr.sin_port = htons(port);

	debugf("Server fd: %d\n", server.fd);

	bind(server.fd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0 ? fatal("Failed to reach socket", cleanup, 1) : 0;

	listen(server.fd, 5) < 0 ? fatal("Failed to listen", cleanup, 1) : 0;
}

