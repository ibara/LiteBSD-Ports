/* language.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

struct translation {
	int code;
	const char *name;
};

struct translation_desc {
	const struct translation *t;
};

unsigned char dummyarray[T__N_TEXTS];

#include "language.inc"

static unsigned char **translation_array[N_LANGUAGES][N_CODEPAGES];

int current_language;
static int current_lang_charset;

void init_trans(void)
{
	int i, j;
	for (i = 0; i < N_LANGUAGES; i++)
		for (j = 0; j < N_CODEPAGES; j++)
			translation_array[i][j] = NULL;
	set_language(-1);
}

void shutdown_trans(void)
{
	int i, j, k;
	for (i = 0; i < N_LANGUAGES; i++)
		for (j = 0; j < N_CODEPAGES; j++) if (translation_array[i][j]) {
			for (k = 0; k < T__N_TEXTS; k++) {
				unsigned char *txt = translation_array[i][j][k];
				if (txt &&
				    txt != cast_uchar translations[i].t[k].name &&
				    txt != cast_uchar translation_english[k].name)
					mem_free(txt);
			}
			mem_free(translation_array[i][j]);
		}
}

int get_default_language(void)
{
	static int default_language = -1;
	unsigned char *lang, *p;
	int i;
	if (default_language >= 0)
		return default_language;

	i = os_default_language();
	if (i >= 0)
		goto ret_i;

	lang = cast_uchar getenv("LANG");
	if (!lang) lang = cast_uchar "en";
	lang = stracpy(lang);
	lang[strcspn(cast_const_char lang, ".@")] = 0;
	if (!strcasecmp(cast_const_char lang, "nn_NO"))
		strcpy(cast_char lang, "no");
	for (p = lang; *p; p++) {
		if (*p >= 'A' && *p <= 'Z')
			*p += 'a' - 'A';
		if (*p == '_')
			*p = '-';
	}
search_again:
	for (i = 0; i < n_languages(); i++) {
		p = cast_uchar translations[i].t[T__ACCEPT_LANGUAGE].name;
		if (!p)
			continue;
		p = stracpy(p);
		p[strcspn(cast_const_char p, ",;")] = 0;
		if (!strcasecmp(cast_const_char lang, cast_const_char p)) {
			mem_free(p);
			mem_free(lang);
			goto ret_i;
		}
		mem_free(p);
	}
	if ((p = cast_uchar strchr(cast_const_char lang, '-'))) {
		*p = 0;
		goto search_again;
	}
	mem_free(lang);
	lang = stracpy(cast_uchar "en");
	goto search_again;

	ret_i:
	default_language = i;
	return i;
}

int get_default_charset(void)
{
	static int default_charset = -1;
	unsigned char *lang, *p;
	int i;
	if (default_charset >= 0)
		return default_charset;

	i = os_default_charset();
	if (i >= 0)
		goto ret_i;

	lang = cast_uchar getenv("LANG");
	if (!lang) {
		i = 0;
		goto ret_i;
	}
	if ((p = cast_uchar strchr(cast_const_char lang, '.'))) {
		p++;
	} else {
		if (strlen(cast_const_char lang) > 5 && !strcasecmp(strchr(cast_const_char lang, 0) - 5, "@euro")) {
			p = cast_uchar "ISO-8859-15";
		} else {
			int def_lang = get_default_language();
			p = cast_uchar translations[def_lang].t[T__DEFAULT_CHAR_SET].name;
			if (!p)
				p = cast_uchar "";
		}
	}
	i = get_cp_index(p);

	if (i < 0)
		i = 0;

	ret_i:
	default_charset = i;
	return i;
}

int get_current_language(void)
{
	if (current_language >= 0)
		return current_language;
	return get_default_language();
}

static inline int is_direct_text(unsigned char *text)
{
/* Do not compare to dummyarray directly - thwart some misoptimizations */
	unsigned char * volatile dm = dummyarray;
	return !(text >= dm && text < dm + T__N_TEXTS);
}

unsigned char *get_text_translation(unsigned char *text, struct terminal *term)
{
	unsigned char **current_tra;
	struct conv_table *conv_table;
	unsigned char *trn;
	int charset;
	int language_idx = get_current_language();
	if (!term) charset = 0;
	else if (term->spec) charset = term_charset(term);
	else charset = utf8_table;
	if (is_direct_text(text)) return text;
	if ((current_tra = translation_array[language_idx][charset])) {
		unsigned char *tt;
		if ((trn = current_tra[text - dummyarray])) return trn;
		tr:
		if (!(tt = cast_uchar translations[language_idx].t[text - dummyarray].name)) {
			trn = cast_uchar translation_english[text - dummyarray].name;
		} else {
			struct document_options l_opt;
			memset(&l_opt, 0, sizeof(l_opt));
			l_opt.plain = 0;
			l_opt.cp = charset;
			conv_table = get_translation_table(current_lang_charset, charset);
			trn = convert_string(conv_table, tt, (int)strlen(cast_const_char tt), &l_opt);
			if (!strcmp(cast_const_char trn, cast_const_char tt)) {
				mem_free(trn);
				trn = tt;
			}
		}
		current_tra[text - dummyarray] = trn;
	} else {
		if (current_lang_charset && charset != current_lang_charset) {
			current_tra = translation_array[language_idx][charset] = mem_alloc(sizeof (unsigned char **) * T__N_TEXTS);
			memset(current_tra, 0, sizeof (unsigned char **) * T__N_TEXTS);
			goto tr;
		}
		if (!(trn = cast_uchar translations[language_idx].t[text - dummyarray].name)) {
			trn = cast_uchar translation_english[text - dummyarray].name;
		}
	}
	return trn;
}

unsigned char *get_english_translation(unsigned char *text)
{
	if (is_direct_text(text)) return text;
	return cast_uchar translation_english[text - dummyarray].name;
}

int n_languages(void)
{
	return N_LANGUAGES;
}

unsigned char *language_name(int l)
{
	if (l == -1) return cast_uchar "default";
	return cast_uchar translations[l].t[T__LANGUAGE].name;
}

void set_language(int l)
{
	int i;
	unsigned char *cp;
	current_language = l;
	l = get_current_language();
	for (i = 0; i < T__N_TEXTS; i++) if (translations[l].t[i].code != i) {
		internal("Bad table for language %s. Run script synclang.", translations[l].t[T__LANGUAGE].name);
		return;
	}
	cp = cast_uchar translations[l].t[T__CHAR_SET].name;
	i = get_cp_index(cp);
	if (i == -1) {
		internal("Unknown charset for language %s.", translations[l].t[T__LANGUAGE].name);
		i = 0;
	}
	current_lang_charset = i;
}
