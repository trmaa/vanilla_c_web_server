/*
 * HTTP Server
 *
 * http_server/src/request.c
 *
 * -> Do with this code whatever you want but selling it, please.
 * -> It must remain free forever.
 *
 * DOC:
 *	respond(req, fd) 	
 *	Sends data from a requested file to an fd (which btw, is the client's fd) by:
 *	    1. Making sure its an http request.
 *	    2. Cleaning up the path to the file.
 *	    3. Opening the file in binary.
 *	    4. Sending to the fd a valid http header based on the file type.
 *	    5. Reading and sending the file to the fd 8 bytes chunks at a time.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "log.h"
#include "request.h"

http_t
parse_request(char* req_buff)
{
	http_t res = {"", "", ""};
	char* line = strtok(req_buff, "\r\n"); 

	sscanf(line, "%32s %255s %63s", res.method, res.path, res.version);

	return res;
}

char*
get_content_type(const char* path)
{
	const char* ext = strrchr(path, '.');
	if (!ext) return "text/plain";

	if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
		return "text/html";
	else if (strcmp(ext, ".js") == 0)
		return "application/javascript";
	else if (strcmp(ext, ".css") == 0)
		return "text/css";
	else if (strcmp(ext, ".json") == 0)
		return "application/json";
	else if (strcmp(ext, ".png") == 0)
		return "image/png";
	else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
		return "image/jpeg";
	else if (strcmp(ext, ".gif") == 0)
		return "image/gif";
	else if (strcmp(ext, ".svg") == 0)
		return "image/svg+xml";
	else if (strcmp(ext, ".ico") == 0)
		return "image/x-icon";
	else if (strcmp(ext, ".txt") == 0)
		return "text/plain";
	else
		return "text/plain";
}

void 
respond(http_t req, char* dir, int fd)
{
	if (strcmp(req.version, "HTTP/1.1")) fatal("Unsuported http version", exit, 1);
	if (strcmp(req.method, "GET")) fatal("Unsuported http method", exit, 1);
	if (req.path[0] != '/') fatal("Path must be absolute", exit, 1);

	char path[1024];
	snprintf(path, sizeof(path), "%s%s", dir, req.path + 1);

	FILE* file = fopen(path, "rb");
	if (!file) 
	  {
		char* response = "HTTP/1.1 404 Not Found\r\n"
			"Content-Type: text/plain\r\n"
			"\r\n"
			"404 - File Not Found\r\n";
		send(fd, response, strlen(response), 0);
		return;
	  }

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	const char* content_type = get_content_type(path);

	char header[512];
	int header_len = snprintf(header, sizeof(header),
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: %s\r\n"
					"Content-Length: %ld\r\n"
					"\r\n", content_type, file_size);

	send(fd, header, header_len, 0);

	char buff[8192];
	size_t bytes_read;
	while ((bytes_read = fread(buff, 1, sizeof(buff), file)) > 0) 
	  {
		send(fd, buff, bytes_read, 0);
	  }

	fclose(file);
}
