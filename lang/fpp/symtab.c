/* @(#)symtab.c 1.3 96/09/02 */
/*
 * ====================================================
 * Copyright (C) 1995 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <string.h>
#include "fpp.h"
#include "service.h"
#include "symtab.h"

struct _sym {
	int	type;
	int	flag;
	SymPtr	next;
	char	name[MAXNAMELEN+1];
	union {
		int	i;
		char	*s;
	} val;
};

static SymPtr symtab[MAXSYMTAB];
static mask[256];

typedef struct _mlist {
	struct _mlist *next;
	Sym	*sym;
} MList;

MList	*marked = NULL; 

/*************************************************** Functions */

void
sympush(Sym *s) {
	MList	*tmp;

	tmp = my_alloc(sizeof(MList),1);
	tmp->next = marked;
	tmp->sym = s;
	marked = tmp;
}

void
sympop() {
	MList	*tmp;
	
	CHECK(marked);
	tmp = marked;
	marked = marked->next;
	free(tmp);
}

int
symrecurse(SymPtr s) {
	MList	*p;
	
	CHECK(s);
	if (marked) {
		if (marked->sym == s) {
/* 			fppmess(WARN_MREC,s->name);	*/
			return 1;
		}
	}
	else 
		return 0;
	CHECK(marked);
	p = marked;
	if (p->sym) p = p->next;     /* `#define a(x) 1+a(x)` aren't recursed */
	for (;p&&p->sym;p=p->next) {
		if (s == p->sym) {
			fppmess(ERR_MREC,s->name);
			return 1;
		}
	}
	return 0;
}

void
symhide() {
	sympush(NULL);
}

void
symunhide() {
	CHECK(marked->sym == NULL);
	sympop();
}

/****************************************************************** Hash */
int factor[8] = { 11, 17, 13, 31, 59, 129, 83, 97};

int
hash(char *s, int n) {
	int	i;
	int	hs;

	hs = 0;
	if (n == 0) n = MAXNAMELEN;
	for (i=0;i<n;i++) {
		if (!*s) break;
		hs += factor[i%8]*(*s++);
	}
	return hs % MAXSYMTAB;
}


SymPtr
symget(char *p, int m) {
	Sym	*s;
	unsigned char *q;
	int	n;

	n = 1; q = (unsigned char*)p;
	do {
		if (!(mask[*q] & n))
			return NULL;
		n <<= 1;
		if (n == 0x8000) break;	/* not too long */
	} while (*q++);

	for(s=symtab[hash(p,0)];s;s=s->next) {
		if (!strncmp(p,s->name,MAXNAMELEN)) {
			if (s->type & m) 
				return s;
			else
				break;
		}
	}
	return NULL;
}

SymPtr
symgetm(char *p, int m) {
	Sym	*s;
	int	n;

	n = strlen(p);
	while (n) {
		for(s=symtab[hash(p,n)];s;s=s->next) {
			if (!strncmp(p,s->name,n)) {
				if (s->type & m) 
					return s;
				else
					break;
			}
		}
		n--;
	}
	return NULL;
}

char	*
symname(SymPtr s) {
	CHECK(s);
	return (char *)&s->name;
}

char	*
symvals(SymPtr s) {
	CHECK(s);
	return s->val.s;
}

int
symvali(SymPtr s) {
	CHECK(s);
	return s->val.i;
}

int
symtype(SymPtr s) {
	CHECK(s);
	return s->type;
}

int
symflag(SymPtr s) {
	CHECK(s);
	return s->flag;
}

void
symsetnm(char *name, char *val) {
	SymPtr	s;
	unsigned char	*p;
	int	i,m;

	i = hash(name,0);
	s = my_alloc(sizeof(Sym),1);
	s->type = CL_NM;
	strncpy(s->name,name,MAXNAMELEN);
	s->val.s = val;
	if (symgetm(name, CL_KEY))
		s->flag = 1;
	s->next = symtab[i];
	symtab[i] = s;

	m = 1; p = (unsigned char *)&s->name;
	do {
		mask[*p] |= m;
		m <<= 1;
	} while (*p++);
}

void
symsetkw(char *name, int val, int type) {
	SymPtr	s;
	unsigned char *p;
	int	i,j,m;

	j = 0;
	if (mmode & MOD_LCASE)
		j = 1;
	for (;j<2;j++) {
		s = my_alloc(sizeof(Sym),1);
		s->type = type;
		strncpy(s->name,name,MAXNAMELEN);
		if (j == 1)
			tolowcase(s->name);
		i = hash(s->name,0);
		s->val.i = val;
		s->next = symtab[i];
		symtab[i] = s;

		m = 1; p = (unsigned char *)&s->name;
		do {
			mask[*p] |= m;
			m <<= 1;
		} while (*p++);	
	}
}

void
symsetvals(SymPtr s,char *v) {
	s->val.s = v;
}

void
symdel(char *name) {
	SymPtr	s;

	s = symget(name, CL_NM);
	if (s) 
		s->name[0] = 0;
}
