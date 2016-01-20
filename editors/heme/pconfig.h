#ifndef PCONFIG_H
# define PCONFIG_H

/* option types */
typedef enum
{
	PCONFIG_STRING = 1, PCONFIG_INT
}
pconfig_otype_t;

typedef struct
{
	pconfig_otype_t type;
	const char *name;
	const char *default_value;
}
pconfig_option_t;

/* prototypes */
int pconfig_parse(const char *filename);
int pconfig_init(unsigned int hash_size, const pconfig_option_t *opts);
void pconfig_quit();
void pconfig_dump();
int pconfig_get_int(const char *name);
const char *pconfig_get_string(const char *name);
const char *pconfig_error();

#endif /* PCONFIG_H */
