#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "search.h"
#include "buffers.h"
#include "machine_type.h"

void	clearsearch(tSearch* search)
{
	memset(search,0,sizeof(tSearch));
	search->forwardnotbackward=1;
}
tInt8	searchfor(tSearch* search,tBuffer* buf,tUInt64* cursorpos,tBool nextnotprev)
{
	tInt64	actcursorpos=*cursorpos;
	tInt64	oldcursorpos=*cursorpos;
	tBool	forward;
	tBool	done;
	tBool	found=0;
	tFptr	fwlog;
	tFptr	frlog;
	unsigned char c;
	tInt32	idx;
	tBool	increment;
	tUInt64	searched=0;
	int i;


	forward=(search->forwardnotbackward==nextnotprev);
	actcursorpos=actcursorpos+(forward?1:-1);
	done=0;
	frlog=NULL;
	fwlog=NULL;
	if (buf->bufsize<search->searchlen) oldcursorpos=0;
	if (search->readsearchlog)
	{
		frlog=fopen(search->readlogfilename,"rb");

		if (frlog==NULL) done=1;
		else if (feof(frlog)) done=1;
	}
	if (search->writesearchlog)
	{
		fwlog=fopen(search->writelogfilename,"wb");
	
		if (fwlog==NULL) done=1;
		else {
			fprintf(fwlog,"#DHEX SEARCHLOG\n");
			fprintf(fwlog,"#VERSION 0\n");
			fprintf(fwlog,"#Search was for");
			for (i=0;i<search->searchlen;i++)
			{
				fprintf(fwlog," %02x",((unsigned int)search->searchstring[i])&0xff);
			}		
			fprintf(fwlog,"\n");
			fprintf(fwlog,"#lines are all in hex\n");
		}
	}
	increment=!search->writesearchlog;
	while (!done && buf->bufsize)
	{
		found=0;
		if (searched++>=buf->bufsize) done=1;	// avoid any theoretical infinite loops
		if (!search->readsearchlog)
		{
			if (increment)
			{
				actcursorpos=actcursorpos+(forward?1:-1);
				if (actcursorpos<0) actcursorpos=buf->bufsize-search->searchlen-1;
				if ((actcursorpos+search->searchlen)>=buf->bufsize) actcursorpos=0;
				if (actcursorpos==oldcursorpos)	done=1;
			}
		} else {
			tUInt8	state=0;	// state 0: skip the spaces at the beginning
						// state 1: collect the number
						// state 2: read until the end of the line
			tUInt64	x=0;
			tBool	havenum;
			tInt64	firstsearchlogpos;

			firstsearchlogpos=search->lastsearchlogpos;
			
			setfilepos(frlog,search->lastsearchlogpos);
			havenum=0;
			while (!done && !havenum)
			{
				c=32;
				x=0;
				state=0;
				while (c>=32 && !done)
				{
					fread(&c,sizeof(char),1,frlog);
					done=feof(frlog);
					if (!done)
					{
						if (c=='#') state=2;
						if (c>='a' && c<='z') c-=32;
						if (state==0) if ((c>='0' && c<='9') || (c>='A' && c<='F')) state=1;
						if (state==1)
						{
							havenum=1;	// this is a valid line	
							if (c>='0' && c<='9') 
							{
								x<<=4;
								x|=(c-'0');
							}
							else if (c>='A' && c<='F')
							{
								x<<=4;
								x|=(c-'A'+10);
							} else state=2;
						}
					}
				}
			}
			actcursorpos=x-buf->baseaddr;	// FIXME
			search->lastsearchlogpos=getfilepos(frlog);
		}
		increment=1;
		idx=getbufferidx(buf,actcursorpos);
		if ((actcursorpos+search->searchlen)<=buf->bufsize)
			 found=(memcmp(&buf->data[idx],search->searchstring,search->searchlen)==0);
		else found=0;
		if (found)
		{
			search->occurancesfound++;
			search->lastoccurance=actcursorpos;
			if (search->writesearchlog)	fprintf(fwlog,"%016llx\n",(tUInt64)actcursorpos+buf->baseaddr); else done=1;
		}
	}
	if (frlog) fclose(frlog);
	if (fwlog) fclose(fwlog);
	if (found && !search->writesearchlog)
	{
		*cursorpos=actcursorpos;
	}
	return RETOK;
}
