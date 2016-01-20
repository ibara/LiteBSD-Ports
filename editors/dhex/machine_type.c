#include <stdio.h>
#include "machine_type.h"

tUInt64	getfilepos(tFptr f)
{
	tUInt64	x;
	x=ftell(f);
	return x;
}
tUInt64 getfilesize(tFptr f)
{
	tUInt64 x;
	fseek(f,0,SEEK_END);
	x=getfilepos(f);

	return x;	
	//return (tUInt64)ftell(f);	
}
void	setfilepos(tFptr f,tUInt64 pos)
{
	fseek(f,pos,SEEK_SET);	
}

