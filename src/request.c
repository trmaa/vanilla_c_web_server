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
respond(http_t req)
{
	if (strcmp(req.version, "HTTP/1.1")) fatal("Unsuported http version", cleanup, 1);
	if (strcmp(req.method, "GET")) fatal("Unsuported http method", cleanup, 1);
	if (req.path[0] != '/') fatal("Path must be absolute", cleanup, 1);

	char* res = malloc(4096);

	char path[256];
	strncpy(path, req.path + 1, sizeof(path) - 1);
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
	
	sprintf(res, "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"\r\n");

	char buff[1024];

	while (fgets(buff, sizeof(buff), html)) 
	  {
		strcat(res, buff);
	  }

	fclose(html);

	return res;
}
