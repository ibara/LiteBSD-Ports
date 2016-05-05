/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2004 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

void	secs_var_init(void) {
}

int	secs_var_set_str(secs_var_t *var, const char *val) {
	size_t	slen = 0;

	assert(var != NULL);
	assert(val != NULL);

	*(var->val_num) = 0;

	slen = strlen(val);
	if (*(var->val_str) == NULL) {
		*(var->val_str) = secs_mem_alloc(slen+1);
		var->length = slen;
	} else if (var->length < slen) {
		*(var->val_str) = secs_mem_realloc(*(var->val_str), slen+1);
		var->length = slen;
	}
	strncpy(*(var->val_str), val, var->length+1);
	return(1);
}

int	secs_var_set_int(secs_var_t *var, const char *value) {
	long int	ival = 0;
	size_t	slen = 0;
	int	numdigits = 0;
	register const char	*val = value;
	char	tmpbuf[11];
	long int	itmp;
	int	ret;

	assert(var != NULL);
	assert(val != NULL);

	while ((*val) && (isspace(*val) || ((*val == '0') && (*(val+1) != 0))))
		val++;
	if (!isdigit(*val))
		return(0);
	itmp = ival = atol(val);
	slen = strlen(val);
	do {
		numdigits++;
	} while ((itmp = (int)(itmp / 10)) > 0);
	assert(numdigits <= slen);
	if (slen == numdigits)
		ret = secs_var_set_str(var, val);
	else if (snprintf(tmpbuf, sizeof(tmpbuf), "%li", ival) == numdigits)
		ret = secs_var_set_str(var, tmpbuf);
	else
		ret = 0;
	*(var->val_num) = ival;
	return(ret);
}

int	secs_var_set_switch(secs_var_t *var, const char *val) {
	int	ret;

	assert(var != NULL);
	assert(val != NULL);

	if ((strcasecmp(val, "ON") == 0)
		|| (strcasecmp(val, "1") == 0)
		|| (strcasecmp(val, "YES") == 0)
		|| (strcasecmp(val, "TRUE") == 0)) {
		ret = secs_var_set_str(var, "1");
		*(var->val_num) = 1;
	} else if ((strcasecmp(val, "OFF") == 0)
		|| (strcasecmp(val, "0") == 0)
		|| (strcasecmp(val, "NO") == 0)
		|| (strcasecmp(val, "FALSE") == 0)) {
		ret = secs_var_set_str(var, "0");
		*(var->val_num) = 0;
	} else
		ret = 0;
	return(ret);
}

int	secs_var_set(secs_var_t *var, const char *val) {
	assert(var != NULL);
	assert(val != NULL);
	assert(var->set != NULL);
	return(var->set(var, val));
}

secs_var_t	*secs_var_find(secs_var_t *first, const char *name) {
	register secs_var_t	*var = first;

	assert(name != NULL);
	if (var == NULL)
		if ((var = secs_block_getroot()->variables) == NULL)
			return(NULL);
	do {
		if (strcasecmp(var->name, name) == 0)
			return(var);
	} while ((var = var->next) != NULL);
	return(NULL);
}

secs_var_t	*secs_var_find_n(secs_var_t *first, const char *name) {
	register secs_var_t	*var = first;

	assert(name != NULL);
	if (var == NULL)
		if ((var = secs_block_getroot()->variables) == NULL)
			return(NULL);
	do {
		if (strncasecmp(var->name, name, strlen(var->name)) == 0)
			return(var);
	} while ((var = var->next) != NULL);
	return(NULL);
}

secs_var_t	*secs_var_create(const char *const name,
	int (*varset)(secs_var_t *, const char *)) {
	secs_var_t	*var = NULL;
	size_t	slen = 0;

	assert(name != NULL);
	assert(varset != NULL);

	slen = strlen(name);
	var = secs_mem_alloc(sizeof(secs_var_t));
	var->name = secs_mem_alloc(slen+1);
	strncpy(var->name, name, slen+1);
	var->set = varset;
	var->val_str = &(var->_buf_str);
	var->_buf_str = NULL;
	var->val_num = &(var->_buf_num);
	var->_buf_num = 0;
	return(var);
}

int	secs_var_free(secs_var_t *var) {
	if (var == NULL)
		return(0);
	if (var->name != NULL) {
		secs_mem_free(var->name);
		var->name = NULL;
	}
	if (*(var->val_str) != NULL) {
		secs_mem_free(*(var->val_str));
		*(var->val_str) = NULL;
	}
	secs_mem_free(var);
	return(1);
}
