#ifndef OUTPUT_H
#define	OUTPUT_H

#include <stdio.h>
#include <ncurses.h>
#include "buffers.h"
#include "machine_type.h"
#include "datatypes.h"

void initcolors();
void colorpair(tOutput* output,uicolors uicol,short fg,short bg,int attr);
void pairsinit(tOutput* output);
void setcolor(tOutput* output,uicolors col);
void printbuffersingle(tOutput* output,tBuffer* hBuf1,tInt64 cursorpos1,tUInt64 firstpos1,tUInt8 windowfield);
void printbufferdiff(tOutput* output,tBuffer* hBuf1,tBuffer* hBuf2,tInt64 cursorpos1,tInt64 cursorpos2);
void printmainmenu(tOutput* output,tBool diffmode);
void drawframe(tOutput* output,tInt16 y,tInt16 x,tInt8 h,tInt8 w,char* header);
void drawcenterframe(tOutput* output,tInt8 h,tInt8 w,char* header);
tInt32 movepositions(tInt64* cursorpos,tInt64* firstpos,tInt64 maxbufsize,tInt32 chars,tInt32 lines,tInt32 pages,tBool diffmode);

#endif
