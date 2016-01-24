/* string.c
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#include "links.h"

int snprint(unsigned char *s, int n, my_uintptr_t num)
{
	my_uintptr_t q = 1;
	while (q <= num / 10) q *= 10;
	while (n-- > 1 && q) {
		*(s++) = (unsigned char)(num / q + '0');
		num %= q;
#ifdef __DECC_VER
		do_not_optimize_here(&q);	/* compiler bug */
#endif
		q /= 10;
	}
	*s = 0;
	return !!q;
}

int snzprint(unsigned char *s, int n, off_t num)
{
	off_t q = 1;
	if (n > 1 && num < 0) *(s++) = '-', num = -num, n--;
	while (q <= num / 10) q *= 10;
	while (n-- > 1 && q) {
		*(s++) = (unsigned char)(num / q + '0');
		num %= q;
#ifdef __DECC_VER
		do_not_optimize_here(&q);	/* compiler bug */
#endif
		q /= 10;
	}
	*s = 0;
	return !!q;
}

void add_to_strn(unsigned char **s, unsigned char *a)
{
	unsigned char *p;
	size_t l1 = strlen(cast_const_char *s), l2 = strlen(cast_const_char a);
	if (((l1 | l2) | (l1 + l2 + 1)) > MAXINT) overalloc();
	p = (unsigned char *)mem_realloc(*s, l1 + l2 + 1);
	strcat(cast_char p, cast_const_char a);
	*s = p;
}

void extend_str(unsigned char **s, int n)
{
	size_t l = strlen(cast_const_char *s);
	if (((l | n) | (l + n + 1)) > MAXINT) overalloc();
	*s = (unsigned char *)mem_realloc(*s, l + n + 1);
}

void add_bytes_to_str(unsigned char **s, int *l, unsigned char *a, size_t ll)
{
	unsigned char *p=*s;
	unsigned long old_length;
	unsigned long new_length;
	unsigned long x;

	old_length=*l;
	if (ll + old_length >= (unsigned)MAXINT / 2 || ll + old_length < (unsigned)ll) overalloc();
	new_length=old_length+ll;
	*l=(int)new_length;
	x=old_length^new_length;
	if (x>=old_length){
		/* Need to realloc */
		new_length|=(new_length>>1);
		new_length|=(new_length>>2);
		new_length|=(new_length>>4);
		new_length|=(new_length>>8);
		new_length|=(new_length>>16);
		p=(unsigned char *)mem_realloc(p,new_length+1);
		*s=p;
	}
	memcpy(p+old_length,a,ll);
	p[*l]=0;
}

void add_to_str(unsigned char **s, int *l, unsigned char *a)
{
	add_bytes_to_str(s, l, a, strlen(cast_const_char a));
}

void add_chr_to_str(unsigned char **s, int *l, unsigned char a)
{
	add_bytes_to_str(s, l, &a, 1);
}

void add_unsigned_long_num_to_str(unsigned char **s, int *l, my_uintptr_t n)
{
	unsigned char a[64];
	snprint(a, 64, n);
	add_to_str(s, l, a);
}

void add_num_to_str(unsigned char **s, int *l, off_t n)
{
	unsigned char a[64];
	snzprint(a, 64, n);
	add_to_str(s, l, a);
}

void add_knum_to_str(unsigned char **s, int *l, off_t n)
{
	unsigned char a[13];
	if (n && n / (1024 * 1024) * (1024 * 1024) == n) snzprint(a, 12, n / (1024 * 1024)), a[strlen(cast_const_char a) + 1] = 0, a[strlen(cast_const_char a)] = 'M';
	else if (n && n / 1024 * 1024 == n) snzprint(a, 12, n / 1024), a[strlen(cast_const_char a) + 1] = 0, a[strlen(cast_const_char a)] = 'k';
	else snzprint(a, 13, n);
	add_to_str(s, l, a);
}

long strtolx(unsigned char *c, unsigned char **end)
{
	long l;
	if (c[0] == '0' && upcase(c[1]) == 'X' && c[2]) l = strtol(cast_const_char(c + 2), (char **)(void *)end, 16);
	else l = strtol(cast_const_char c, (char **)(void *)end, 10);
	if (!*end) return l;
	if (upcase(**end) == 'K') {
		(*end)++;
		if (l < -MAXINT / 1024) return -MAXINT;
		if (l > MAXINT / 1024) return MAXINT;
		return l * 1024;
	}
	if (upcase(**end) == 'M') {
		(*end)++;
		if (l < -MAXINT / (1024 * 1024)) return -MAXINT;
		if (l > MAXINT / (1024 * 1024)) return MAXINT;
		return l * (1024 * 1024);
	}
	return l;
}

my_strtoll_t my_strtoll(unsigned char *string, unsigned char **end)
{
	my_strtoll_t f;
	errno = 0;
#if defined(HAVE_STRTOLL)
	f = strtoll(cast_const_char string, (char **)(void *)end, 10);
#elif defined(HAVE_STRTOQ)
	f = strtoq(cast_const_char string, (char **)(void *)end, 10);
#elif defined(HAVE_STRTOIMAX)
	f = strtoimax(cast_const_char string, (char **)(void *)end, 10);
#else
	f = strtol(cast_const_char string, (char **)(void *)end, 10);
#endif
	if (f < 0 || errno) return -1;
	return f;
}

/* Copies at most dst_size chars into dst. Ensures null termination of dst. */
unsigned char *safe_strncpy(unsigned char *dst, const unsigned char *src, size_t dst_size)
{
	size_t to_copy;
	if (!dst_size) return dst;
	to_copy = strlen(cast_const_char src);
	if (to_copy >= dst_size) to_copy = dst_size - 1;
	memcpy(dst, src, to_copy);
	memset(dst + to_copy, 0, dst_size - to_copy);
	return dst;
}

#ifdef JS
/* deletes all nonprintable characters from string */
void skip_nonprintable(unsigned char *txt)
{
	unsigned char *txt1=txt;

	if (!txt||!*txt)return;
	for (;*txt;txt++)
		switch(*txt)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
			break;

			case 9:
			*txt1=' ';
			txt1++;
			break;

			default:
			*txt1=*txt;
			txt1++;
			break;
		}
	*txt1=0;
}
#endif

/* case insensitive compare of 2 strings */
/* comparison ends after len (or less) characters */
/* return value: 1=strings differ, 0=strings are same */
int casecmp(const unsigned char *c1, const unsigned char *c2, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++) if (upcase(c1[i]) != upcase(c2[i])) return 1;
	return 0;
}

int casestrstr(const unsigned char *h, const unsigned char *n)
{
	const unsigned char *p;

	for (p=h;*p;p++)
	{
		if (!srch_cmp(*p,*n))  /* same */
		{
			const unsigned char *q, *r;
			for (q=n, r=p;*r&&*q;)
			{
				if (!srch_cmp(*q,*r)) r++,q++;    /* same */
				else break;
			}
			if (!*q) return 1;
		}
	}

	return 0;
}


