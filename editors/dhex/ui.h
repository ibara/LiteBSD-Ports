#ifndef UI_H
#define	UI_H
#include "machine_type.h"
#include "buffers.h"
#include "search.h"
#include "output.h"
#include "correlation.h"
void searchmask(tOutput* output,tSearch* search,tBuffer* buf,tUInt64* cursorpos);
tInt8 savedialog(tOutput* output,tBuffer* buf);
tInt8 correlationmask(tOutput* output,tCorrelation* correlation);
#endif
