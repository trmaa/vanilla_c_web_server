#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "log.h"
#include "request.h"

void cleanup(int err);

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

char*
respond(http_t req, char* dir)
{
	if (strcmp(req.version, "HTTP/1.1")) fatal("Unsuported http version", cleanup, 1);
	if (strcmp(req.method, "GET")) fatal("Unsuported http method", cleanup, 1);
	if (req.path[0] != '/') fatal("Path must be absolute", cleanup, 1);

	char path[256];
	strncpy(path, req.path + 1, sizeof(path) - 1);

	{
		char temp[256];
		strcpy(temp, dir);
		strcat(temp, path);
		strcpy(path, temp);
	}

	path[sizeof(path) - 1] = '\0';

	/* 
	 * TODO: 
	 * 	1. separate querys
	 * 	2. urldecode
	 * 	3. normalize
	 */

	const char* content_type = get_content_type(path);

	int is_file_binary = strncmp(content_type, "text/", 5) 
				&& strcmp(content_type, "application/javascript")
				&& strcmp(content_type, "application/json");
	
	FILE* file;
	file = fopen(path, "rb");

	if (!file) 
	  {
		char* res = malloc(512);
		sprintf(res, "HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/plain\r\n"
				"\r\n"
				"404 - File Not Found\r\n");
		return res;
	  }

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	size_t header_size = 512;

	size_t total_size = header_size + file_size;
	char* res = malloc(total_size);

	int header_len = sprintf(res, "HTTP/1.1 200 OK\r\n"
					"Content-Type: %s\r\n"
					"Content-Length: %ld\r\n"
					"\r\n", content_type, file_size);

	size_t bytes_read = fread(res + header_len, 1, file_size, file);

	if (bytes_read != (size_t)file_size) 
	  {
		debug("Couldn't read all bytes:");
		debugf("To read: %d, Read: %d\n", file_size, bytes_read);
	  }

	if (!is_file_binary) res[header_len + bytes_read] = '\0';

	fclose(file);
	return res;
}
