#include "correlation.h"
#include "buffers.h"
#include <strings.h>
#include <string.h>
void find_longestmatch(tOutput* output,tCorrelation* correlation,tBuffer* buf1,tBuffer* buf2,tInt64* cursorpos1,tInt64* cursorpos2)
{
	tInt64	idx1;
	tInt64	idx2;
	tInt64	match;
	tInt64	sumpos;
	tInt64	pos1;
	tInt64	pos2;
	tInt16	offsx;
	tInt16	offsy;
	WINDOW* smallwin=NULL;
	tInt64	diff;
	tBool	found=0;

	
	if (output)
	{
		offsx=COLS/2-22;
		offsy=LINES/2-3;
		drawcenterframe(output,6,44,"correlating...");
		setcolor(output,COLOR_TEXT);
		smallwin=subwin(output->win,6,19,offsy,offsx+24);
		mvwprintw(output->win,offsy+1,offsx+1,"pos1:");
		mvwprintw(output->win,offsy+4,offsx+1,"longest match:");
		mvwprintw(output->win,offsy+5,offsx+1,"at");
		if (smallwin) mvwprintw(smallwin,4,1,"%18lli",0);
		wrefresh(output->win);
	}
	
	correlation->bestmatch=0;
	correlation->mindiff=MAX(buf1->bufsize,buf2->bufsize);
	for (pos1=-(tInt64)buf2->bufsize;pos1<(tInt64)buf1->bufsize;pos1++)
	{
		match=0;
		diff=0;
		if (smallwin) mvwprintw(smallwin,1,3,"%16lli",((tInt64)buf1->bufsize-pos1));
		else fprintf(stderr,"%16lli\r",((tInt64)buf1->bufsize-pos1));
		if (smallwin) wrefresh(smallwin);
		for (pos2=0;pos2<buf2->bufsize;pos2++)
		{
			sumpos=pos1+pos2;
			if (sumpos>=0 && sumpos<buf1->bufsize)
			{
				idx1=getbufferidx(buf1,sumpos);
				idx2=getbufferidx(buf2,pos2);

				if (buf1->data[idx1]==buf2->data[idx2]) match++;
				else {
					diff++;
					if (match>correlation->bestmatch)
					{
						correlation->bestmatch=match;
						correlation->bestmatchpos=pos1;
						correlation->mindiff=diff;
						if (smallwin) mvwprintw(smallwin,4,1,"%18lli",match);
						if (smallwin) mvwprintw(smallwin,5,3,"%16llx",pos1);
						if (smallwin) wrefresh(smallwin);
						found=1;
					}
					match=0;
				}
				if (match>correlation->bestmatch)
				{
					correlation->bestmatch=match;
					correlation->bestmatchpos=pos1;
					correlation->mindiff=diff;
					if (smallwin) mvwprintw(smallwin,4,1,"%18lli",match);
					if (smallwin) mvwprintw(smallwin,5,3,"%16llx",pos1);
					if (smallwin) wrefresh(smallwin);
					found=1;
				}
			}
		}
		if (smallwin) wrefresh(smallwin);
	}
	if (found)
	{
		if (correlation->bestmatch>=0)
		{
			*cursorpos1=correlation->bestmatchpos;
			*cursorpos2=0;
		} else {
			*cursorpos1=0;
			*cursorpos2=-correlation->bestmatchpos;
		}
	}
	if (smallwin) delwin(smallwin);
}
void find_bestmatch(tOutput* output,tCorrelation* correlation,tBuffer* buf1,tBuffer* buf2,tInt64* cursorpos1,tInt64* cursorpos2)
{
	tInt64	idx1;
	tInt64	idx2;
	tInt64	match;
	tInt64	sumpos;
	tInt64	pos1;
	tInt64	pos2;
	tInt16	offsx;
	tInt16	offsy;
	WINDOW* smallwin=NULL;
	tInt64	diff;
	tBool	found=0;

	
	if (output)
	{
		offsx=COLS/2-22;
		offsy=LINES/2-3;
		drawcenterframe(output,6,44,"correlating...");
		setcolor(output,COLOR_TEXT);
		smallwin=subwin(output->win,6,19,offsy,offsx+24);
		mvwprintw(output->win,offsy+1,offsx+1,"pos1:");
		mvwprintw(output->win,offsy+4,offsx+1,"best match:");
		mvwprintw(output->win,offsy+5,offsx+1,"at");
		if (smallwin) mvwprintw(smallwin,4,1,"%18lli",0);
		wrefresh(output->win);
	}
	
	correlation->bestmatch=0;
	correlation->mindiff=MAX(buf1->bufsize,buf2->bufsize);
	for (pos1=-(tInt64)buf2->bufsize;pos1<(tInt64)buf1->bufsize;pos1++)
	{
		match=0;
		diff=0;
		if (output) mvwprintw(smallwin,1,3,"%16lli",((tInt64)buf1->bufsize-pos1));
		else fprintf(stderr,"%16lli\r",((tInt64)buf1->bufsize-pos1));
		for (pos2=0;pos2<buf2->bufsize;pos2++)
		{
			sumpos=pos1+pos2;
			if (sumpos>=0 && sumpos<buf1->bufsize)
			{
				idx1=getbufferidx(buf1,sumpos);
				idx2=getbufferidx(buf2,pos2);

				match+=(buf1->data[idx1]==buf2->data[idx2]);
				diff+=(buf1->data[idx1]!=buf2->data[idx2]);
			} else {
				diff++;
			}
		}
		if (match>correlation->bestmatch)
		{
			correlation->bestmatch=match;
			correlation->bestmatchpos=pos1;
			correlation->mindiff=diff;
			if (smallwin) mvwprintw(smallwin,4,1,"%18lli",match);
			if (smallwin) mvwprintw(smallwin,5,3,"%16llx",pos1);
			found=1;
		}
		if (output) wrefresh(smallwin);
	}
	if (found)
	{
		if (correlation->bestmatch>=0)
		{
			*cursorpos1=correlation->bestmatchpos;
			*cursorpos2=0;
		} else {
			*cursorpos1=0;
			*cursorpos2=-correlation->bestmatchpos;
		}
	}
	if (smallwin) delwin(smallwin);
}
void find_mindiff(tOutput* output,tCorrelation* correlation,tBuffer* buf1,tBuffer* buf2,tInt64* cursorpos1,tInt64* cursorpos2)
{
	tInt64	idx1;
	tInt64	idx2;
	tInt64	diff;
	tInt64	sumpos;
	tInt64	pos1;
	tInt64	pos2;
	tInt16	offsx;
	tInt16	offsy;
	WINDOW* smallwin=NULL;
	tBuffer* smallbuf;
	tBuffer* bigbuf;
	tInt64	match;
	tBool	found=0;

	
	
	if (output)
	{
		offsx=COLS/2-22;
		offsy=LINES/2-3;
		drawcenterframe(output,6,44,"correlating...");
		setcolor(output,COLOR_TEXT);
		smallwin=subwin(output->win,6,19,offsy,offsx+24);
		mvwprintw(output->win,offsy+1,offsx+1,"pos1:");
		mvwprintw(output->win,offsy+4,offsx+1,"mindiff:");
		mvwprintw(output->win,offsy+5,offsx+1,"at");
		mvwprintw(smallwin,4,1,"%18lli",correlation->start_mindiff);
		wrefresh(output->win);
	}
	
	smallbuf=(buf1->bufsize<buf2->bufsize)?buf1:buf2;
	bigbuf  =(buf1->bufsize<buf2->bufsize)?buf2:buf1;
	
	correlation->mindiff=correlation->start_mindiff;
	correlation->bestmatch=0;
	if (!correlation->mindiff) correlation->mindiff=bigbuf->bufsize;

	for (pos1=-(tInt64)smallbuf->bufsize;pos1<(tInt64)bigbuf->bufsize && correlation->mindiff;pos1++)
	{
		diff=0;
		match=0;
		if (smallwin) mvwprintw(smallwin,1,3,"%16lli",((tInt64)bigbuf->bufsize-pos1));
		else fprintf(stderr,"%16lli\r",((tInt64)buf1->bufsize-pos1));

		for (pos2=0;pos2<smallbuf->bufsize && diff<correlation->mindiff;pos2++)
		{
			sumpos=pos1+pos2;
			if (sumpos>=0 && sumpos<bigbuf->bufsize)
			{
				idx1=getbufferidx(bigbuf,sumpos);
				idx2=getbufferidx(smallbuf,pos2);

				diff+=(bigbuf->data[idx1]!=smallbuf->data[idx2]);
			} else {
				diff++;
			}
		}
		if (diff<correlation->mindiff)
		{
			correlation->mindiffpos=pos1;
			correlation->mindiff=diff;
			if (smallwin) mvwprintw(smallwin,4,1,"%18lli",diff);
			if (smallwin) mvwprintw(smallwin,5,3,"%16llx",pos1);
			found=1;
		}
		if (smallwin) wrefresh(smallwin);
	}
	if (found)
	{
		if (correlation->mindiffpos>=0)
		{
			*cursorpos1=(bigbuf==buf1)?correlation->mindiffpos:0;
			*cursorpos2=(bigbuf==buf1)?0:correlation->mindiffpos;
		} else {
			*cursorpos1=(bigbuf==buf1)?0:-correlation->mindiffpos;
			*cursorpos2=(bigbuf==buf1)?-correlation->mindiffpos:0;
		}
	}
	if (smallwin) delwin(smallwin);
}
void find_correlation(tOutput* output,tCorrelation* correlation,tBuffer* buf1,tBuffer* buf2,tInt64* cursorpos1,tInt64* cursorpos2)
{
	tInt16	ch=0;
	correlation->correlated=1;
	switch(correlation->algorithm)
	{
		case CORR_BEST_MATCH:		find_bestmatch(output,correlation,buf1,buf2,cursorpos1,cursorpos2);break;
		case CORR_LONGEST_MATCH:	find_longestmatch(output,correlation,buf1,buf2,cursorpos1,cursorpos2);break;
		case CORR_MIN_DIFF:		find_mindiff(output,correlation,buf1,buf2,cursorpos1,cursorpos2);break;
	}
	if (output)
	{
		while (ch!=ERR)
		{
			ch=getch();		// remove all the keys which have been pressed from the buffer
		}
	}
}
void clear_correlation(tCorrelation* correlation)
{
	memset(correlation,0,sizeof(tCorrelation));
	correlation->algorithm=CORR_BEST_MATCH;
}
