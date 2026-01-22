#ifndef LOG_H
#define LOG_H

void help(int err);

void fatal(char* s, void (*callback)(int), int err);

void debug(char* s); 
void debugf(const char *format, ...);

#endif
