/*
 * Copy me if you can.
 * by 20h
 */

#ifndef IND_H
#define IND_H

#include <stdio.h>
#define nil NULL

extern int glfd;

typedef struct Elems Elems;
struct Elems {
	char **e;
	int num;
};

typedef struct Indexs Indexs;
struct Indexs {
	Elems **n;
	int num;
};

typedef struct filetype filetype;
struct filetype {
        char *end;
        char *type;
        void (* f)(int, char *, char *, char *, char *, char *, char *);
};

filetype *gettype(char *filename);
void *gmallocz(int l, int d);
char *gstrdup(char *str);
Indexs *scanfile(char *fname);
Elems *getadv(char *str);
void printelem(int fd, Elems *el, char *addr, char *port);
void addindexs(Indexs *idx, Elems *el);
void addelem(Elems *e, char *s);
void freeindex(Indexs *i);
void freeelem(Elems *e);
char *readln(int fd);
void tprintf(int fd, char *fmt, ...);
int initlogging(char *logf);
void stoplogging(int fd);
char *smprintf(char *fmt, ...);
char *reverselookup(char *host);

#endif

