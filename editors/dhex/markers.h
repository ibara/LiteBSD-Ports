#ifndef MARKERS_H
#define	MARKERS_H
#include "machine_type.h"
#include "output.h"
#include "datatypes.h"

void* initmarkers();
int	parsemarkerfile(tMarkers* markers,char* filename);
tInt8 gotomask(tOutput* output,tMarkers* markers,tUInt64* cursorpos,tInt64 baseaddr);


#endif

