#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct sockaddr_in socketaddr_t;

#include "log.h"
#include "server.h"
#include "client.h"

int port;

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
	port = atoi(argv[1]);
	
	server_setup();	
	client_setup();

	char req[1025];

	while (1) 
	  {
		(client.fd = accept(server.fd, (struct sockaddr*)&client.addr, &client.len)) < 0 ? fatal("Failed to accept connection", cleanup, 1) : 0;
		
		debugf("Client connected from: %s:%d\n",
				inet_ntoa(client.addr.sin_addr),
				ntohs(client.addr.sin_port));

		while (1) 
		  {
			int bytes_read = recv(client.fd, req, 1024, 0);

			if (bytes_read <= 0) 
			  {
				if (bytes_read == 0) debug("Client desconected.\n");
				else fatal("Corrupt request", cleanup, 1);
				break;
			  }

			req[bytes_read] = '\0';

			printf("\t%s", req);
			fflush(stdout);
		  }

		close(client.fd);
	  }

	close(server.fd);
}
