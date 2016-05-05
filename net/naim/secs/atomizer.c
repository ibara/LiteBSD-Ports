/*  ___  ___  ___ ___
** / __|/ _ \/ __/ __| secs
** \__ \  __/ (__\__ \ Copyright 1998-2003 Daniel Reed <n@ml.org>
** |___/\___|\___|___/ Simple Embedded Client Scripting
*/
#include <naim/secs.h>

char	*firstatom(char *string, char *bounds) {
	assert(string != NULL);
	assert(bounds != NULL);
	assert(*bounds != 0);
	if (*string == 0)
		return(string);
	while (*string && (strchr(bounds, *string) == NULL))
		string++;
	if (*string) {
		*string = 0;
		string++;
	} else
		string = NULL;
	return(string);
}

char	*firstwhite(char *string) {
	char	inquote = 0;

	if (string == NULL)
		return(NULL);
	if (*string == 0)
		return(string);
	while (isspace(*string))
		string++;		// skip over leading whitespace
	while (*string && (inquote || (!isspace(*string)))) {
		if (*string == '"') {
			if (!inquote)
				inquote = 1;
			else
				break;
		}
		string++;		// then skip over the first word
	}

	if (*string) {
		*string = 0;
		string++;
		while (isspace(*string))
			string++;	// skip over trailing whitespace
	}
	if (*string == 0)
		string = NULL;
	return(string);
}

char	*atom(char *string) {
	if (string == NULL)
		return(NULL);
	while (isspace(*string))
		string++;		// skip over leading whitespace
	if (*string == '"')
		string++;
	return(string);
}
