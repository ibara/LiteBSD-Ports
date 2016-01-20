#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void err_exit(char *msg, ...)
{
	char string[512];
	va_list args;

	va_start(args, msg);
	vsnprintf(string, 511, msg, args);
	string[511] = 0;
	va_end(args);

	fprintf(stderr, "%s\n", string);
	exit(EXIT_FAILURE);
}

void err_ret(char *msg, ...)
{
	char string[512];
	va_list args;

	va_start(args, msg);
	vsnprintf(string, 511, msg, args);
	string[511] = 0;
	va_end(args);

	fprintf(stderr, "%s\n", string);
	fflush(stderr);
}
