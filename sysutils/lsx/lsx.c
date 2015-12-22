/* (C)opyright MMVI Anselm R. Garbe <garbeam at gmail dot com>
 * See LICENSE file for license details.
 */
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main(int argc, char *argv[]) {
	int i;
	struct dirent *dp;
	struct stat s;
	DIR *dir;

	if((argc > 1) && !strncmp(argv[1], "-v", 3)) {
		fputs("lsx-"VERSION", (C)opyright MMVI Anselm R. Garbe\n", stdout);
		exit(EXIT_SUCCESS);
	}
	for(i = 0; i < argc; i++)
		if((dir = opendir(argv[i]))) {
			fchdir(dirfd(dir));
			do
				if((dp = readdir(dir))
					&& (stat(dp->d_name, &s) != -1)
					&& S_ISREG (s.st_mode)
					&& !access(dp->d_name, X_OK))
					puts(dp->d_name);
			while(dp);
			closedir(dir);
		}
	return 0;
}
