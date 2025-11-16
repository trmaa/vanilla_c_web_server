/*
 * HTTP Server
 *
 * http_server/src/main.c
 *
 * -> Do with this code whatever you want but selling it, please.
 * -> It must remain free forever.
 *
 * DOC:
 * 	Installation: ./scripts/make.sh build install
 *
 * 	Usage: serve [-p port] (default: 8080) [-d dir] (default: ./)
 * 	Help: serve -h
 * 	
 * 	server_t and client_t
 * 	Are structs which contain a file descriptor and their address.
 *
 * 	http_t
 * 	Is a struct which stores a char* method, path, and version.
 * 	Ex: GET /index.html HTTP/1.1
 *
 * 	main()	
 * 	Will create an infinite loop where we:
 * 	    1. Accept a new request into the client fd -via the server fd- to the client's address. 
 * 	    2. Read the request from the client fd.
 * 	    4. Process the request (parses the char* into an http_t creates a valid path for the requested file).
 * 	    5. Sends a response to the client fd.
 * 	    6. Closes the client
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "log.h"
#include "socket.h"
#include "request.h"

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
				*dir = malloc(strlen(argv[i+1])+1);
				strcpy(*dir, argv[++i]);
				if ((*dir)[strlen(*dir)-1] != '/') fatal("Dir must end with '/'", exit, 1);
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

	server_t server;
	server_setup(&server, port);	

	client_t client;
	client_setup(&client);

	debugf("Server fd: %d\n", server.fd);
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
