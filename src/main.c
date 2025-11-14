#include <stdio.h>
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
	close(server.fd);
	close(client.fd);
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
		(client.fd = accept(server.fd, (struct sockaddr*)&client.addr, &client.len)) < 0 ? fatal("Failed to accept connection", cleanup, 1) : 0;
		debugf("Client fd: %d\n", client.fd);
		
		debugf("Client connected from: %s:%d\n",
				inet_ntoa(client.addr.sin_addr),
				ntohs(client.addr.sin_port));

		while (1) 
		  {
			int bytes_read = recv(client.fd, req_buff, 1024, 0);

			if (bytes_read <= 0) 
			  {
				if (bytes_read == 0) debug("Client desconected.");
				else fatal("Corrupt req_buffuest", cleanup, 1);
				break;
			  }

			req_buff[bytes_read] = '\0';

			req = parse_request(req_buff);
			res = respond(req);
			send(client.fd, res, strlen(res), 0);

			fflush(stdout);
		  }

		close(client.fd);
	  }

	close(server.fd);
}
