#ifndef INPUT_H
#define	INPUT_H
#include <stdio.h>
#include <stdlib.h>
#include "machine_type.h"
#include "output.h"
#include <ncurses.h>
#include "datatypes.h"

#define KEYF1           0x0100
#define KEYF2           0x0101
#define KEYF3           0x0102
#define KEYF4           0x0103
#define KEYF5           0x0104
#define KEYF6           0x0105
#define KEYF7           0x0106
#define KEYF8           0x0107
#define KEYF9           0x0108
#define KEYF10          0x0109
#define KEYESC          0x010a
#define KEYBACKSPACE    0x010b
#define KEYDEL          0x010c
#define KEYENTER        0x010d
#define KEYTAB          0x010e
#define KEYUP           0x010f
#define KEYDOWN         0x0110
#define KEYLEFT         0x0111
#define KEYRIGHT        0x0112
#define KEYPGUP         0x0113
#define KEYPGDOWN       0x0114
#define KEYHOME         0x0115
#define KEYEND          0x0116

#define NUM_SPECIALKEYS	0x17


void initkeytab(tOutput* output);
tInt16 getkey(tKeyTab* pKeyTab,tBool inputfield);
tInt16 decinput(tOutput* output,tInt16 y,tInt16 x,tUInt64* val,tInt16 len);
tInt16 hexinput(tOutput* output,tInt16 y,tInt16 x,tUInt64* val,char* relative,tInt16 len);
tInt16 hexinput2(tOutput* output,tInt16 y,tInt16 x,char* s,tInt16* usedlen,tInt16 len);
tInt16 stringinput(tOutput* output,tInt16 y,tInt16 x,char* s,tInt16 len);
int configkeytab(tKeyTab* pKeyTab,char* line);
void keyboardsetup(tOutput* output,char* configfilename);

#endif
