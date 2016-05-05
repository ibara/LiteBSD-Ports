/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2004 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

void	secs_block_init(void) {
}

secs_block_t	*secs_block_create(secs_block_t *parent, char *name) {
	secs_block_t	*block = NULL;
	size_t	slen = 0;

	assert(name != NULL);

	slen = strlen(name);
	block = secs_mem_alloc(sizeof(secs_block_t));
	block->variables = NULL;
	return(block);
}

int	secs_block_var_add(secs_block_t *block, secs_var_t *var) {
	assert(block != NULL);
	assert(var != NULL);

	if (var->next != NULL) {
		secs_var_t	*varptr = var->next;

		while (varptr->next != NULL)
			varptr = varptr->next;
		varptr->next = block->variables;
	} else
		var->next = block->variables;
	block->variables = var;
	return(1);
}
