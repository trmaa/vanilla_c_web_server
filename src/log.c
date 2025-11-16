/*
 * HTTP Server
 *
 * http_server/src/log.c
 *
 * -> Do with this code whatever you want but selling it, please.
 * -> It must remain free forever.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void
log_usage(int err)
{
	fprintf(stderr, "Usage: serve [-p port] [-d directory]\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h HELP\n");
	fprintf(stderr, "  -p PORT      (default: 8080)\n");
	fprintf(stderr, "  -d DIR       (default: ./)\n");
	exit(err);
}

void 
fatal(char* s, void (*callback)(int), int err) 
{
	fprintf(stderr, "\e[1;31m[FATAL] %s\e[0m\n", s);
	callback(err);
}

void 
debug(char* s) 
{
	printf("\e[1;33m[debug] * %s\e[0m\n", s);
}

void 
debugf(const char *format, ...) 
{
	printf("\e[1;33m[debug] * ");

	va_list args;
	va_start(args, format);

	while (*format != '\0') 
	  {
		if (*format == '%') 
		  {
			format++; // Move past the '%'

			switch (*format) 
			  {
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
					printf("%%%c", *format); // Print unknown format literally
					break;
			  }
		  } else {
			putchar(*format);
		  }
		format++;
	  }

	va_end(args);
	
	printf("\e[0m");
}
