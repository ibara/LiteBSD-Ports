/*
 * Copyright (c) 2014, 2016 Brian Callahan <bcallah@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void 	usage(void);
static void 	version(void);

int
main(int argc, char *argv[])
{
	int 		ch, o, p;
	int 		b = 3;
	int 		d = 1;
	int 		t = 0;
	int 		z = 0;	/* For -t flag.  */

	while ((ch = getopt(argc, argv, "b:d:h:m:s:t:v")) != -1) {
		switch (ch) {
		case 'b':	/* Number of beeps at the end.  */
			b = atoi(optarg);
			break;
		case 'd':	/* Decrementer.  */
			d = atoi(optarg);
			break;
		case 'h':
			t += atoi(optarg) * 3600;
			break;
		case 'm':
			t += atoi(optarg) * 60;
			break;
		case 's':
			t += atoi(optarg);
			break;
		case 't':	/*
				 * Absolute time, in seconds.
				 * Takes precedence.
				 */
			z = atoi(optarg);
			break;
		case 'v':
			version();
			break;
		default:
			usage();
			/* NOTREACHED */
		}
	}
	argc -= optind;
	argv += optind;

	if (argc > 0)
		usage();

	/* Setting the -t flag overrides all other time settings.  */
	if (z > 0)
		t = z;

	if (b < 1 || b > 100) {
		fprintf(stderr, "Setting beeps to 3.\n");
		b = 3;
	}

	if (d < 1 || d > 86400) {
		fprintf(stderr, "Setting update interval to 1 second.\n");
		d = 1;
	}

        /* If you didn't set the time or an error set to 15 minutes.  */
	if (t < 1 || t > 86400) {
		fprintf(stderr, "Setting time to 15 minutes.\n");
		t = 900;
	}

	/* No need for fflush() everywhere.  */
	setvbuf(stdout, NULL, _IONBF, 0);

	/* Format the watch.  */
	o = t;
	p = d;

	do {
		if (p == d) {
			p = 0;
			if (t >= 3600)
				printf("\r%02d:%02d:%02d", (t / 3600), ((t % 3600) / 60), (t % 60));
			else if (t >= 60) {
				if (o >= 3600)
					printf("\r00:%02d:%02d", (t / 60), (t % 60));
				else
					printf("\r%02d:%02d", (t / 60), (t % 60));
			} else {
				if (o >= 3600)
					printf("\r00:00:%02d", t);
				else
					printf("\r00:%02d", t);
			}
		}

		sleep(1);
		p++;
	} while (--t > 0);

	/* Remove the watch. Only really needed for eight character watch.  */
	printf("\r        \r");

	do {
		printf("Beep!\a");
		sleep(1);
		if (--b > 0)
			fputc(' ', stdout);
	} while (b > 0);
	fputc('\n', stdout);

	return 0;
}

static void
usage(void)
{
	(void) fprintf(stderr,
		       "usage: egg [-b beeps] [-d seconds] [-h hours] [-m minutes] [-s seconds] [-t time] [-v]\n");
	exit(1);
}

static void
version(void)
{
	const char v[] = "egg version 4\nCopyright (c) 2014, 2016 Brian Callahan <bcallah@openbsd.org>\n\nPermission to use, copy, modify, and distribute this software for any\npurpose with or without fee is hereby granted, provided that the above\ncopyright notice and this permission notice appear in all copies.\n\nTHE SOFTWARE IS PROVIDED \"AS IS\" AND THE AUTHOR DISCLAIMS ALL WARRANTIES\nWITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF\nMERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR\nANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES\nWHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN\nACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF\nOR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.\n";

	(void) fprintf(stderr, v);
	exit(1);
}
