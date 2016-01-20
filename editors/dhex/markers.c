#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "machine_type.h"
#include "menu.h"
#include "output.h"
#include "input.h"
#include "markers.h"

void* initmarkers()
{
	tMarkers* p;
	int i;
	p=malloc(sizeof(tMarkers));
	for (i=0;i<NUMMARKERS;i++)
	{
		p->relative[i]='=';
		p->cursorpos[i]=0;
	}
	return p;
}

int	parsemarkerfile(tMarkers* markers,char* filename)
{
	tFptr	f;
	char c;
	tMarkers*	tmp;
	int	markersnum=0;
	tInt8	state=0;	//	0=start of the line, waiting for the relativ tokens, or a number
			//	1=got relative
			//	2=reading in number
			//	3=end of line
	tBool	hadnumber=0;	
	tmp=initmarkers();
	f=fopen(filename,"r");
	if (!f)	return	RETNOK;
	while (!feof(f) && markersnum<10)
	{
		fread(&c,sizeof(char),1,f);
		if (c>='a' && c<='z') c=c-32;	// make the letters uppercase
		if (!feof(f) && c!=' ' && c!=9)	// ignore the spaces
		{
			if (c=='#')	state=3;	// the rest of this line is just a comment
			if (c<32)	//	newline
			{
				if (hadnumber)	
				{
					markersnum++;
					tmp->relative[markersnum]='=';
					tmp->cursorpos[markersnum]=0;
				}
				hadnumber=0;
				state=0;
			}
			if (state==1) state=2;	// the last character was the relative, now for the number
			if (state==0 && (c=='=' || c=='+' || c=='-'))	
			{
				tmp->relative[markersnum]=c;
				state=1;
			}
			if (state==0 && ((c>='A' && c<='F') || (c>='0' && c<='9'))) state=2;	// the number starts right here
			if (state==2)
			{
				tmp->cursorpos[markersnum]<<=4;
				hadnumber=1;
				if (c>='0' && c<='9') tmp->cursorpos[markersnum]+=(c-'0');
				else if (c>='A' && c<='F') tmp->cursorpos[markersnum]+=(c-'A'+10);
				else return RETNOK;	// illegal value
			}
		}	
	}
	fclose(f);
	memcpy(markers,tmp,sizeof(tMarkers));
	free(tmp);
	return	RETOK;
}

int	writemarkerfile(tMarkers* markers,char* filename)
{
	tFptr	f;
	int i;
	
	f=fopen(filename,"w");
	if (!f)	return	RETNOK;
	fprintf(f,"#DHEX MARKERS\n");
	fprintf(f,"#VERSION 0\n");
	fprintf(f,"#lines are all in hex\n");
	fprintf(f,"#relative positions are =, +, -\n");
	fprintf(f,"\n"); 
	for (i=0;i<NUMMARKERS;i++)
	{
		fprintf(f,"%c%016llX\n",markers->relative[i],markers->cursorpos[i]);
	}
	fclose(f);
	return	RETOK;	
}
tInt8 gotomask(tOutput* output,tMarkers* markers,tUInt64* cursorpos,tInt64 baseaddr)
{
	tUInt64 actcursorpos=*cursorpos+baseaddr;
	tUInt64 newcursorpos=*cursorpos+baseaddr;
	tInt8	itemnums[25];
	tInt8	selected;
	tMenu	Menu1;
	tInt16	offsx;
	tInt16	offsy;
	tInt8	retval=RETOK;
	char	buf[17];
	char	filename[64];


	int	i;
	tInt16	ch;

	memset(filename,0,64);

	offsx=COLS/2-30;
	offsy=LINES/2-10;

	drawcenterframe(output,17,60,"Goto...");
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,offsy+3,offsx+8,"[                 ]");
	clearMenu(&Menu1);
	newMenuItem(&Menu1,"To",3,1,'T',0,&itemnums[0]);
	newMenuItem(&Menu1,"Go",3,48,'G',0,&itemnums[1]);
	newMenuItem(&Menu1,"Cancel",3,53,'C',1,&itemnums[2]);

	newMenuItem(&Menu1,"Set", 5,1,0,0,&itemnums[ 3]);
	newMenuItem(&Menu1,"Set", 6,1,0,0,&itemnums[ 4]);
	newMenuItem(&Menu1,"Set", 7,1,0,0,&itemnums[ 5]);
	newMenuItem(&Menu1,"Set", 8,1,0,0,&itemnums[ 6]);
	newMenuItem(&Menu1,"Set", 9,1,0,0,&itemnums[ 7]);
	newMenuItem(&Menu1,"Set",10,1,0,0,&itemnums[ 8]);
	newMenuItem(&Menu1,"Set",11,1,0,0,&itemnums[ 9]);
	newMenuItem(&Menu1,"Set",12,1,0,0,&itemnums[10]);
	newMenuItem(&Menu1,"Set",13,1,0,0,&itemnums[11]);
	newMenuItem(&Menu1,"Set",14,1,0,0,&itemnums[12]);

	newMenuItem(&Menu1,"Mark 1", 5,7,'1',0,&itemnums[13]);
	newMenuItem(&Menu1,"Mark 2", 6,7,'2',0,&itemnums[14]);
	newMenuItem(&Menu1,"Mark 3", 7,7,'3',0,&itemnums[15]);
	newMenuItem(&Menu1,"Mark 4", 8,7,'4',0,&itemnums[16]);
	newMenuItem(&Menu1,"Mark 5", 9,7,'5',0,&itemnums[17]);
	newMenuItem(&Menu1,"Mark 6",10,7,'6',0,&itemnums[18]);
	newMenuItem(&Menu1,"Mark 7",11,7,'7',0,&itemnums[19]);
	newMenuItem(&Menu1,"Mark 8",12,7,'8',0,&itemnums[20]);
	newMenuItem(&Menu1,"Mark 9",13,7,'9',0,&itemnums[21]);
	newMenuItem(&Menu1,"Mark 0",14,7,'0',0,&itemnums[22]);


	newMenuItem(&Menu1,"Load Markers",16, 1,'L',0,&itemnums[23]);
	newMenuItem(&Menu1,"Save Markers",16,17,'S',0,&itemnums[24]);
	selected=-1;

	while (1)
	{
		setcolor(output,COLOR_FRAME);
		for (i=0;i<58;i++) mvwprintw(output->win,offsy+17,offsx+1+i," ");
		setcolor(output,COLOR_TEXT);
		if (retval!=RETOK) mvwprintw(output->win,offsy+17,offsx+1,"error opening file");
		for (i=0;i<NUMMARKERS;i++) 
		{
			setcolor(output,COLOR_BRACKETS);
			mvwprintw(output->win,offsy+5+i,offsx+14,"[                 ]");
			mvwprintw(output->win,offsy+5+i,offsx+41,"[                ]");
			setcolor(output,COLOR_TEXT);
			mvwprintw(output->win,offsy+5+i,offsx+36,"Diff:");

			snprintf(buf,17,"%c%llx",markers->relative[i],markers->cursorpos[i]);
			mvwprintw(output->win,offsy+5+i,offsx+32-strlen(buf),"%s",buf);
			mvwprintw(output->win,offsy+5+i,offsx+42,"%16llx",abs(markers->cursorpos[i]-actcursorpos));
		}
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,offsy+3,offsx+9,"%17llx",newcursorpos);
		selected=MenuInteract(output,&Menu1,offsy,offsx);
		if (selected==itemnums[0]) {
			newcursorpos=actcursorpos;
			hexinput(output,offsy+3,offsx+8,&newcursorpos,NULL,17);
		}
		if (selected==itemnums[1]) {*cursorpos=newcursorpos-baseaddr;return RETOK;} //FIXME
		if (selected==itemnums[2]) return RETNOK;

		for (i=0;i<NUMMARKERS;i++)
		{
			if (selected==itemnums[3+i])
			{ 
				if (markers->relative[i]=='=' && markers->cursorpos[i]==0) markers->cursorpos[i]=actcursorpos;
				hexinput(output,offsy+ 5+i,offsx+14,&markers->cursorpos[i],&markers->relative[i],17);
			}
			if (selected==itemnums[13+i]) 
			{
				if (markers->relative[i]=='=') newcursorpos =markers->cursorpos[i]-baseaddr;	// FIXME
				if (markers->relative[i]=='-') newcursorpos=actcursorpos-markers->cursorpos[i];
				if (markers->relative[i]=='+') newcursorpos=actcursorpos+markers->cursorpos[i];
			}
		}
		if (selected==itemnums[23] || selected==itemnums[24])
		{
			setcolor(output,COLOR_TEXT);
			mvwprintw(output->win,offsy+17,offsx+1,"Filename:");
			setcolor(output,COLOR_BRACKETS);
			mvwprintw(output->win,offsy+17,offsx+10,"[                                ]");
			ch=stringinput(output,offsy+17,offsx+10,filename,32);
			if (ch==KEYENTER)
			{
				if (selected==itemnums[23]) retval=parsemarkerfile(markers,filename);
				if (selected==itemnums[24]) retval=writemarkerfile(markers,filename);
			}
		}
	}
	return	RETOK;
}

