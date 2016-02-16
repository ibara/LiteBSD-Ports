/*
 * vi-wrapper	Should be installed as /bin/vi. This program first
 *		checks if /usr/bin/vi exists. If it does, it checks if
 *		it isn't a link to /bin/vi, and executes it.
 *
 *		If /usr/bin/vi isn't present it does the same for
 *		/bin/elvis-tiny.
 *
 *		Copyright 2000 Miquel van Smoorenburg miquels@cistron.nl
 *		License: GPL v2 or later.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define REALVI	"/usr/bin/vi"
#define TINYVI	"/bin/elvis-tiny"
#define WRAPVI	"/bin/vi"

/*
 *	Avoid stdio - it saves a few hundred bytes, and so keeps the
 *	size of the executable _just_ under 4096 bytes.
 */
#define err(x) write(2, x, sizeof(x) - 1)

int main(int argc, char **argv)
{
	struct stat	real, tiny, wrap;
	char		*r = NULL;
	int		e;

	if (stat(WRAPVI, &wrap) != 0) {
		err("vi: wrapper should be installed as " WRAPVI "\n");
		exit(1);
	}
	if (stat(REALVI, &real) == 0) {
		if (real.st_dev == wrap.st_dev && real.st_ino == wrap.st_ino) {
			err("vi: " REALVI
				" is the same as the wrapper in "
				WRAPVI "\n");
			exit(1);
		}
		execv(REALVI, argv);
		r = REALVI;
		e = errno;
	}
	if (stat(TINYVI, &tiny) == 0) {
		if (tiny.st_dev == wrap.st_dev && tiny.st_ino == wrap.st_ino) {
			err("vi: " TINYVI
				" is the same as the wrapper in "
				WRAPVI "\n");
			exit(1);
		}
		execv(TINYVI, argv);
		r = TINYVI;
		e = errno;
	}

	if (r) {
		errno = e;
		perror(r);
		exit(1);
	}
	err("vi: wrapper couldn't execute " REALVI
		" nor " TINYVI "\n");
	return 1;
}

