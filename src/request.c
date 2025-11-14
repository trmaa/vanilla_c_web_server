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

	printf("Req: %s\n", line);

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
		return "application/octet-stream";
}

char*
respond(http_t req, char* dir)
{
	if (strcmp(req.version, "HTTP/1.1")) fatal("Unsuported http version", cleanup, 1);
	if (strcmp(req.method, "GET")) fatal("Unsuported http method", cleanup, 1);
	if (req.path[0] != '/') fatal("Path must be absolute", cleanup, 1);

	char* res = malloc(4096);

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

	FILE* html;
	html = fopen(path, "r");

	if (!html) 
	  {
		sprintf(res, "HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/plain\r\n"
				"\r\n"
				"404 - File Not Found\r\n");
		return res;
	  }
	
	const char* content_type = get_content_type(path);

	sprintf(res, "HTTP/1.1 200 OK\r\n"
			"Content-Type: %s\r\n"
			"\r\n", content_type);

	char buff[1024];

	while (fgets(buff, sizeof(buff), html)) 
	  {
		strcat(res, buff);
	  }

	fclose(html);

	return res;
}
