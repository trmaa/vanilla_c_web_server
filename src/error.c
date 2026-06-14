/*
 * HTTP Server
 *
 * http_server/src/error.c
 *
 * -> Do with this code whatever you want but selling it, please.
 * -> It must remain free forever.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void help(enum status err)
{
	fprintf(stderr, "Usage: serve [-p port] [-d directory]\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h HELP\n");
	fprintf(stderr, "  -p PORT      (default: 8080)\n");
	fprintf(stderr, "  -d DIR       (default: ./)\n");
	exit(err);
}

void fatal(char *s, void (*callback)(enum status), enum status err)
{
	fprintf(stderr, "\033[1;31m[FATAL] %s\033[0m\n", s);
	if (callback)
		callback(err);
	exit(err);
}

void debug(char *s)
{
	printf("\033[1;33m[debug] * %s\033[0m\n", s);
}

void debugf(const char *format, ...)
{
	printf("\033[1;33m[debug] * ");

	va_list args;
	va_start(args, format);

	while (*format != '\0') {
		if (*format == '%') {
			format++;

			switch (*format) {
			case 'd':
			{
				int num = va_arg(args, int);
				printf("%d", num);
				break;
			}
			case 'f':
			{
				double num = va_arg(args, double);
				printf("%f", num);
				break;
			}
			case 'c':
			{
				char c = (char)va_arg(args, int);
				printf("%c", c);
				break;
			}
			case 's':
			{
				char *str = va_arg(args, char*);
				printf("%s", str);
				break;
			}
			case '%':
			{
				printf("%%");
				break;
			}
			default:
				printf("%%%c", *format);
				break;
			}
		} else {
			putchar(*format);
		}
		format++;
	}

	va_end(args);

	printf("\033[0m");
}
