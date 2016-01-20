#ifndef BUFFERS_H
#define	BUFFERS_H

#include <stdio.h>
#include <stdlib.h>
#include "machine_type.h"
#include "config.h"
#include "datatypes.h"

// opens a buffer, bufnum has to be either 1 or 2
tUInt32 openbuf(tBuffer* hBuf,tUInt8 bufnum,char* filename);

// reads into the buffer, if the need arises
tUInt32 readbuf(tBuffer* hBuf,tInt64 pos);

// get the index of the file position within the buffer
tInt32	getbufferidx(tBuffer* hBuf,tInt64 pos);


tInt8   savechanges(tBuffer* hBuf);

#endif
