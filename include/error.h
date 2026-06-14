#pragma once

enum status {
	OK           = 0,
	ERROR        = 1,
	USER_ERROR   = 2,
	SYSTEM_ERROR = 3,
};

void help(enum status err);
void fatal(char *s, void (*callback)(enum status), enum status err);
void debug(char *s);
void debugf(const char *format, ...);
