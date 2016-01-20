#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "machine_type.h"
#include "menu.h"
#include "output.h"
#include "input.h"

void clearMenu(tMenu* Menu)
{
	Menu->menuitemnum=0;
	Menu->menuitemactive=0;
}
void newMenuItem(tMenu* Menu,char* text,tUInt16 y,tUInt16 x,char hotkey,tBool active,tInt8* itemnum)
{
	memcpy(Menu->MenuItems[Menu->menuitemnum].menutext,text,64);
	Menu->MenuItems[Menu->menuitemnum].x=x;
	Menu->MenuItems[Menu->menuitemnum].y=y;
	if (hotkey>='a' && hotkey<='z') hotkey-=32;	// make the hotkey uppercase
	Menu->MenuItems[Menu->menuitemnum].hotkey=hotkey;
	
	if (active) Menu->menuitemactive=Menu->menuitemnum;
	*itemnum=Menu->menuitemnum;
	Menu->menuitemnum++;
}
void printMenu(tOutput* output,tMenu* Menu,tUInt16 offsy,tUInt16 offsx)
{
	int i;
	int j;
	for (i=0;i<Menu->menuitemnum;i++)
	{
		setcolor(output,(i==Menu->menuitemactive)?COLOR_MENUACTIVE:COLOR_MENUNORMAL);
		mvwprintw(output->win,offsy+Menu->MenuItems[i].y,offsx+Menu->MenuItems[i].x,"%s",Menu->MenuItems[i].menutext);
		if (Menu->MenuItems[i].hotkey)
		{
			for (j=0;j<strlen(Menu->MenuItems[i].menutext);j++)
			{
				if (Menu->MenuItems[i].menutext[j]==Menu->MenuItems[i].hotkey || (Menu->MenuItems[i].hotkey>='A' && Menu->MenuItems[i].hotkey<='Z' && Menu->MenuItems[i].menutext[j]-32==Menu->MenuItems[i].hotkey))
				{
					setcolor(output,(i==Menu->menuitemactive)?COLOR_MENUHOTKEYACTIVE:COLOR_MENUHOTKEY);
					mvwprintw(output->win,offsy+Menu->MenuItems[i].y,offsx+Menu->MenuItems[i].x+j,"%c",Menu->MenuItems[i].menutext[j]);
					j=65;
				}
			}
		}	
	}
}
void MenuMoveLeft(tMenu* Menu)
{
	tUInt16	actx;
	tUInt16	acty;
	tInt16	diffx;
	tInt16	diffy;
	tInt16	mindiffx;
	tInt16	mindiffy;
	tUInt16	minact;
	int i;

	actx=Menu->MenuItems[Menu->menuitemactive].x;
	acty=Menu->MenuItems[Menu->menuitemactive].y;


	mindiffx=COLS;
	mindiffy=LINES;
	minact=Menu->menuitemactive;

	for (i=0;i<Menu->menuitemnum;i++)
	if (i!=Menu->menuitemactive)
	{
		diffx=actx-Menu->MenuItems[i].x;
		diffy=acty-Menu->MenuItems[i].y;
		if (diffy<0) diffy=-diffy;
		if (mindiffy>diffy)
		{
			mindiffy=diffy;
			mindiffx=COLS;
		}
		if (mindiffy==diffy)
		{
			if (mindiffx>=diffx && diffx>0)
			{
				mindiffx=diffx;
				minact=i;				
			}
		}
	}
	Menu->menuitemactive=minact;	
}
void MenuMoveRight(tMenu* Menu)
{
	tUInt16	actx;
	tUInt16	acty;
	tInt16	diffx;
	tInt16	diffy;
	tInt16	mindiffx;
	tInt16	mindiffy;
	tUInt16	minact;
	int i;

	actx=Menu->MenuItems[Menu->menuitemactive].x;
	acty=Menu->MenuItems[Menu->menuitemactive].y;


	mindiffx=COLS;
	mindiffy=LINES;
	minact=Menu->menuitemactive;

	for (i=0;i<Menu->menuitemnum;i++)
	if (i!=Menu->menuitemactive)
	{
		diffx=Menu->MenuItems[i].x-actx;
		diffy=acty-Menu->MenuItems[i].y;
		if (diffy<0) diffy=-diffy;
		if (mindiffy>diffy)
		{
			mindiffy=diffy;
			mindiffx=COLS;
		}
		if (mindiffy==diffy)
		{
			if (mindiffx>=diffx && diffx>0)
			{
				mindiffx=diffx;
				minact=i;				
			}
		}
	}
	Menu->menuitemactive=minact;	
}
void MenuMoveUp(tMenu* Menu)
{
	tUInt16	actx;
	tUInt16	acty;
	tInt16	diffx;
	tInt16	diffy;
	tInt16	mindiffx;
	tInt16	mindiffy;
	tUInt16	minact;
	int i;

	actx=Menu->MenuItems[Menu->menuitemactive].x;
	acty=Menu->MenuItems[Menu->menuitemactive].y;


	mindiffx=COLS;
	mindiffy=LINES;
	minact=Menu->menuitemactive;

	for (i=0;i<Menu->menuitemnum;i++)
	if (i!=Menu->menuitemactive)
	{
		diffx=Menu->MenuItems[i].x-actx;
		diffy=acty-Menu->MenuItems[i].y;
		if (diffx<0) diffx=-diffx;
		if (mindiffx>diffx)
		{
			mindiffx=diffx;
			mindiffy=LINES;
		}
		if (mindiffx==diffx)
		{
			if (mindiffy>=diffy && diffy>0)
			{
				mindiffy=diffy;
				minact=i;				
			}
		}
	}
	Menu->menuitemactive=minact;	
}
void MenuMoveDown(tMenu* Menu)
{
	tUInt16	actx;
	tUInt16	acty;
	tInt16	diffx;
	tInt16	diffy;
	tInt16	mindiffx;
	tInt16	mindiffy;
	tUInt16	minact;
	int i;

	actx=Menu->MenuItems[Menu->menuitemactive].x;
	acty=Menu->MenuItems[Menu->menuitemactive].y;


	mindiffx=COLS;
	mindiffy=LINES;
	minact=Menu->menuitemactive;

	for (i=0;i<Menu->menuitemnum;i++)
	if (i!=Menu->menuitemactive)
	{
		diffx=Menu->MenuItems[i].x-actx;
		diffy=Menu->MenuItems[i].y-acty;
		if (diffx<0) diffx=-diffx;
	
		if (mindiffx>diffx)
		{
			mindiffx=diffx;
			mindiffy=LINES;
		}
		if (mindiffx==diffx)
		{
			if (mindiffy>=diffy && diffy>0)
			{
				mindiffy=diffy;
				minact=i;				
			}
		}
	}
	Menu->menuitemactive=minact;	
}
void MenuSetActiveItem(tMenu* Menu,tInt8 itemnum)
{
	Menu->menuitemactive=itemnum;
}
tInt8 MenuInteract(tOutput* output,tMenu* Menu,tInt16 offsy,tInt16 offsx)
{
	tInt16	ch;
	int	i;
	tInt8	retval;

	ch=0;
	printMenu(output,Menu,offsy,offsx);
	while (ch!=KEYENTER)
	{
		ch=getkey((tKeyTab*)output->pKeyTab,0);
		switch(ch)
		{
			case	KEYLEFT:	MenuMoveLeft(Menu);						break;
			case	KEYRIGHT:	MenuMoveRight(Menu);						break;
			case	KEYUP:		MenuMoveUp(Menu);						break;
			case	KEYDOWN:	MenuMoveDown(Menu);						break;
			case	KEYTAB:		Menu->menuitemactive=(Menu->menuitemactive+1)%Menu->menuitemnum;break;
			case	KEYENTER:	retval=Menu->menuitemactive;					break;
			default:
				if (ch>=' ' && ch<127)
				{
					if (ch>='a' && ch<='z') ch-=32;
					for (i=0;i<Menu->menuitemnum;i++)
					{
						if (Menu->MenuItems[i].hotkey==ch)
						{
							Menu->menuitemactive=i;
							ch=KEYENTER;
						}	
					}
				}
			break;
		}
		printMenu(output,Menu,offsy,offsx);
	}
	return Menu->menuitemactive;
}
