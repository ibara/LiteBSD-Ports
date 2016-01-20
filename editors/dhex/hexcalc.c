#include <stdio.h>
#include <stdlib.h>
#include "hexcalc.h"
#include <ncurses.h>
#include "output.h"
#include "input.h"
typedef	enum {FIELDHEX,FIELDDEC,FIELDBIN}	eFields;

void hexcalc(tOutput* output,thHexCalc* hHexCalc)
{
	tInt16	offsx;
	tInt16	offsy;
	tInt64	akkumulator=0;
	int i;
	int j;
	int cursorline=-1;
	tInt16	ch=0;
	tUInt64 x;
	tUInt64	newval=0;
	tUInt8	dig=0;
	tUInt64	g=10;
	tBool	neg=0;
	eFields	field=FIELDHEX;

//	hHexCalc->valuenum=0;
	if (hHexCalc->operators[0]==0) hHexCalc->operators[0]='=';

	offsx=COLS/2-40;
	offsy=LINES/2-10;	
	drawframe(output,offsy,offsx,21,79,"HexCalc");

	setcolor(output,COLOR_BRACKETS);
	for (i=0;i<17;i++)
	{
		mvwprintw(output->win,offsy+2+i,offsx+2,"[                ] [                    ] [                                ]");
	}
	setcolor(output,COLOR_MENUNORMAL);
	mvwprintw(output->win,offsy+20,offsx+ 2,":Add");
	mvwprintw(output->win,offsy+20,offsx+ 8,":Sub");
	mvwprintw(output->win,offsy+20,offsx+14,":Mul");
	mvwprintw(output->win,offsy+20,offsx+20,":Div");
	mvwprintw(output->win,offsy+20,offsx+26,":Mod");
	mvwprintw(output->win,offsy+20,offsx+32,">:Right");
	mvwprintw(output->win,offsy+20,offsx+41,"<:Left");
	mvwprintw(output->win,offsy+20,offsx+49,":And");
	mvwprintw(output->win,offsy+20,offsx+55,":Or");
	mvwprintw(output->win,offsy+20,offsx+61,":Xor");
	
	mvwprintw(output->win,offsy+20,offsx+74,"Exit");	

	setcolor(output,COLOR_MENUHOTKEY);
	mvwprintw(output->win,offsy+20,offsx+ 1,"+");
	mvwprintw(output->win,offsy+20,offsx+ 7,"-");
	mvwprintw(output->win,offsy+20,offsx+13,"*");
	mvwprintw(output->win,offsy+20,offsx+19,"/");
	mvwprintw(output->win,offsy+20,offsx+25,"%%");
	mvwprintw(output->win,offsy+20,offsx+31,">");
	mvwprintw(output->win,offsy+20,offsx+40,"<");
	mvwprintw(output->win,offsy+20,offsx+48,"&");
	mvwprintw(output->win,offsy+20,offsx+54,"|");
	mvwprintw(output->win,offsy+20,offsx+60,"^");
	mvwprintw(output->win,offsy+20,offsx+75,"x");
	

	setcolor(output,COLOR_TEXT);
	mvwprintw(output->win,offsy+1,offsx+ 3,"Hex");
	mvwprintw(output->win,offsy+1,offsx+22,"Dec");
	mvwprintw(output->win,offsy+1,offsx+45,"Bin");
	while (ch!='x' && ch!='X')
	{
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,offsy+19,offsx+ 1,"%c",hHexCalc->operators[0]);
		for (i=0;i<17 && i<hHexCalc->valuenum;i++)
		{	
			setcolor(output,COLOR_TEXT);
			mvwprintw(output->win,offsy+18-i,offsx+ 1,"%c",hHexCalc->operators[i+1]);
			x=hHexCalc->values[i];
			setcolor(output,(field==FIELDHEX && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			mvwprintw(output->win,offsy+18-i,offsx+ 3,"%16llx",x);
			setcolor(output,(field==FIELDDEC && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			mvwprintw(output->win,offsy+18-i,offsx+22,"%20lli",(tInt64)x);
			setcolor(output,(field==FIELDBIN && cursorline==i)?COLOR_INPUT:COLOR_TEXT);
			for (j=0;j<32;j++)
			{
				if (x || !j)	mvwprintw(output->win,offsy+18-i,offsx+76-j,"%i",(x&1));
				else		mvwprintw(output->win,offsy+18-i,offsx+76-j," ");
				x>>=1;
			}
		}
		if (cursorline==-1) x=newval; else x=hHexCalc->values[cursorline];
		setcolor(output,(field==FIELDHEX && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		mvwprintw(output->win,offsy+19,offsx+ 3,"%16llx",x);
		setcolor(output,(field==FIELDDEC && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		if (x || !neg) mvwprintw(output->win,offsy+19,offsx+22,"%20lli",(tInt64)x);
		else if (neg) mvwprintw(output->win,offsy+19,offsx+22,"                  -"); 
		setcolor(output,(field==FIELDBIN && cursorline==-1)?COLOR_INPUT:COLOR_TEXT);
		for (j=0;j<32;j++)
		{
			if (x || !j)	mvwprintw(output->win,offsy+19,offsx+76-j,"%i",(x&1));
			else		mvwprintw(output->win,offsy+19,offsx+76-j," ");
			x>>=1;
		}
		ch=getkey((tKeyTab*)output->pKeyTab,1);
		switch (ch)
		{
			case	KEYUP:		if (cursorline<(hHexCalc->valuenum-1))	cursorline++;break;
			case	KEYDOWN:	if (cursorline>-1)		cursorline--;break;
			case	KEYLEFT:	if (field!=FIELDHEX)		field--;break;
			case	KEYRIGHT:	if (field!=FIELDBIN)		field++;break;
			case	KEYTAB:		field=(field!=FIELDBIN)?field+1:FIELDHEX;break;
			case	'+':
			case	'-':
			case	'*':
			case	'/':
			case	'%':
			case	'&':
			case	'|':
			case	'^':
			case	'>':
			case	'<':
			case	KEYENTER:
					
						if (cursorline!=-1) newval=hHexCalc->values[cursorline];
						cursorline=-1;
						if (hHexCalc->valuenum)
						{
							switch(hHexCalc->operators[0])
							{
								case	'=':	akkumulator=newval;break;
								case	'+':	akkumulator=akkumulator+newval;break;
								case	'-':	akkumulator=akkumulator-newval;break;
								case	'*':	akkumulator=akkumulator*newval;break;
								case	'/':	akkumulator=akkumulator/newval;break;
								case	'%':	akkumulator=akkumulator%newval;break;
								case	'&':	akkumulator=akkumulator&newval;break;
								case	'|':	akkumulator=akkumulator|newval;break;
								case	'^':	akkumulator=akkumulator^newval;break;
								case	'>':	akkumulator=akkumulator>>newval;break;
								case	'<':	akkumulator=akkumulator<<newval;break;
							}
						}
						else
						{
							akkumulator=newval;
						}
						if (ch==KEYENTER) ch='=';
						if (cursorline!=-1) newval=hHexCalc->values[cursorline];	
						for (i=31;i>0;i--)
						{
							hHexCalc->values[i]=hHexCalc->values[i-1];
							hHexCalc->operators[i]=hHexCalc->operators[i-1];
						}
						hHexCalc->operators[0]=ch;
						hHexCalc->values[0]=newval;
						if (ch=='=') newval=akkumulator; else newval=0;
						hHexCalc->valuenum++;
						break;


			default:
				if (cursorline!=-1) cursorline=-1;
				dig=17;
				if (field==FIELDBIN) g=2;
				else if (field==FIELDDEC) g=10;
				else if (field==FIELDHEX) g=16;

				if (ch>='a' && ch<='f') ch-=32;
				if (field==FIELDBIN && ch>='0' && ch<='1') dig=(ch-'0');
				else if (field==FIELDDEC && ch>='0' && ch<='9') dig=(ch-'0');
				else if (field==FIELDHEX && ch>='0' && ch<='9') dig=(ch-'0');
				else if (field==FIELDHEX && ch>='A' && ch<='F') dig=(ch-'A'+10);
				else if (field==FIELDDEC && ch=='-') neg=1;
				if (ch==KEYBACKSPACE) 
				{
					if (newval) newval=newval/g;
					else neg=0;
				} else if (dig!=17) { 
					newval=newval*g;
					newval=newval+(neg?-dig:dig);
				}
				break;
		}
	}
}

