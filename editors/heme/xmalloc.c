#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "error.h"

void *xmalloc(size_t size)
{
	void *p;
	
	p = malloc(size);
	if(p == NULL && size != 0)
		err_exit("malloc failed: %s", strerror(errno));
	
	return p;
}

void *xrealloc(void *ptr, size_t size)
{
	void *p;
	
	p = realloc(ptr, size);
	if(p == NULL && size != 0)
		err_exit("realloc failed: %s", strerror(errno));
	
	return p;
}

void *xcalloc(size_t nmemb, size_t size)
{
	void *p;
	
	p = calloc(nmemb, size);
	if(p == NULL && size != 0)
		err_exit("calloc failed: %s", strerror(errno));
	
	return p;
}

char *xstrdup(const char *s)
{
	return strcpy(xmalloc(strlen(s) + 1), s);
}
