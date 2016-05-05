/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2003 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

void	secs_mem_init(void) {
}

void	*secs_mem_alloc(size_t size) {
	void	*ptr = NULL;

	ptr = calloc(1, size);
	assert(ptr != NULL);
	return(ptr);
}

void	*secs_mem_realloc(void *ptr, size_t size) {
	ptr = realloc(ptr, size);
	assert(ptr != NULL);
	return(ptr);
}

void	secs_mem_free(void *ptr) {
	assert(ptr != NULL);
	free(ptr);
}
