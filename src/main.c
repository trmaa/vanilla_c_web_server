#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "log.h"
#include "server.h"
#include "client.h"
#include "request.h"

void
cleanup(int err)
{
	if (server.fd >= 0) close(server.fd);
	if (client.fd >= 0) close(client.fd);
	exit(err);
}

int
main(int argc, char** argv)
{
	if (argc != 3) fatal("Use: serve <port> <dir>", exit, 1);
	int port = atoi(argv[1]);
	
	server_setup(port);	
	client_setup();

	debugf("Server is listening on port:  %d\n", port);

	char req_buff[1025];
	http_t req;
	char* res;

	while (1) 
	  {
		client.fd = accept(server.fd, (struct sockaddr*)&client.addr, &client.len);

		if (client.fd < 0)
		  {
			debug("Failed to accept connection.");
			continue;
		  }

		debugf("Client fd: %d\n", client.fd);
		
		debugf("Client connected from: %s:%d\n",
				inet_ntoa(client.addr.sin_addr),
				ntohs(client.addr.sin_port));

		int bytes_read = recv(client.fd, req_buff, 1024, 0);

		req_buff[bytes_read] = '\0';

		req = parse_request(req_buff);
		res = respond(req);
		send(client.fd, res, strlen(res), 0);
		free(res);

		debug("Client's connection closed!");
		close(client.fd);
		client.fd = -1;
	  }

	close(server.fd);
	exit(0);
}
