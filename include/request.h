#ifndef REQUEST_H
#define REQUEST_H

typedef struct http {
	char method[33], path[256], version[64];
} http_t;


http_t parse_request(char* req_buff);

char* respond(http_t req, char* dir);

#endif

