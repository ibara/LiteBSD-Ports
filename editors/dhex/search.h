#ifndef	SEARCH_H
#define	SEARCH_H
#include "machine_type.h"
#include "buffers.h"
#include "datatypes.h"

void	clearsearch(tSearch* search);
tInt8	searchfor(tSearch* search,tBuffer* buf,tUInt64* cursorpos,tBool nextnotprev);

#endif
