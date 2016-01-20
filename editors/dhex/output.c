#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <ncurses.h>
#include "machine_type.h"
#include "output.h"
#include "buffers.h"
void initcolors(tOutput* output)
{

	output->colors[COLOR_BRACKETS].fg	=COLOR_BLACK;	output->colors[COLOR_BRACKETS].bg	=COLOR_BLACK;	output->colors[COLOR_BRACKETS].attrs	=A_BOLD;
	output->colors[COLOR_HEXFIELD].fg	=COLOR_WHITE;	output->colors[COLOR_HEXFIELD].bg	=COLOR_BLACK;	output->colors[COLOR_HEXFIELD].attrs	=0;
	output->colors[COLOR_INPUT].fg		=COLOR_BLACK;	output->colors[COLOR_INPUT].bg		=COLOR_WHITE;	output->colors[COLOR_INPUT].attrs	=0;
	output->colors[COLOR_CURSOR].fg		=COLOR_WHITE;	output->colors[COLOR_CURSOR].bg		=COLOR_BLACK;	output->colors[COLOR_CURSOR].attrs	=0;
	output->colors[COLOR_TEXT].fg		=COLOR_CYAN;	output->colors[COLOR_TEXT].bg		=COLOR_BLACK;	output->colors[COLOR_TEXT].attrs	=A_BOLD;
	output->colors[COLOR_MENUNORMAL].fg	=COLOR_BLUE;	output->colors[COLOR_MENUNORMAL].bg	=COLOR_BLACK;	output->colors[COLOR_MENUNORMAL].attrs	=A_BOLD;
	output->colors[COLOR_MENUACTIVE].fg	=COLOR_BLUE;	output->colors[COLOR_MENUACTIVE].bg	=COLOR_BLUE;	output->colors[COLOR_MENUACTIVE].attrs	=A_BOLD;
	output->colors[COLOR_MENUHOTKEY].fg	=COLOR_CYAN;	output->colors[COLOR_MENUHOTKEY].bg	=COLOR_BLACK;	output->colors[COLOR_MENUHOTKEY].attrs	=0;
	output->colors[COLOR_MENUHOTKEYACTIVE].fg=COLOR_CYAN;	output->colors[COLOR_MENUHOTKEYACTIVE].bg=COLOR_BLUE;	output->colors[COLOR_MENUHOTKEYACTIVE].attrs=0;
	output->colors[COLOR_FRAME].fg		=COLOR_BLUE;	output->colors[COLOR_FRAME].bg		=COLOR_BLACK;	output->colors[COLOR_FRAME].attrs	=0;
	output->colors[COLOR_DIFF].fg		=COLOR_YELLOW;	output->colors[COLOR_DIFF].bg		=COLOR_BLACK;	output->colors[COLOR_DIFF].attrs	=A_BOLD;
	output->colors[COLOR_HEADLINE].fg	=COLOR_BLUE;	output->colors[COLOR_HEADLINE].bg	=COLOR_BLACK;	output->colors[COLOR_HEADLINE].attrs	=0;
//	output->colors[COLOR_INFO].fg		=COLOR_WHITE;	output->colors[COLOR_INFO].bg		=COLOR_BLACK;	output->colors[COLOR_INFO].attrs	=A_BOLD;
	output->colors[COLOR_HEADER].fg		=COLOR_BLACK;	output->colors[COLOR_HEADER].bg		=COLOR_CYAN;	output->colors[COLOR_HEADER].attrs	=0;
}	
void colorpair(tOutput* output,uicolors uicol,short fg,short bg,int attr)
{
	//init_pair(uicol,fg,bg);attrs[uicol]=COLOR_PAIR(uicol)+attr;
	output->colors[uicol].fg=fg;
	output->colors[uicol].bg=bg;
	output->colors[uicol].attrs=attr;
}
void pairsinit(tOutput* output)
{
	int i;
	start_color();
	for (i=0;i<UICOLORNUM;i++)
	{
		init_pair(i,output->colors[i].fg,output->colors[i].bg);output->attrs[i]=COLOR_PAIR(i)+output->colors[i].attrs;
	}
}
void setcolor(tOutput* output,uicolors col)
{
	wattrset(output->win,output->attrs[col]);
}

void drawframe(tOutput* output,tInt16 y,tInt16 x,tInt8 h,tInt8 w,char* header)
{
	int i;
	int j;
	
	setcolor(output,COLOR_FRAME);
	wmove(output->win,y,x);
	waddch(output->win,ACS_ULCORNER);
	for (i=0;i<w-2;i++) waddch(output->win,ACS_HLINE);
	waddch(output->win,ACS_URCORNER);
	for (j=1;j<h;j++)
	{
		wmove(output->win,y+j,x);
		waddch(output->win,ACS_VLINE);
		for (i=0;i<w-2;i++) waddch(output->win,' ');
		waddch(output->win,ACS_VLINE);
	}
	wmove(output->win,y+h,x);
	waddch(output->win,ACS_LLCORNER);
	for (i=0;i<w-2;i++) waddch(output->win,ACS_HLINE);
	waddch(output->win,ACS_LRCORNER);
	if (header)
	{
		setcolor(output,COLOR_BRACKETS);
		mvwprintw(output->win,y,x+1,"[");
		mvwprintw(output->win,y,x+1+strlen(header)+1,"]");
		setcolor(output,COLOR_HEADER);
		mvwprintw(output->win,y,x+2,"%s",header);
	}
}
void drawcenterframe(tOutput* output,tInt8 h,tInt8 w,char* header)
{
	drawframe(output,LINES/2-h/2,COLS/2-w/2,h,w,header);
}
void printbuffersingle(tOutput* output,tBuffer* hBuf1,tInt64 cursorpos1,tUInt64 firstpos1,tUInt8 windowfield)
{
	int i;
	int j;
	int k;
	int addrwidth;
	int bytesperline;
	tInt32 intpos1;
	tInt32 charcnt;



	addrwidth=((hBuf1->bufsize+hBuf1->baseaddr)>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	
	setcolor(output,COLOR_HEADLINE);
	wmove(output->win,0,0);
	for (i=0;i<COLS;i++)
	{
		waddch(output->win,ACS_HLINE);
	}
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,0,COLS-3-strlen(hBuf1->filename),"[");
	mvwprintw(output->win,0,COLS-2,"]");
	if (addrwidth==16)
	{
		mvwprintw(output->win,0,1,"[                /                ]");
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,0,2,"%16llX",cursorpos1+hBuf1->baseaddr);
		mvwprintw(output->win,0,19,"%16llX",hBuf1->bufsize+hBuf1->baseaddr);
	} else {
		mvwprintw(output->win,0,1,"[        /        ]");
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,0,2,"%8X",(tUInt32)(cursorpos1+hBuf1->baseaddr));
		mvwprintw(output->win,0,11,"%8X",(tUInt32)(hBuf1->bufsize+hBuf1->baseaddr));
	}
	setcolor(output,COLOR_HEADER);
	mvwprintw(output->win,0,COLS-2-strlen(hBuf1->filename),"%s",hBuf1->filename);
	intpos1=getbufferidx(hBuf1,firstpos1);
	if (intpos1>=0)
	{
		charcnt=0;
		for (i=0;i<LINES-2;i++)
		{
			tBool colhex;
			setcolor(output,COLOR_HEXFIELD);
			if (addrwidth==8)	mvwprintw(output->win,i+1,0, "% 8X    ",(tUInt32)(firstpos1+hBuf1->baseaddr));
			else			mvwprintw(output->win,i+1,0,"% 16llX    ",firstpos1+hBuf1->baseaddr);

			mvwprintw(output->win,i+1,COLS-bytesperline-5,"      ");
			wmove(output->win,i+1,addrwidth+3);
			colhex=1;
			for (j=0;j<bytesperline;j++)
			{
				tInt16	c;
				if ((intpos1+j)<hBuf1->filesize) c=hBuf1->data[intpos1+j];
				else	c=-1;
				if (!(j&7)) wprintw(output->win," ");
				wprintw(output->win," ");
				for (k=0;k<hBuf1->changesnum;k++)
				{
					if (hBuf1->changes[k].pos==(firstpos1+j))	
					{
						setcolor(output,COLOR_DIFF);
						c=hBuf1->changes[k].after;
						colhex=0;
					}
				}	
				if (cursorpos1==(firstpos1+j) && windowfield==0) 
				{
					setcolor(output,COLOR_INPUT);
					colhex=0;
				}
				//  TODO: setcolor(hBuf1->data[intpos1]!=buf2->data[intpos2]?COLOR_DIFF:COLOR_HEXFIELD);
				if (c>=0)
				{
					if (hBuf1->nibble && hBuf1->changepos==(firstpos1+j))
					
						wprintw(output->win,"%01x ",hBuf1->nexthex&0xf);
					else
						wprintw(output->win,"%02x",c&0xff);
				}
				else
					wprintw(output->win,"  ");

				if (!colhex) setcolor(output,COLOR_HEXFIELD);
				colhex=1;
//				mvwprintw(output->win,i+1,addrwidth+5+j*3+j/8,"   ");
			}
			for (j=0;j<bytesperline;j++)
			{
				unsigned char c;
				if (intpos1<hBuf1->filesize)
					c=hBuf1->data[intpos1];
				else c=' ';
				for (k=0;k<hBuf1->changesnum;k++)
				{
					if (hBuf1->changes[k].pos==(firstpos1))
					{
						setcolor(output,COLOR_DIFF);
						c=hBuf1->changes[k].after;
					}
				}	
				if (cursorpos1==firstpos1 && windowfield==1) setcolor(output,COLOR_INPUT);
				//  TODO: setcolor(buf1->data[intpos1]!=buf2->data[intpos2]?COLOR_DIFF:COLOR_HEXFIELD);
				mvwprintw(output->win,i+1,COLS-bytesperline+j,"%c",(c>=32 && c<127)?c:'.');
				intpos1++;
				firstpos1++;
 				setcolor(output,COLOR_HEXFIELD);
			}
		}
	}
}
void printbufferdiff(tOutput* output,tBuffer* hBuf1,tBuffer* hBuf2,tInt64 cursorpos1,tInt64 cursorpos2)
{
	int i;
	int j;
	int addrwidth;
	int bytesperline;
	tInt32 intpos1;
	tInt32 intpos1b;
	tInt32 intpos2;
	tInt32 intpos2b;
	tInt32 charcnt;
	uicolors oldcolor;


	addrwidth=((hBuf1->bufsize+hBuf1->baseaddr)>0xffffffffull || (hBuf2->bufsize+hBuf2->baseaddr)>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	
	setcolor(output,COLOR_HEADLINE);
	wmove(output->win,0,0);
	for (i=0;i<COLS;i++)
	{
		waddch(output->win,ACS_HLINE);
	}
	wmove(output->win,LINES/2,0);
	for (i=0;i<COLS;i++)
	{
		waddch(output->win,ACS_HLINE);
	}
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,0,COLS-3-strlen(hBuf1->filename),"[");
	mvwprintw(output->win,0,COLS-2,"]");
	mvwprintw(output->win,LINES/2,COLS-3-strlen(hBuf2->filename),"[");
	mvwprintw(output->win,LINES/2,COLS-2,"]");
	if (addrwidth==16)
	{
		mvwprintw(output->win,0,1,"[                /                ]");
		mvwprintw(output->win,LINES/2,1,"[                /                ]");
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,0,2,"%16llX",cursorpos1+hBuf1->baseaddr);
		mvwprintw(output->win,0,19,"%16llX",hBuf1->bufsize+hBuf1->baseaddr);
		mvwprintw(output->win,LINES/2,2,"%16llX",cursorpos2+hBuf2->baseaddr);
		mvwprintw(output->win,LINES/2,19,"%16llX",hBuf2->bufsize+hBuf2->baseaddr);
		
	} else {
		mvwprintw(output->win,0,1,"[        /        ]");
		mvwprintw(output->win,LINES/2,1,"[        /        ]");
		setcolor(output,COLOR_TEXT);
		mvwprintw(output->win,0,2,"%8X",(tUInt32)cursorpos1+hBuf1->baseaddr);
		mvwprintw(output->win,0,11,"%8X",(tUInt32)hBuf1->bufsize+hBuf1->baseaddr);
		mvwprintw(output->win,LINES/2,2,"%8X",(tUInt32)cursorpos2+hBuf2->baseaddr);
		mvwprintw(output->win,LINES/2,11,"%8X",(tUInt32)hBuf2->bufsize+hBuf2->baseaddr);
	}
	setcolor(output,COLOR_HEADER);
	mvwprintw(output->win,0,COLS-2-strlen(hBuf1->filename),"%s",hBuf1->filename);
	mvwprintw(output->win,LINES/2,COLS-2-strlen(hBuf2->filename),"%s",hBuf2->filename);
	if (cursorpos1<0)
	{
		getbufferidx(hBuf1,0);
		intpos1=cursorpos1;
	} else {
		intpos1=getbufferidx(hBuf1,cursorpos1);
	}
	if (cursorpos2<0)
	{
		getbufferidx(hBuf2,0);
		intpos2=cursorpos2;
	} else {
		intpos2=getbufferidx(hBuf2,cursorpos2);
	}
	if (intpos1>=0 || intpos2>=0)
	{
		charcnt=0;
		intpos1b=intpos1;
		intpos2b=intpos2;

		for (i=0;i<(LINES+1)/2-2;i++)
		{
			setcolor(output,COLOR_HEXFIELD);
			if (addrwidth==8)	mvwprintw(output->win,i+1,0, "% 8X    ",(tUInt32)(cursorpos1+hBuf1->baseaddr));
			else			mvwprintw(output->win,i+1,0,"% 16llX    ",cursorpos1+hBuf1->baseaddr);

			mvwprintw(output->win,i+1,COLS-bytesperline-5,"      ");
			oldcolor=COLOR_HEXFIELD;
			wmove(output->win,i+1,addrwidth+3);
			for (j=0;j<bytesperline;j++)
			{
				tInt16	c1,c2;
				if ((intpos1+j)>=0 && (intpos1+j)<hBuf1->filesize) c1=hBuf1->data[intpos1+j];
				else	c1=-1;
				if ((intpos2+j)>=0 && (intpos2+j)<hBuf2->filesize) c2=hBuf2->data[intpos2+j];
				else	c2=-1;
				if (c1!=c2 && oldcolor==COLOR_HEXFIELD)
				{
					setcolor(output,COLOR_DIFF);
					oldcolor=COLOR_DIFF;
				} else if (c1==c2) {
					setcolor(output,COLOR_HEXFIELD);
					oldcolor=COLOR_HEXFIELD;
				}
				wprintw(output->win," ");
				if (j!=(bytesperline-1))
				{
					if (!(j&7)) wprintw(output->win," ");
				}
				if (c1>=0)
				{
					wprintw(output->win,"%02x",(c1&0xff));
				}
				else
					wprintw(output->win,"  ");

			}
			wmove(output->win,i+1,COLS-bytesperline);
			for (j=0;j<bytesperline;j++)
			{
				unsigned char c1,c2;
				if (intpos1>=0 && intpos1<hBuf1->filesize)
					c1=hBuf1->data[intpos1];
				else c1=' ';
				if (intpos2>=0 && intpos2<hBuf2->filesize)
					c2=hBuf2->data[intpos2];
				else c2=' ';

				if (c1!=c2 && oldcolor==COLOR_HEXFIELD)
				{
					setcolor(output,COLOR_DIFF);
					oldcolor=COLOR_DIFF;
				} else if (c1==c2) {
					setcolor(output,COLOR_HEXFIELD);
					oldcolor=COLOR_HEXFIELD;
				}
				wprintw(output->win,"%c",(c1>=32 && c1<127)?c1:'.');
				intpos1++;
				cursorpos1++;
				intpos2++;
			}
		}
		oldcolor=COLOR_HEXFIELD;
		setcolor(output,COLOR_HEXFIELD);
		if (!(LINES&1))
		{
			wmove(output->win,LINES/2-1,0);
			for (i=0;i<COLS;i++) wprintw(output->win," ");
		}

		intpos1=intpos1b;
		intpos2=intpos2b;
		for (i=0;i<(LINES+1)/2-2;i++)
		{
			setcolor(output,COLOR_HEXFIELD);
			if (addrwidth==8)	mvwprintw(output->win,i+1+LINES/2,0, "% 8X    ",(tUInt32)(cursorpos2+hBuf2->baseaddr));
			else			mvwprintw(output->win,i+1+LINES/2,0,"% 16llX    ",cursorpos2+hBuf2->baseaddr);

			mvwprintw(output->win,i+1+LINES/2,COLS-bytesperline-5,"      ");
			oldcolor=COLOR_HEXFIELD;
			wmove(output->win,i+1+LINES/2,addrwidth+3);
			for (j=0;j<bytesperline;j++)
			{
				tInt16	c1,c2;
				if ((intpos1+j)>=0 && (intpos1+j)<hBuf1->filesize) c1=hBuf1->data[intpos1+j];
				else	c1=-1;
				if ((intpos2+j)>=0 && (intpos2+j)<hBuf2->filesize) c2=hBuf2->data[intpos2+j];
				else	c2=-1;
				if (c1!=c2 && oldcolor==COLOR_HEXFIELD)
				{
					setcolor(output,COLOR_DIFF);
					oldcolor=COLOR_DIFF;
				} else if (c1==c2) {
					setcolor(output,COLOR_HEXFIELD);
					oldcolor=COLOR_HEXFIELD;
				}
				wprintw(output->win," ");
				if (j!=(bytesperline-1))
				{
					if (!(j&7)) wprintw(output->win," ");
				}
				if (c2>=0)
				{
					wprintw(output->win,"%02x",(c2&0xff));
				}
				else
					wprintw(output->win,"  ");

			}
			wmove(output->win,i+1+LINES/2,COLS-bytesperline);
			for (j=0;j<bytesperline;j++)
			{
				unsigned char c1,c2;
				if (intpos1>=0 && intpos1<hBuf1->filesize)
					c1=hBuf1->data[intpos1];
				else c1=' ';
				if (intpos2>=0 && intpos2<hBuf2->filesize)
					c2=hBuf2->data[intpos2];
				else c2=' ';

				if (c1!=c2 && oldcolor==COLOR_HEXFIELD)
				{
					setcolor(output,COLOR_DIFF);
					oldcolor=COLOR_DIFF;
				} else if (c1==c2) {
					setcolor(output,COLOR_HEXFIELD);
					oldcolor=COLOR_HEXFIELD;
				}
				wprintw(output->win,"%c",(c2>=32 && c2<127)?c2:'.');
				intpos1++;
				intpos2++;
				cursorpos2++;
			}
		}
	}
}
void printmainmenu(tOutput* output,tBool diffmode)
{
	int i;
	int x;
	unsigned char* menutextsnodiff[]={
"Goto  ",	// F1
"Search",	// F2
"Next  ",	// F3
"Prev  ",	// F4
"HexCal",	// F5
"      ",	// F6
"      ",	// F7
"      ",	// F8
"Undo  ",	// F9
"Quit  "};	// F10

	unsigned char* menutextsdiff[]={
"      ",	// F1
"      ",	// F2
"Next  ",	// F3
"Prev  ",	// F4
"HexCal",	// F5
"Corr. ",	// F6
"      ",	// F7
"      ",	// F8
"       ",	// F9
"Quit  "};	// F10

	setcolor(output,COLOR_MENUHOTKEY);
	
	x=0;
	for (i=0;i<10;i++)
	{
		mvwprintw(output->win,LINES-1,x," %i",(i+1)%10);
		x+=8;
	}
	setcolor(output,COLOR_MENUNORMAL);
	x=2;
	for (i=0;i<10;i++)
	{
		mvwprintw(output->win,LINES-1,x,"%s",diffmode?menutextsdiff[i]:menutextsnodiff[i]);
		x+=8;
	}
	
}
tInt32 movepositions(tInt64* cursorpos,tInt64* firstpos,tInt64 maxbufsize,tInt32 chars,tInt32 lines,tInt32 pages,tBool diffmode)
{
	tInt32 bytesperline;
	tInt32 bytesperhalfpage;
	tInt32 bytesperpage;
	tInt32 addrwidth;
	
	tInt64 newfirstpos=*firstpos;
	tInt64 newcursorpos=*cursorpos;
	

	addrwidth=(maxbufsize>0xffffffffull)?16:8;
	bytesperline=(COLS-(addrwidth+3+3))*8/(8*3+8+1);			// this many bytes can be printed in one line. every 8 bytes there is an extra space in the hex field.
	bytesperhalfpage=bytesperline*(LINES/2-2);
	bytesperpage=bytesperline*(LINES-2);
	

	newcursorpos+=(chars+lines*bytesperline+pages*(diffmode?bytesperhalfpage:bytesperpage));
	if (newcursorpos>=(newfirstpos+(diffmode?bytesperhalfpage:bytesperpage)) ||  (newcursorpos<newfirstpos)) newfirstpos+=(chars+lines*bytesperline+pages*(diffmode?bytesperhalfpage:bytesperpage));
		
	*firstpos=newfirstpos;	
	*cursorpos=newcursorpos;
	if (newcursorpos<0 || newfirstpos<0 || newcursorpos>maxbufsize || newfirstpos>maxbufsize) return RETNOK;
	return RETOK;

}
