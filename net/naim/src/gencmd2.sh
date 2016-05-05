#!/bin/sh

echo 'Automatically generated list of commands for naim.'
echo '#include "conio.c"' \
	| ${CPP} -DCONIOCPP -dD - \
	| sed 's/^CONIO\(....\)(\(.*\)).*$/\1,\2/g' \
	| ${AWK} -F ',' '{
		if ((inalia == 1) && ($1 != "ALIA")) {
			inalia = 0;
			if (numaliases > 0)
				printf("\n");
			indesc = 1;
		}
		if ((indesc == 1) && ($1 != "DESC")) {
			indesc = 0;
			inargs = 1;
			printf("  SYNTAX: /%s", funcn);
		}
		if ((inargs == 1) && ($1 != "AREQ") && ($1 != "AOPT")) {
			inargs = 0;
			for (i = 0; i < maxarg; i++)
				printf("]");
			printf("\n");
		}

		if ($1 == "FUNC") {
			funcn = $2;
			minarg = 0;
			maxarg = 0;
			funcwhere = "ANYWHERE";
			printf("\nCOMMAND: %s\n", $2);
			inalia = 1;
			numaliases = 0;
		} else if ($1 == "WHER")
			funcwhere = $2;
		else if ($1 == "ALIA") {
			if (numaliases == 0)
				printf("  ALIASES: %s", $2);
			else
				printf(", %s", $2);
			numaliases++;
		} else if ($1 == "DESC")
			printf("  DESCRIPTION: %s\n", $2);
		else if ($1 == "AREQ") {
			printf(" %s", $3);
			minarg++;
		} else if ($1 == "AOPT") {
			printf(" [%s", $3);
			maxarg++;
		}
	}'
