#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "machine_type.h"
#include "config.h"
#include "buffers.h"




// retval: OK
// 
tUInt32 openbuf(tBuffer* hBuf,tUInt8 bufnum,char* filename)
{
	unsigned int filenamelen=0;
	if (filename == NULL)
		return	RETNOK;

	filenamelen=strlen(filename);
	if (filenamelen>510)
		filenamelen=510;

	hBuf->changesnum=0;
	hBuf->valid=0;
	hBuf->fresh=1;
	hBuf->file=fopen(filename,"rb");
	memcpy(hBuf->filename,filename,filenamelen+1);

	hBuf->filename[511]=0;
	if (hBuf->file)
	{
		hBuf->valid=1;
		hBuf->filesize=getfilesize(hBuf->file);
		hBuf->bufsize=hBuf->filesize;
		hBuf->bufferpos=-1;
		return RETOK;
	} else return RETNOK;
}

tUInt32 readbuf(tBuffer* hBuf,tInt64 pos)
{
	tBool havetoread=0;
	tInt32 bytesread;
	if (hBuf->fresh || pos<hBuf->bufferpos || pos>(hBuf->bufferpos+BUFFERSIZE)) 
	{
		havetoread=1;
		hBuf->bufferpos=pos&(~(BUFFERSIZE-1));	// floor
	}
	else if ((hBuf->bufferpos+BUFFERMARGIN)>pos && hBuf->bufferpos>=(BUFFERSIZE/2))
	{
		havetoread=1;
		hBuf->bufferpos-=(BUFFERSIZE/2);
	}
	else if ((hBuf->bufferpos+BUFFERSIZE-BUFFERMARGIN)<pos && (hBuf->bufferpos+(BUFFERSIZE/2))<hBuf->bufsize && hBuf->bufsize>BUFFERSIZE)
	{
		havetoread=1;
		hBuf->bufferpos+=(BUFFERSIZE/2);
	}
	if (havetoread)
	{
		hBuf->fresh=0;
		if (hBuf->filesize>hBuf->bufferpos)
		{
			setfilepos(hBuf->file,hBuf->bufferpos);
			bytesread=fread(hBuf->data,sizeof(tUInt8),BUFFERSIZE,hBuf->file);
			if (bytesread!=BUFFERSIZE)
			{
				memset(&hBuf->data[bytesread],0,sizeof(tUInt8)*(BUFFERSIZE-bytesread));	// fill the rest with 0
			}
		} else {
			memset(hBuf->data,0,sizeof(tUInt8)*BUFFERSIZE);
		}
	}

	return RETOK;
}

tInt32	getbufferidx(tBuffer* hBuf,tInt64 pos)
{
	tInt32 retval=OK;
	if ((pos<(hBuf->bufferpos+BUFFERMARGIN)) || (pos>(hBuf->bufferpos+BUFFERSIZE-BUFFERMARGIN)))
	{
		retval=readbuf(hBuf,pos);
	}
	if (retval==OK)
	{
		retval=pos-(hBuf->bufferpos);	
	}
	return retval;
}
tInt8	savechanges(tBuffer* hBuf)
{
	int i;
	fclose(hBuf->file);
	if (hBuf->changesnum)
	{
		hBuf->file=fopen(hBuf->filename,"r+b");
		if (hBuf->file==NULL) return RETNOK;
		for (i=0;i<hBuf->changesnum;i++)
		{
			setfilepos(hBuf->file,hBuf->changes[i].pos);
			fwrite(&(hBuf->changes[i].after),sizeof(tUInt8),1,hBuf->file);
		}	
		fclose(hBuf->file);	
	}
	return RETOK;
}

