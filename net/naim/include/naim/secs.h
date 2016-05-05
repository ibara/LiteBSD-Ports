/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2004 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#ifndef secs_h
#define secs_h  1

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SECS_ERROR	(-1)
#define SECS_CONTINUE	0
#define SECS_HANDLED	1

typedef struct secs_var_s {
	char	*name,
		*_buf_str,
		**val_str;
	long	_buf_num,
		*val_num;
	size_t	length;
	struct secs_var_s	*next;
	int	(*set)(struct secs_var_s *, const char *);
} secs_var_t;

typedef struct secs_block_s {
	secs_var_t	*variables;
} secs_block_t;


/*
 * Provide G_GNUC_INTERNAL that is used for marking library functions
 * as being used internally to the lib only, to not create inefficient PLT entries
 */
#if defined (__GNUC__)
# define G_GNUC_INTERNAL	__attribute((visibility("hidden")))
#else
# define G_GNUC_INTERNAL
#endif


/* From `atomizer.c': */
char * firstatom (char *string , char *bounds ) G_GNUC_INTERNAL;
char * firstwhite (char *string ) G_GNUC_INTERNAL;
char * atom (char *string ) G_GNUC_INTERNAL;

/* From `block.c': */
void secs_block_init (void) G_GNUC_INTERNAL;
secs_block_t * secs_block_create (secs_block_t *parent , char *name ) G_GNUC_INTERNAL;
int secs_block_var_add (secs_block_t *block, secs_var_t *var ) G_GNUC_INTERNAL;

/* From `liaison.c': */
secs_block_t *secs_block_getroot (void) G_GNUC_INTERNAL;
int	secs_init(void);
void	secs_handle(char *line) G_GNUC_INTERNAL;
int	secs_makevar(const char *name, const char *value, const char type);
int	secs_makevar_int(const char *name, long value, const char type, long *buf_num);
int	secs_setvar(const char *name, const char *val);
char	*secs_getvar(const char *name);
long	secs_getvar_int(const char *name);
char	*secs_listvars(int i, size_t *length, void **_var);

/* From `mem.c': */
void secs_mem_init (void) G_GNUC_INTERNAL;
void * secs_mem_alloc (size_t size ) G_GNUC_INTERNAL;
void * secs_mem_realloc (void *ptr , size_t size ) G_GNUC_INTERNAL;
void secs_mem_free (void *ptr ) G_GNUC_INTERNAL;

/* From `script.c': */
void secs_script_init (void) G_GNUC_INTERNAL;
char * secs_script_expand (secs_block_t *block , const char *instr );
int secs_script_parse (const char *line );

/* From `vars.c': */
void secs_var_init (void) G_GNUC_INTERNAL;
int secs_var_set_str (secs_var_t *var , const char *val ) G_GNUC_INTERNAL;
int secs_var_set_int (secs_var_t *var , const char *value ) G_GNUC_INTERNAL;
int secs_var_set_switch (secs_var_t *var , const char *val ) G_GNUC_INTERNAL;
int secs_var_set (secs_var_t *var , const char *val ) G_GNUC_INTERNAL;
secs_var_t * secs_var_find (secs_var_t *first , const char *name ) G_GNUC_INTERNAL;
secs_var_t * secs_var_find_n (secs_var_t *first , const char *name ) G_GNUC_INTERNAL;
secs_var_t *secs_var_create(const char *name, int (*varset)(secs_var_t *, const char *)) G_GNUC_INTERNAL;

#endif
