#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char *poptarg = NULL;
int poptindex = 0;

typedef struct {
	char *name;
	int index;
	int requires_arg;
}
poption_t;

static poption_t *option_list = NULL;
static int noptions = 0;
static int first_nonopt = 0;

static void build_option_list(const char *opt_spec)
{
	const char *p, *r;
	int arg, i;

	p = opt_spec;
	while(*p)
	{
		if(isspace(*p)) {
			p++;
			continue;
		}

		r = p;
		arg = 0;
		while(*r && !isspace(*r) && *r != ':')
			r++;

		if(*r == ':')
			arg = 1;

		noptions++;
		option_list = realloc(option_list, sizeof(poption_t) * noptions);
		option_list[noptions - 1].requires_arg = arg;
		option_list[noptions - 1].name = malloc(r - p + 1);
		option_list[noptions - 1].index = noptions - 1;

		i = 0;
		while(p != r)
			option_list[noptions - 1].name[i++] = *p++;
		option_list[noptions - 1].name[i] = '\0';

		if(*p)
			p++;
		if(*p)
			p += arg;
	}
}

static void free_option_list()
{
	int i;

	for(i = 0; i < noptions; i++)
		free(option_list[i].name);
	free(option_list);
}

static poption_t *find_option(const char *str)
{
	int i;

	for(i = 0; i < noptions; i++)
	{
		if(strcmp(option_list[i].name, str) == 0)
			return &option_list[i];
	}
	return NULL;
}

/* reorder the arguments */
static void reorder_args(int argc, char **argv)
{
	int i, j;
	int nc[3] = { 0 };
	char buf[3];
	char **op_w, **op, **nop;
	poption_t *p;

	op = op_w = nop = NULL;
	for(i = 1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			p = find_option(argv[i]);
			if(p == NULL)
			{
				/* check multiple options */
				const char *t = argv[i];
				
				if(!isalpha(*++t))
				{
					fprintf(stderr, "%s: option `%s' not recognized\n", argv[0], argv[i]);
					exit(1);	/* no need to free anything */
				}
				
				while(*t)
				{
					buf[0] = '-';
					buf[1] = *t;
					buf[2] = '\0';
					p = find_option(buf);
					if(p == NULL)
					{
						fprintf(stderr, "%s: option `%c' not recognized\n", argv[0], *t);
						exit(1);	/* no need to free anything */
					}
					if(p->requires_arg)
					{
						fprintf(stderr, "%s: option `%c' requires an argument and can't be put"
								" together with other options\n", argv[0], *t);
						exit(1);	/* no need to free anything */
					}
					t++;
				}

				/* treat as normal option */
				nc[0]++;
				op = realloc(op, sizeof(char *) * nc[0]);
				op[nc[0] - 1] = argv[i];
			}
			else if(p->requires_arg)
			{
				/* option that takes an argument */
				if(i == argc - 1 || find_option(argv[i + 1]) != NULL)
				{
					fprintf(stderr, "%s: option `%s' requires an argument\n", argv[0], p->name);
					exit(1);	/* no need to free anything */
				}

				nc[1] += 2;
				op_w = realloc(op_w, sizeof(char *) * nc[1]);
				op_w[nc[1] - 2] = argv[i];
				op_w[nc[1] - 1] = argv[++i];
			}
			else
			{
				/* normal option */
				nc[0]++;
				op = realloc(op, sizeof(char *) * nc[0]);
				op[nc[0] - 1] = argv[i];
			}
		}
		else
		{
			/* normal argument */
			nc[2]++;
			nop = realloc(nop, sizeof(char *) * nc[2]);
			nop[nc[2] - 1] = argv[i];
		}
	}

	j = 1;
	for(i = 0; i < nc[0]; i++, j++)
		argv[j] = op[i];
	for(i = 0; i < nc[1]; i++, j++)
		argv[j] = op_w[i];

	first_nonopt = j;
	for(i = 0; i < nc[2]; i++, j++)
		argv[j] = nop[i];

	free(op);
	free(op_w);
	free(nop);
}

int pgetopt(int argc, char **argv, const char *opt_spec)
{
	static const char *prev_opt_spec;
	static int cur_index,im;
	char buf[3];
	static const char *multiple;
	poption_t *opt;

	if(prev_opt_spec != opt_spec)
	{
		if(option_list)
			free_option_list();
		build_option_list(opt_spec);
		reorder_args(argc, argv);
		prev_opt_spec = opt_spec;
		cur_index = 1;
	}

	if(cur_index >= first_nonopt)
	{
		poptindex = cur_index;
		return -1;
	}

beg:
	if(im)
	{
		buf[0] = '-';
		buf[1] = *multiple++;
		buf[2] = '\0';
		if(*multiple == '\0')
		{
			multiple = 0;
			im = 0;
			cur_index++;
		}
		opt = find_option(buf);
		if(opt)
			return opt->index;
		else
			return -1;
	}

	opt = find_option(argv[cur_index]);

	if(opt == NULL)
	{
		/* this is multiple option */
		multiple = argv[cur_index] + 1;
		im = 1;
		goto beg;
	}

	if(opt->requires_arg)
	{
		poptarg = argv[cur_index + 1];
		cur_index++;
	}

	cur_index++;
	return opt->index;
}
