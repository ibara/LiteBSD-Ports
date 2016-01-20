#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "configfile.h"
#include "machine_type.h"
#include "ncurses.h"
#include "output.h"
#include "input.h"

typedef	struct _tColorName
{
	char name[16];
	short len;
	short val;
	int attrs;
} tColorName;

typedef struct _tColorGroup
{
	char 	configname[16];
	short	 len;
	uicolors uicol;		
} tColorGroup;

#define	NUMCOLORGROUPS	14
const tColorGroup colorGroups[NUMCOLORGROUPS]={
				{"BRACKETS:",		 9,COLOR_BRACKETS},
				{"HEXFIELD:",		 9,COLOR_HEXFIELD},
				{"INPUT:",		 6,COLOR_INPUT},
				{"CURSOR:",		 7,COLOR_CURSOR},
				{"TEXT:",		 5,COLOR_TEXT},
				{"MENU_NORMAL:",	12,COLOR_MENUNORMAL},
				{"MENU_HIGHLIGHT:",	15,COLOR_MENUACTIVE},
				{"MENU_HOTKEY:",	12,COLOR_MENUHOTKEY},
				{"MENU_HOTKEY_HI:",	15,COLOR_MENUHOTKEYACTIVE},
				{"NORMAL_DIFF:",	12,COLOR_DIFF},
				{"CURSOR_DIFF:",	12,COLOR_CURSORDIFF},	
				{"HEADLINE:",		 9,COLOR_HEADLINE},
				{"HEADER:",		 7,COLOR_HEADER},
				{"FRAME:",		 6,COLOR_FRAME}
				};
#define	NUMCOLORNAMES	20
const tColorName colorNames[NUMCOLORNAMES]={
					{"BLACK",	5,COLOR_BLACK,0},
					{"RED",		3,COLOR_RED,0},
					{"GREEN",	5,COLOR_GREEN,0},
					{"BLUE",	4,COLOR_BLUE,0},
					{"PURPLE",	6,COLOR_MAGENTA,0},
					{"MAGENTA",	7,COLOR_MAGENTA,0},
					{"CYAN",	4,COLOR_CYAN,0},
					{"WHITE",	5,COLOR_WHITE,0},
					{"YELLOW",	6,COLOR_YELLOW,A_BOLD},
					{"BROWN",	5,COLOR_YELLOW,0},

					{"LIGHTBLACK",	10,COLOR_BLACK,A_BOLD},
					{"LIGHTRED",	 8,COLOR_RED,A_BOLD},
					{"LIGHTGREEN",	10,COLOR_GREEN,A_BOLD},
					{"LIGHTBLUE",	 9,COLOR_BLUE,A_BOLD},
					{"LIGHTPURPLE",	11,COLOR_MAGENTA,A_BOLD},
					{"LIGHTMAGENTA",12,COLOR_MAGENTA,A_BOLD},
					{"LIGHTCYAN",	 9,COLOR_CYAN,A_BOLD},
					{"LIGHTWHITE",	10,COLOR_WHITE,A_BOLD},
					{"LIGHTYELLOW",	11,COLOR_YELLOW,A_BOLD},
					{"LIGHTBROWN",	10,COLOR_YELLOW,A_BOLD}
				};

#define	NUMATTRNAMES	5
const tColorName attrNames[NUMATTRNAMES]={
					{"UNDERLINE",	 9,0,A_UNDERLINE},
					{"REVERSE",	 7,0,A_REVERSE},
					{"BLINK",	 5,0,A_BLINK},
					{"DIM",		 3,0,A_DIM},
					{"BOLD",	 4,0,A_BOLD}
				};

int getcolors(tOutput* output,char* line)
{
	int i;
	int l;
	int state=0;	// 0: find :, 1=find FG=, 2=find, , 3=find BG=, 4=find , ,5=find END, 6=nothing
	char token[6][64];
	int tokenidx=0;
	char c;
	short col1;
	short col2;
	int attrs1;
	int attrs2;
	int attrsret;
	short fg=7,bg=0;
	
	l=strlen(line);
	memset(token,0,sizeof(token));
	for (i=0;i<l;i++)
	{
		c=line[i];
		if (c=='#') state=6;
		if (c==' ' || c==9) c=0;	// ignore all the white spaces
		else {
			if (c>='a' && c<='z') c-=32;	// make the letters lower case
			if (state==0 && c==':') {
				token[state][tokenidx++]=c;	//apend the last char
				token[state][tokenidx]=0;	
				state=1;
				tokenidx=0;
			}
			else if (state==1 && c=='=') {state=2;tokenidx=0;}
			else if (state==2 && c==',') {state=3;tokenidx=0;}
			else if (state==3 && c=='=') {state=4;tokenidx=0;}
			else if (state==4 && c==',') {state=5;tokenidx=0;}
			else if (state==5 && c<32) {state=6;tokenidx=0;}
			else if (state!=6 && tokenidx<63)
			{
				token[state][tokenidx++]=c;
				token[state][tokenidx]=0;	
			}
		}
	}
	//printf("%s;%s;%s;%s;%s;%s\n",token[0],token[1],token[2],token[3],token[4],token[5]);
	// token0: contains the color group
	// token1: contains fg or bg
	// token2: contains the color
	// token3: contains fg or bg
	// token4: contains the color black, red, green, blue, purple, cyan, brown, yellow, white, lightblack, lightred, lightgreen, lightblue, lightpurple,lightcyan, lightbrown, lightyellow
	// token5: contains something light BOLD, reverse, blink, underline, dim
	
	col1=0;col2=0;
	attrs1=0;attrs2=0;
	attrsret=0;
	for (i=0;i<NUMCOLORNAMES;i++)
	{
		//printf("   colorNames[i].name=%s,token[2]=%s\n",colorNames[i].name,token[2]);
		if (strncmp(token[2],colorNames[i].name,colorNames[i].len)==0) {col1=colorNames[i].val;attrs1=colorNames[i].attrs;}
		if (strncmp(token[4],colorNames[i].name,colorNames[i].len)==0) {col2=colorNames[i].val;attrs2=colorNames[i].attrs;}
	}
	if (strncmp(token[1],"FG",2)==0) 
	{
		fg=col1;
		attrsret|=attrs1;
	}
	if (strncmp(token[1],"BG",2)==0) 
	{
		bg=col1;
		attrsret|=(attrs1==A_BOLD)?A_BLINK:attrs1;
	}

	if (strncmp(token[3],"FG",2)==0) 
	{
		fg=col2;
		attrsret|=attrs2;
	}
	if (strncmp(token[3],"BG",2)==0) 
	{
		bg=col2;
		attrsret|=(attrs2==A_BOLD)?A_BLINK:attrs2;
	}
	for (i=0;i<NUMATTRNAMES;i++)
	{
		if (strncmp(attrNames[i].name,token[5],attrNames[i].len)==0) attrsret|=attrNames[i].attrs;
	}

	for (i=0;i<NUMCOLORGROUPS;i++)
	{
		if (strncmp(colorGroups[i].configname,token[0],colorGroups[i].len)==0)
		{
			colorpair(output,colorGroups[i].uicol,fg,bg,attrsret);
		}
	}
	return	RETOK;
}
int readconfigfile(tOutput* output,char* filename)
{
	tFptr f=fopen(filename,"rb");
	unsigned char line[512];
	unsigned char c;
	int lineidx=0;
	int	keyboardcnt;
	if (!f) return 1;
	keyboardcnt=0;
	while (!feof(f))
	{
		fread(&c,sizeof(char),1,f);
		if (!feof(f))
		{
			if (c>='a' && c<='z') c-=32;	// make everything uppercase
			if (c==9) c=32;			// and whitespaces are all the same
			if (lineidx<511 && c!=32) 
			{
				line[lineidx++]=c;
				line[lineidx]=0;
				if (c<32)
				{
					getcolors(output,line);
					keyboardcnt+=(configkeytab((tKeyTab*)output->pKeyTab,line)==0);
					lineidx=0;
					line[0]=0;
				}
			}
		}
	}
	fclose(f);
	if (keyboardcnt!=NUM_SPECIALKEYS) return 2;	// not every spcial key was configured in the config file
	return 0;
}
