#ifndef LOG_H
#define LOG_H

void fatal(char* s, void (*callback)(int), int err);
void debug(char* s); 
void debugf(const char *format, ...);

#endif
