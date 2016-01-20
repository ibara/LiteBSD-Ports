#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ncurses.h>
#include "machine_type.h"
#include "output.h"
#include "input.h"
#include "menu.h"
#include "search.h"
#include "correlation.h"

void searchmask(tOutput* output,tSearch* search,tBuffer* buf,tUInt64* cursorpos)
{
	tInt8	itemnums[10];
	tMenu	Menu1;
	tInt8	selected;
	tInt16	offsx;
	tInt16	offsy;
	int i;

	offsx=COLS/2-30;
	offsy=LINES/2-7;

	drawcenterframe(output,14,60,"Search...");
	clearMenu(&Menu1);

	newMenuItem(&Menu1,"Searchstring: (hex)",1,1,'S',0,&itemnums[0]);
	newMenuItem(&Menu1,"Searchstring: (asc)",3,1,0,0,&itemnums[1]);
	newMenuItem(&Menu1,"Forward",4,1,'F',0,&itemnums[2]);
	newMenuItem(&Menu1,"Backward",4,17,'B',0,&itemnums[3]);
	newMenuItem(&Menu1,"Write to searchlog",6,1,'W',0,&itemnums[4]);
	newMenuItem(&Menu1,"File:",7,1,0,0,&itemnums[5]);
	newMenuItem(&Menu1,"Read from searchlog",9,1,'R',0,&itemnums[6]);
	newMenuItem(&Menu1,"File:",10,1,0,0,&itemnums[7]);
	newMenuItem(&Menu1,"Go",13,1,'G',0,&itemnums[8]);
	newMenuItem(&Menu1,"Cancel",13,51,'C',1,&itemnums[9]);
	selected=-1;
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,offsy+2,offsx+1,"[                                                        ]");
	mvwprintw(output->win,offsy+3,offsx+38,"[                   ]");
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,offsy+4,offsx+11,"( )");
	mvwprintw(output->win,offsy+4,offsx+28,"( )");
	mvwprintw(output->win,offsy+6,offsx+22,"[ ]");
	mvwprintw(output->win,offsy+9,offsx+23,"[ ]");
	mvwprintw(output->win,offsy+7 ,offsx+7,"[                                                  ]");
	mvwprintw(output->win,offsy+10,offsx+7,"[                                                  ]");
	while (selected!=itemnums[8] && selected!=itemnums[9])
	{
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,offsy+4,offsx+12,"%c",search->forwardnotbackward?'o':' ');
		mvwprintw(output->win,offsy+4,offsx+29,"%c",search->forwardnotbackward?' ':'o');
		mvwprintw(output->win,offsy+6,offsx+23,"%c",search->writesearchlog?'X':' ');
		mvwprintw(output->win,offsy+9,offsx+24,"%c",search->readsearchlog?'X':' ');
		setcolor(output,COLOR_TEXT);
		for (i=0;i<19;i++)
		{
			if (i<search->searchlen)	mvwprintw(output->win,offsy+2,offsx+2+i*3,"%02x",(tUInt32)(search->searchstring[i]&0xff));
			else				mvwprintw(output->win,offsy+2,offsx+2+i*3,"  ");

			if (i<search->searchlen)	mvwprintw(output->win,offsy+3,offsx+39+i,"%c",(search->searchstring[i]>=32 && search->searchstring[i]<127)?search->searchstring[i]:'.');
			else 				mvwprintw(output->win,offsy+3,offsx+39+i," ");
		}

		selected=MenuInteract(output,&Menu1,offsy,offsx);
			
		if (selected==itemnums[0]) 
		{
			hexinput2(output,offsy+2,offsx+1,search->searchstring,&search->searchlen,19);
		}
		if (selected==itemnums[1])
		{
			char	tmp[32];
			memset(tmp,0,32);
			for (i=0;i<search->searchlen && i<19;i++) tmp[i]=(search->searchstring[i]>=32 && search->searchstring[i]<127)?search->searchstring[i]:'.';
			
			if (stringinput(output,offsy+3,offsx+38,tmp,19)==KEYENTER)
			{
				search->searchlen=strlen(tmp);
				memcpy(search->searchstring,tmp,19);	
			}
		}
		if (selected==itemnums[2]) search->forwardnotbackward=1;
		if (selected==itemnums[3]) search->forwardnotbackward=0;
		if (selected==itemnums[4]) search->writesearchlog=!search->writesearchlog;
		if (selected==itemnums[5]) 
		{
			stringinput(output,offsy+7,offsx+7,search->writelogfilename,50);
		}
		if (selected==itemnums[6]) search->readsearchlog=!search->readsearchlog;
		if (selected==itemnums[7]) 
		{
			stringinput(output,offsy+10,offsx+7,search->readlogfilename,50);
		}
		if (selected==itemnums[8])
		{
			searchfor(search,buf,cursorpos,1);
			if (search->writesearchlog)
			{
				drawcenterframe(output,14,60,"Found");
				setcolor(output,COLOR_TEXT);
				mvwprintw(output->win,offsy+13,offsx+1,"%lld occurances found",search->occurancesfound);
				getkey((tKeyTab*)output->pKeyTab,1);
			}
		}

	}	
}
tInt8 savedialog(tOutput* output,tBuffer* hBuf)
{
	tInt16	offsx;
	tInt16	offsy;
	tMenu	Menu1;
	tInt8	itemnums[3];
	tInt8	selected;

	offsx=COLS/2-13;
	offsy=LINES/2-2;

	drawcenterframe(output,3,26,"Save changes?");
	setcolor(output,COLOR_TEXT);
	mvwprintw(output->win,offsy+2,offsx+1,"Save %12i byte%s",hBuf->changesnum,(hBuf->changesnum==1)?"?":"s?");
	clearMenu(&Menu1);
	newMenuItem(&Menu1,"Yes",3,1,'Y',0,&itemnums[0]);		
	newMenuItem(&Menu1,"No",3,9,'N',0,&itemnums[1]);
	newMenuItem(&Menu1,"Cancel",3,19,'C',1,&itemnums[2]);
	selected=MenuInteract(output,&Menu1,offsy,offsx);
	if (selected==itemnums[2]) return RETNOK;
	if (selected==itemnums[0])
	{
		if (savechanges(hBuf)!=RETOK)
		{
			drawcenterframe(output,3,22,"ERROR");
			setcolor(output,COLOR_TEXT);
			mvwprintw(output->win,offsy+1,offsx+1,"Could not save\n");
			return	RETNOK;
		}
	}	
	return RETOK;
}
tInt8 correlationmask(tOutput* output,tCorrelation* correlation)
{
	tInt16	offsx;
	tInt16	offsy;
	tMenu	Menu1;
	tInt8	itemnums[10];
	tInt8	selected;
	tInt8	retval=RETOK;
	tBool	done=0;

	offsx=COLS/2-21;
	offsy=LINES/2-3;
	drawcenterframe(output,6,42,"Correlation");
	clearMenu(&Menu1);
	newMenuItem(&Menu1,"best match",1,5,'B',0,&itemnums[0]);
	newMenuItem(&Menu1,"longest match",2,5,'L',0,&itemnums[1]);
	newMenuItem(&Menu1,"minimum difference",3,5,'D',0,&itemnums[2]);
	newMenuItem(&Menu1,"upper diff limit",4,1,'S',0,&itemnums[3]);
	newMenuItem(&Menu1,"Go",5,1,'G',0,&itemnums[4]);
	newMenuItem(&Menu1,"Cancel",5,35,'C',1,&itemnums[5]);
	selected=itemnums[0];
	while (!done)
	{
		setcolor(output,COLOR_BRACKETS);
		mvwprintw(output->win,offsy+1,offsx+1,"( )");
		mvwprintw(output->win,offsy+2,offsx+1,"( )");
		mvwprintw(output->win,offsy+3,offsx+1,"( )");
		mvwprintw(output->win,offsy+4,offsx+22,"[                 ]");
		setcolor(output,COLOR_TEXT);
		switch(correlation->algorithm)
		{
			case CORR_BEST_MATCH:		mvwprintw(output->win,offsy+1,offsx+2,"o");break;
			case CORR_LONGEST_MATCH:	mvwprintw(output->win,offsy+2,offsx+2,"o");break;
			case CORR_MIN_DIFF:		mvwprintw(output->win,offsy+3,offsx+2,"o");break;
		}
		mvwprintw(output->win,offsy+4,offsx+23,"%17lli",correlation->start_mindiff);
		wrefresh(output->win);
		selected=MenuInteract(output,&Menu1,offsy,offsx);
		if (selected==itemnums[0])
		{
			correlation->algorithm=CORR_BEST_MATCH;
		} 
		if (selected==itemnums[1]) 
		{
			correlation->algorithm=CORR_LONGEST_MATCH;
		}
		if (selected==itemnums[2]) 
		{
			correlation->algorithm=CORR_MIN_DIFF;
		} 
		if (selected==itemnums[3]) 
		{
			decinput(output,offsy+4,offsx+22,&correlation->start_mindiff,18);
		}
		if (selected==itemnums[4]) 
		{
			done=1;
			retval=RETOK;
		}
		if (selected==itemnums[5]) {
			done=1;
			retval=RETNOK;
		}
	}
	return retval;
}
