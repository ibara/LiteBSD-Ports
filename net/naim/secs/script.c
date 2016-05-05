/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1999-2004 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

void	set_echof(const char *, ...);

void	secs_script_init(void) {
}

char	*secs_script_expand(secs_block_t *block, const char *instr) {
	static char	*workstr = NULL;
	static signed int	worklen = 0;
	register const char
			*str;
	signed int	slen;

	if (instr == NULL)
		return(NULL);

	str = instr;
	slen = 0;

	if (block == NULL)
		block = secs_block_getroot();

	if (workstr == NULL) {
		assert(worklen == 0);
		workstr = secs_mem_alloc(256);
		worklen = 256;
	} else
		assert(worklen != 0);
	while (*str) {
		if (*str == '$') {
			secs_var_t *var = NULL;

			if ((var = secs_var_find_n(NULL, str+1)) != NULL) {
				char	*val = *(var->val_str);
				signed int	vallen = strlen(val);

				str += 1+strlen(var->name);
				assert((worklen - slen - vallen) <= worklen);
				while ((worklen - slen - vallen) < 2) {
					worklen += 256;
					workstr = secs_mem_realloc(workstr, worklen);
				}
				strncpy(workstr+slen, val, vallen);
				slen += vallen;
				continue;
			}
		}
		if ((worklen - slen) < 2) {
			worklen += 256;
			workstr = secs_mem_realloc(workstr, worklen);
		}
		workstr[slen++] = *str++;
	}
	assert(slen < worklen);
	assert((worklen - slen - 256) <= worklen);
	workstr[slen] = 0;
	if ((worklen - slen - 256) > 2) {
		worklen -= 256;
		workstr = secs_mem_realloc(workstr, worklen);
	}

	return(workstr);
}

typedef enum {
	COMP_UNKNOWN = 0,
	COMP_EQUAL,
	COMP_NEQUAL,
	COMP_LESS,
	COMP_LEQUAL,
	COMP_GREAT,
	COMP_GEQUAL,
	COMP_AND,
	COMP_OR
} comp_t;

int	secs_script_eval(char **line) {
	comp_t	comp = COMP_UNKNOWN;
	int	which = 0,
		val[3] = { 0, 0, 0 };

	while (**line != 0) {
		while (isspace(**line))
			(*line)++;

		switch (**line) {
			case '(':
				(*line)++;
				val[which++] = secs_script_eval(line);
				break;
			case ')':
				(*line)++;
				while (isspace(**line))
					(*line)++;
				if (which != 2) {
					set_echof("secs_script_eval: Hit end of expression before done finding values to compare.\n");
					return(0);
				}
				switch (comp) {
					case COMP_EQUAL:
						return(val[0] == val[1]);
					case COMP_NEQUAL:
						return(val[0] != val[1]);
					case COMP_LESS:
						return(val[0] <  val[1]);
					case COMP_LEQUAL:
						return(val[0] <= val[1]);
					case COMP_GREAT:
						return(val[0] >  val[1]);
					case COMP_GEQUAL:
						return(val[0] >= val[1]);
					case COMP_AND:
						return(val[0] && val[1]);
					case COMP_OR:
						return(val[0] || val[1]);
					case COMP_UNKNOWN:
						set_echof("secs_script_eval: Hit end of expression without finding operator.\n");
						return(0);
				}
			case '$': {
				secs_var_t	*var = NULL;

				if ((var = secs_var_find_n(NULL, *line+1)) != NULL) {
					(*line) += strlen(var->name)+1;
					val[which++] = *(var->val_num);
				} else {
					set_echof("secs_script_eval: Unknown variable: %s\n", *line);
					(*line)++;
					return(0);
				}
				break;
			}
			case '=':
				(*line)++;
				if (**line == '=') {
					(*line)++;
					comp = COMP_EQUAL;
				} else {
					set_echof("secs_script_eval: Unable to set variable values in a control block (maybe you meant ==).\n");
					return(0);
				}
				break;
			case '!':
				(*line)++;
				if (**line == '=') {
					(*line)++;
					comp = COMP_NEQUAL;
				} else if (**line == '<') {
					(*line)++;
					comp = COMP_GEQUAL;
				} else if (**line == '>') {
					(*line)++;
					comp = COMP_LEQUAL;
				} else {
					set_echof("secs_script_eval: ! must be followed by =, <, or >.\n");
					return(0);
				}
				break;
			case '<':
				(*line)++;
				if (**line == '=') {
					(*line)++;
					comp = COMP_LEQUAL;
				} else
					comp = COMP_LESS;
				break;
			case '>':
				(*line)++;
				if (**line == '=') {
					(*line)++;
					comp = COMP_GEQUAL;
				} else
					comp = COMP_GREAT;
				break;
			case '&':
				(*line)++;
				if (**line == '&') {
					(*line)++;
					comp = COMP_AND;
				} else {
					set_echof("secs_script_eval: Unable to perform boolean arithmetic in a control block (maybe you meant &amp;&amp;).\n");
					return(0);
				}
				break;
			case '|':
				(*line)++;
				if (**line == '|') {
					(*line)++;
					comp = COMP_OR;
				} else {
					set_echof("secs_script_eval: Unable to perform boolean arithmetic in a control block (maybe you meant ||).\n");
					return(0);
				}
				break;
			default: {
				char	*tmp;

				val[which] = strtol(*line, &tmp, 0);
				if (*line != tmp) {
					*line = tmp;
					which++;
				} else {
					set_echof("secs_script_eval: Unknown input: %s\n", *line);
					return(0);
				}
			}
		}
		if (which == 3) {
			set_echof("secs_script_eval: Too many values.\n");
			return(0);
		}
	}

	set_echof("secs_script_eval: error parsing line, end of string found before close parenthesis\n");
	return(0);
}

int	secs_script_parse(const char *line) {
	char	*strbuf = NULL, *str = NULL;
	size_t	slen = 0;

	if ((line == NULL) || ((slen = strlen(line)) == 0))
		return(0);
	strbuf = secs_mem_alloc(slen+2);
	if (strbuf == NULL)
		return(0);
	strncpy(strbuf, line, slen+2);
	while ((str = strchr(strbuf, '\n')) != NULL)
		*str = ' ';
	while ((str = strrchr(strbuf, ';')) != NULL)
		*str = 0;
	str = strbuf;
	while ((str != NULL) && (*str != 0)) {
		char	*workstr = str,
			*word = NULL;

		str += strlen(str)+1;	// firstwhite() munges str, so we
					// have to do this before processing
		if (*workstr == '/') {
			while ((word = strchr(workstr, '\001')) != NULL)
				*word = ';';
			secs_handle(secs_script_expand(NULL, workstr+1));
			continue;
		}
		word = atom(workstr);
		workstr = firstwhite(workstr);
		if (strcasecmp(word, "ECHO") == 0)
			set_echof("%s\n", secs_script_expand(NULL, workstr));
		else if (strcasecmp(word, "SET") == 0) {
			word = atom(workstr);
			workstr = firstwhite(workstr);
			secs_setvar(word, workstr);
		} else if (strcasecmp(word, "IF") == 0) {
			if (*workstr == '(') {
				workstr++;
				if (secs_script_eval(&workstr) != 0)
					secs_script_parse(workstr);
			} else
				set_echof("secs_script_parse: You need to"
					" include parenthesis around"
					" control blocks for if (%s)\n",
					workstr);
		}
	}
	secs_mem_free(strbuf);
	return(1);
}
