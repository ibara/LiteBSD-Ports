#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pconfig.h"

/*
 * pconfig.c: parse config files
 * 
 * (in config file)
 *  option_1 = blah    # this is just a comment
 *  option_2 = 13413.32
 *  option3=   #this setting is unset, use default
 *
 *
 * (in program)
 * pconfig_option_t myoptions[] = {
 * type             name         default value
 * PCONFIG_STRING, "option_1",      "something",
 * PCONFIG_INT,    "option_2",   "10",
 * 0, NULL, NULL
 *
 * }
 *
 * pconfig_init(0, myoptions);
 * pconfig_parse("/etc/program.conf");
 *
 * int a;
 * a = pconfig_get_int("option_2");
 *
 * char *s;
 * s = pconfig_get_string("option_1");
 *
 */

/* internal options struct */
typedef struct _pconfig_opt_t
{
	char *name;
	pconfig_otype_t type;
	union
	{
		char *str_val;
		int int_val;
	}
	val;
	struct _pconfig_opt_t *next;
}
pconfig_opt_t;

static unsigned int pconfig_hashsize;
static pconfig_opt_t **opt_table;

static char errbuf[512];

/* a simple hash function */
static unsigned int hash(const char *s)
{
	unsigned int val = 0;

	while(*s)
		val = 31 * val + *s++;
	return val % pconfig_hashsize;
}

/*
 * reads a line from 'fp' regardless of its length
 * returns pointer to malloc()ed line buffer which the caller must free
 */
static char *get_line(FILE *fp)
{
	char *buf, *p;
	int ch;
	size_t bufsize, size;

	bufsize = 1024;
	buf = malloc(bufsize);
	if(buf == NULL)
		return NULL;

	size = 0;
	while(1)
	{
		ch = fgetc(fp);
		if(ch == EOF)
		{
			 /* end of file or error */
			if(buf)
				free(buf);
			return NULL;
		}

		buf[size++] = ch;
		if(size == bufsize - 1)
		{
			 /* allocate a bigger buffer */
			p = realloc(buf, bufsize * 2);
			if(p == NULL)
			{
				free(buf);
				sprintf(errbuf, "realloc() failed, not enough memory");
				return NULL;
			}
			bufsize *= 2;
			buf = p;
		}

		if(ch == '\n')
		{
			 /* replace the newline with '\0' */
			buf[size - 1] = '\0';
			break;
		}
	}
	return buf;
}

/*
 * inserts an option into the hash table
 */
static int add_option(const char *name, const char *value)
{
	pconfig_opt_t *t;

	t = opt_table[hash(name)];
	if(t == NULL)
	{
		/* this option is unrecognized */
		sprintf(errbuf, "unrecognized option: %s", name);
		return -1;
	}

	while(t && strcmp(t->name, name) != 0)
		t = t->next;

	switch(t->type)
	{
	case PCONFIG_STRING:
		free(t->val.str_val);
		t->val.str_val = strdup(value);
		break;
	case PCONFIG_INT:
		t->val.int_val = atoi(value);
		break;
	default:
		/* this shoud never happen */
		break;
	}
	return 0;
}

/*
 * parses a line pointed to by 'buf'
 */
#define SKIP_WS(p) while(isspace(*p)) p++
#define BUF_LEN 4096 /* limitation, FIXME */
static int parse_line(char *buf)
{
	char *p;
	char name[BUF_LEN], val[BUF_LEN], *t, delim;
	size_t len;

	len = strlen(buf);
	if(len == 0)
		return 0;

	p = strchr(buf, '#');
	if(p)
	{
		/* throw away everything beyond the '#' */
		*p = '\0';
		len = p - buf;
		if(len == 0)
			return 0;
	}
	p = strchr(buf, '=');
	if(p == NULL)
	{
		while(*buf)
		{
			if(!isspace(*buf))
			{
				sprintf(errbuf, "parse error: missing '='");
				return -1;
			}
			buf++;
		}
	}

	if(strchr(p + 1, '='))
	{
		sprintf(errbuf, "parse error: multiple '='");
		return -1;
	}

   /* get the name */
	t = name;
	p = buf;
	SKIP_WS(p);
	while(*p != '=')
	{
		*t++ = *p++;
		if(isspace(*p))
		{
			SKIP_WS(p);
			break;
		}
	}
	if(*p != '=')
	{
		sprintf(errbuf, "parse error: illegal assignment");
		return -1;
	}

	*t = '\0';
	p++;

   /* get the value */
	t = val;
	SKIP_WS(p);
	if(*p == '\'' || *p == '"')
	{
		/* we have a string */
		delim = *p++;
		while(*p && *p != delim)
		{
			if(*p == '\\' && (*(p + 1)) && (*(p + 1)) == delim)
			{
				  /* quoting */
				p++;
				*t++ = *p++;
				continue;
			}
			*t++ = *p++;
		}
		if(*p != delim)
		{
			sprintf(errbuf, "parse error: missing '%c'", delim);
			return -1;
		}

		*t = '\0';
	}
	else
	{
		/* not a quoted string */
		while(*p && !isspace(*p))
		{
			*t++ = *p++;
		}
		*t = '\0';
	}

	return add_option(name, val);
}
#undef SKIP_WS

/*
 * parses config file 'filename'
 * returns 0 on success, -1 on failure
 */
int pconfig_parse(const char *filename)
{
	FILE *fp;
	char *p;
	int line_num;

	fp = fopen(filename, "rt");
	if(fp == NULL)
		return -1;

	line_num = 0;
	while (1)
	{
		/* get the line */
		p = get_line(fp);
		if(p == NULL)
		{

			if(feof(fp))
			{
				fclose(fp);
				break;
			}
			else
			{
				fclose(fp);
				return -1; /* error occured */
			}
		}

		line_num++;
		/* parse this line */
		if(parse_line(p) < 0)
		{
			sprintf(errbuf + strlen(errbuf), " on line %d", line_num);
			fclose(fp);
			return -1;
		}

		free(p);
	}
	return 0;
}

/*
 * allocates the hash table and sets up options
 * returns 0 on success, -1 on failure
 */
int pconfig_init(unsigned int hash_size, const pconfig_option_t *opts)
{
	pconfig_opt_t *np;
	unsigned int i, t;

   /* first allocate hash table */
	if(hash_size == 0)
		hash_size = 109; /* default hash table size */

	opt_table = malloc(sizeof(pconfig_opt_t *) * hash_size);
	if(opt_table == NULL)
		return -1;
	pconfig_hashsize = hash_size;
	for(i = 0; i < hash_size; i++)
		opt_table[i] = NULL;

   /* set up options */
	while(opts->type)
	{
		np = malloc(sizeof(pconfig_opt_t));
		if(np == NULL)
			return -1;

		np->name = strdup(opts->name);
		np->type = opts->type;
		switch(opts->type)
		{
		case PCONFIG_STRING:
			np->val.str_val = strdup(opts->default_value);
			break;
		case PCONFIG_INT:
			np->val.int_val = atoi(opts->default_value);
			break;
		default:
			 /* this shoud never happen */
			free(np);
			return -1;
			break;
		}
		/* add this option to the hash table */
		t = hash(np->name);
		np->next = opt_table[t];
		opt_table[t] = np;
		opts++;
	}
	return 0;
}

void pconfig_quit()
{
	pconfig_opt_t *p, *t;
	unsigned int i;

   /* free all allocated stuff */
	for(i = 0; i < pconfig_hashsize; i++)
	{
		p = opt_table[i];
		while(p)
		{
			t = p->next;
			if(p->type == PCONFIG_STRING)
				free(p->val.str_val);
			free(p->name);
			free(p);
			p = t;
		}
	}
	free(opt_table);
}

void pconfig_dump()
{
	pconfig_opt_t *p;
	unsigned int i;

	for(i = 0; i < pconfig_hashsize; i++)
	{
		p = opt_table[i];
		while(p)
		{
			if(p->type == PCONFIG_STRING)
				printf("%s = %s\n", p->name, p->val.str_val);
			else
				printf("%s = %d\n", p->name, p->val.int_val);
			p = p->next;
		}
	}
}

int pconfig_get_int(const char *name)
{
	pconfig_opt_t *t;

	t = opt_table[hash(name)];
	assert(t != NULL);
	while(strcmp(t->name, name) != 0)
		t = t->next;
	return t->val.int_val;
}

const char *pconfig_get_string(const char *name)
{
	pconfig_opt_t *t;

	t = opt_table[hash(name)];
	assert(t != NULL);
	while(strcmp(t->name, name) != 0)
		t = t->next;
	return t->val.str_val;
}

const char *pconfig_error()
{
	return errbuf;
}
