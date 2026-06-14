/*
 * HTTP Server
 *
 * http_server/src/server.c
 *
 * -> Do with this code whatever you want but selling it, please.
 * -> It must remain free forever.
 *
 * DOC:
 *	server_setup(&server, port)
 *	Constructs a struct server by:
 *	    1. Creating a socket on the server's fd.
 *	    2. Allawing the fd to be reusable in case it isn't closed when the program crashes.
 *	    3. Giving the server an address and a port.
 *	    4. Binding all together.
 *	    5. Start listening.
 */

#include <stdlib.h>
#include <arpa/inet.h>
#include "error.h"
#include "socket.h"

void server_setup(struct server *server, int port)
{
	server->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->fd < 0)
		fatal("Failed to create socket", NULL, SYSTEM_ERROR);

	int opt = 1;
	setsockopt(server->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	server->addr.sin_family = AF_INET;
	server->addr.sin_addr.s_addr = INADDR_ANY;
	server->addr.sin_port = htons(port);

	if (bind(server->fd, (struct sockaddr *)&server->addr,
		 sizeof(server->addr)) < 0)
		fatal("Failed to reach socket", NULL, SYSTEM_ERROR);

	if (listen(server->fd, 5) < 0)
		fatal("Failed to listen", NULL, SYSTEM_ERROR);
}

void client_setup(struct client *client)
{
	client->len = sizeof(client->addr);
}
