// SC is free software distributed under the MIT license

#include "sc.h"

char* scxmalloc (unsigned n)
{
    char* p = (char*) malloc (n);
    assert (p && "out of memory");
    if (!p) abort();
    return (p);
}

// we make sure realloc will do a malloc if needed
char* scxrealloc (char *ptr, unsigned n)
{
    char* p = (char*) realloc (ptr, n);
    assert (p && "out of memory");
    if (!p) abort();
    return (p);
}

void scxfree (void *p)
{
    assert (p && "scxfree: NULL");
    free(p);
}
