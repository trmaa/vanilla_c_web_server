#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "log.h"
#include "server.h"
#include "client.h"
#include "request.h"

void
log_usage(int err)
{
	fprintf(stderr, "Usage: serve [-p port] [-d directory]\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h HELP\n");
	fprintf(stderr, "  -p PORT      (default: 8080)\n");
	fprintf(stderr, "  -d DIR       (default: ./)\n");
	exit(err);
}

void
flag_config(int* port, char** dir, int argc, char** argv)
{
	for (int i = 1; i < argc; i++) 
	  {
		if (argv[i][0] != '-') continue;

		switch (argv[i][1])
		  {
			case 'h':
				log_usage(0);
				break;
			case 'p':
				if (i+1 >= argc) fatal("Incomplete flag!!!", exit, 1);
				*port = atoi(argv[++i]);		
				break;
			case 'd':
				if (i+1 >= argc) fatal("Incomplete flag!!!", exit, 1);
				*dir = argv[++i];
				break;
			default:
				fatal("Wrong flag!", log_usage, 1);
		  }
	  }	
}

int
main(int argc, char** argv)
{
	int port = 8080;
	char* dir = "./";

	flag_config(&port, &dir, argc, argv);

	if (dir[strlen(dir)-1] != '/') fatal("Dir must end with '/'", exit, 1);

	server_t server;
	server_setup(&server, port);	

	client_t client;
	client_setup(&client);

	debugf("Server is listening on port:  %d\n", port);

	char req_buff[1025];
	http_t req;

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

		printf("REQ:\n%s", req_buff);

		req = parse_request(req_buff);

		respond(req, dir, client.fd);

		debug("Client's connection closed!");
		close(client.fd);
		client.fd = -1;
	  }

	close(server.fd);
	exit(0);
}
