#ifndef CORRELATION_H
#define	CORRELATION_H
#include "machine_type.h"
#include "buffers.h"
#include "output.h"
#include "datatypes.h"

void find_correlation(tOutput* output,tCorrelation* correlation,tBuffer* buf1,tBuffer* buf2,tInt64* cursorpos1,tInt64* cursorpos2);
void clear_correlation(tCorrelation* correlation);

#endif
