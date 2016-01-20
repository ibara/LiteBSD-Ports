#ifndef HEXCALC_H
#define	HEXCALC_H
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "machine_type.h"
#include "output.h"

typedef struct	_thHexCalc
{
	tUInt64	values[32];
	char	operators[32];
	tInt8	valuenum;
} thHexCalc;
void	hexcalc(tOutput* output,thHexCalc* hHexCalc);

#endif
