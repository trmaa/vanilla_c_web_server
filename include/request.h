#pragma once

struct http {
	char method[33], path[256], version[65];
};

struct http parse_request(char *req_buff);

void respond(struct http req, char *dir, int client_fd);
