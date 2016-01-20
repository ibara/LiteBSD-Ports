#ifndef __XMALLOC_H__
#define __XMALLOC_H__

#include <sys/types.h>

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
void *xcalloc(size_t nmemb, size_t size);
char *xstrdup(const char *s);

#endif	/* __XMALLOC_H__ */
