/*
 * Copyright (c) 2004,2005 Jan Schaumann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <fts.h>
#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int compare(const FTSENT **, const FTSENT **);
static int printFile(char *);
static int printFiles(char *[]);
static void printHead(const char *);
static time_t updateTimestamp(const char *);
static void usage(void);
int main(int, char *[]);

#define NEWS_PATH	"/var/news"
#define NEWS_TIME	".news_time"

#ifndef LOGIN_NAME_MAX
#define LOGIN_NAME_MAX	32	/* max login name length incl. NUL */
#endif

/* flags */
int f_all;		/* print all items regardless of currency */
int f_name;		/* print names of current items without content */
int f_short;		/* print number of current items */

time_t nf_time;

/*
 * The news utility prints news items stored in /var/news.  When invoked
 * without any arguments, news prints the name and timestamp of each
 * current item followed by its contents, most recent first.
 */
int
main(int argc, char *argv[])
{
	int ch, len;
	char *home, *nf;

	extern int optind;

	f_all = f_name = f_short = 0;

#ifdef __NetBSD__
	setprogname(argv[0]);
#endif

	while ((ch = getopt(argc, argv, "ans")) != -1) {
		switch(ch) {
		case 'a':
			f_all = 1;
			break;
		case 'n':
			f_name = 1;
			f_short = 0;
			break;
		case 's':
			f_short = 1;
			f_name = 0;
			break;
		case '?':
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if (*argv && (f_all || f_name || f_short))
		errx(EXIT_FAILURE, "Options are not allowed with file names.");
		/* NOTREACHED */

	if ((home = getenv("HOME")) == NULL)
		errx(EXIT_FAILURE, "Unable to find HOME variable.");
		/* NOTREACHED */

	len = strlen(home) + strlen(NEWS_TIME) + 2;
	if ((nf = malloc(len)) == NULL)
		errx(EXIT_FAILURE, "Unable to allocate memory.");
		/* NOTREACHED */

	if ((snprintf(nf, len, "%s/%s", home, NEWS_TIME)) == -1)
		errx(EXIT_FAILURE, "Unable to prepare path.");
		/* NOTREACHED */

	nf_time = updateTimestamp(nf);
	free(nf);

	return printFiles(argv);
}

/*
 * print either all files given or all files considered ``current''
 */
int
printFiles(char **files)
{
	int num, retval;

	num = retval = 0;

	if (chdir(NEWS_PATH) == -1)
		err(EXIT_FAILURE, "Unable to access %s", NEWS_PATH);
		/* NOTREACHED */

	if (*files) {
		char *file;

		while ((file = *files++) != NULL)
			retval += printFile(file);
	} else {
		FTS *ftsp;
		FTSENT *p;
		char *dirs[2];
		char **retval;

		dirs[0] = NEWS_PATH;
		dirs[1] = NULL;
		num = 0;

		if ((ftsp = fts_open(dirs, FTS_PHYSICAL, compare)) == NULL)
			err(EXIT_FAILURE, "unable to crawl %s", NEWS_PATH);

		while ((p = fts_read(ftsp)) != NULL) {
			if ((p->fts_info == FTS_F) &&
					(f_all || (p->fts_statp->st_mtime > nf_time))) {
				if (f_short)
					num++;
				else
					retval += printFile(p->fts_name);
			}
		}
	}

	if (f_short) {
		if (num > 0)
			printf("%d news item%s\n", num, num > 1 ? "s." : ".");
		else
			printf("No news.\n");
		return 0;
	}

	return retval;
}

void
usage()
{
	(void)fprintf(stderr, "usage: news [-ans] [ items ]\n");
	exit(EXIT_FAILURE);
	/* NOTREACHED */
}

int
printFile(char *file)
{
	if (file) {
		char buf[1024];
		FILE *fs;

		if ((fs = fopen(file, "r")) == NULL) {
			fprintf(stderr, "Cannot open %s%s%s: %s\n",
					file[0] == '/' ? "" : NEWS_PATH,
					file[0] == '/' ? "" : "/", file,
					strerror(errno));
			return 1;
		}

		printHead(file);

		if (!f_name) {
			while (fgets(buf, 1024, fs) != NULL) {
				if (buf[strlen(buf)-1] == '\n')
					printf("   ");
				printf("%s", buf);
			}
			printf("\n");
		}
		fclose(fs);
	}
	return 0;
}

void
printHead(const char *file)
{
	struct stat sb;
	struct passwd *pw;
	char owner[LOGIN_NAME_MAX+2];

	if (stat(file, &sb) == -1) {
		warn("unable to stat %s", file);
		return;
	}

	if (((pw = getpwuid(sb.st_uid)) == NULL))
		(void)snprintf(owner, LOGIN_NAME_MAX+2, "(%d)", sb.st_uid);
	else
		(void)snprintf(owner, LOGIN_NAME_MAX+2, "(%s)", pw->pw_name);

	printf("%s %s %s", file, owner, ctime(&(sb.st_mtime)));
}

time_t
updateTimestamp(const char *nf)
{
	struct stat sb;
	time_t retval;

	if (stat(nf, &sb) == -1) {
		int fd;

		/* can't stat .news_time file, so create one and consider all
		 * entries ``current'' */
		f_all = 1;

		if ((fd = open(nf, O_CREAT |O_EXCL | O_TRUNC | O_WRONLY,
						S_IRUSR | S_IWUSR)) == -1)
			warn("unable to create %s", nf);
		else {
			struct timeval tv[2];
			struct timeval tv1;

			tv1.tv_sec = tv1.tv_usec = 0;
			tv[0] = tv[1] = tv1;
			
			if (utimes(nf, tv) == -1)
				warn("unable to update timestamp");
			retval = (time_t)0;
			close(fd);
		}
	} else
		retval = sb.st_mtime;

	if (!f_short)	
		if (utimes(nf, NULL) == -1)
			warn("unable to update timestamp");

	return retval;
}

int
compare(const FTSENT **a, const FTSENT **b)
{
	if ((*a)->fts_statp->st_mtime > (*b)->fts_statp->st_mtime)
		return -1;
	else if ((*a)->fts_statp->st_mtime == (*b)->fts_statp->st_mtime)
		return 0;
	else
		return 1;
}
