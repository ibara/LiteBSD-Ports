#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "machine_type.h"
#include "input.h"
#include "output.h"
#include "unistd.h"

void initkeytab(tOutput* output)
{
	// those values are for mac os x
	const tKeyTab KeyTab[NUM_SPECIALKEYS]={
	// key return value (retval),allowed_in_inputfield,seqlen,seq[8],config[16]
        {KEYESC,        1,1,{0x1b,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYESC:","The standard cancel key"},
        {KEYF1,         0,2,{0xc2,0xa1,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF1:","This key opens the goto dialog"},
        {KEYF2,         0,3,{0xe2,0x84,0xa2,0x00,0x00,0x00,0x00,0x00},"KEYF2:","This key opens the search dialog"},
        {KEYF3,         0,2,{0xc2,0xa3,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF3:","This key jumps to the next search item"},
        {KEYF4,         0,2,{0xc2,0xa2,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF4:","This key jumps to the previous search item"},
        {KEYF5,         0,3,{0xe2,0x88,0x9e,0x00,0x00,0x00,0x00,0x00},"KEYF5:","This key openes the hexcalc tool"},
        {KEYF6,         0,2,{0xc2,0xa7,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF6:","This key is not used yet"},
        {KEYF7,         0,2,{0xc2,0xb6,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF7:","This key is not used yet"},
        {KEYF8,         0,3,{0xe2,0x80,0xa2,0x00,0x00,0x00,0x00,0x00},"KEYF8:","This key is not used yet"},
        {KEYF9,         0,2,{0xc2,0xaa,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF9:","This key is for undo"},
        {KEYF10,        0,2,{0xc2,0xba,0x00,0x00,0x00,0x00,0x00,0x00},"KEYF10:","This key is for saving and quitting"},
        {KEYBACKSPACE,  1,1,{0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYBACKSPACE:","Erases a character in input fields"},
        {KEYDEL,        1,0,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYDEL:","Erases a character in input fields"},
        {KEYENTER,      1,1,{0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYENTER:","The standard Enter key"},
        {KEYTAB,        1,1,{0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYTAB:","This key jumps trough windows and menu items"},
        {KEYUP,         0,3,{0x1b,0x5b,0x41,0x00,0x00,0x00,0x00,0x00},"KEYUP:","Standard scroll key"},
        {KEYDOWN,       0,3,{0x1b,0x5b,0x42,0x00,0x00,0x00,0x00,0x00},"KEYDOWN:","Standard scroll key"},
        {KEYRIGHT,      0,3,{0x1b,0x5b,0x43,0x00,0x00,0x00,0x00,0x00},"KEYRIGHT:","Standard scroll key"},
        {KEYLEFT,       0,3,{0x1b,0x5b,0x44,0x00,0x00,0x00,0x00,0x00},"KEYLEFT:","Standard scroll key"},
        {KEYPGUP,       0,0,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYPGUP:","Scrolls up a full page"},
        {KEYPGDOWN,     0,0,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYPGDOWN:","Scrolls down a full page"},
        {KEYHOME,       0,0,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYHOME:","Jumps to beginning"},
        {KEYEND,        0,0,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},"KEYEND:","Jumps to the end"}
};
	if (output->pKeyTab) free(output->pKeyTab);
	output->pKeyTab=malloc(sizeof(tKeyTab)*NUM_SPECIALKEYS);
	memcpy(output->pKeyTab,KeyTab,sizeof(tKeyTab)*NUM_SPECIALKEYS);
}
tInt16 getkey(tKeyTab* pKeyTab,tBool inputfield)		// =1 this is an inputfield. which means that certain allowed_in_inputfields may not be used
{
	tInt16 ch;
	unsigned char seq[8];
	tUInt8 seqlen=0;
	tBool	done;
	tInt16 donecnt;
	tInt16 lastch=ERR;
	tBool	partial;
	tBool	exact;
	int i,j;

	donecnt=1000;	
	done=0;
	partial=0;
	exact=0;
	while (!done)
	{
		while (!done)
		{
			ch=getch();

			if ((lastch!=ERR && ch==ERR) || seqlen==8) done=1;
			if (ch!=ERR)
			{
				if (seqlen<8) seq[seqlen++]=ch;
				lastch=ch;
				done=1;	// a key was pressed
			} else {
				usleep(1);
				donecnt=donecnt-partial;
				done=!donecnt;
			}
		}
		exact=0;
		partial=0;
		for (i=0;i<NUM_SPECIALKEYS;i++)	// check if it was a special key
		{
			if (pKeyTab[i].seqlen==seqlen)
			{
				j=memcmp(pKeyTab[i].seq,seq,seqlen);
				if (j==0) 
				{
					lastch=pKeyTab[i].retval;	// in an inputfield, certain special keys have to be disabled
					exact=1;
				}
			} else if (pKeyTab[i].seqlen>seqlen) {
				j=memcmp(pKeyTab[i].seq,seq,seqlen);
				if (j==0) partial=1;
			}
		}
		if (exact && (!partial || !donecnt)) done=1;
		else if (partial && donecnt) done=0;
	}
	return lastch;
}


tInt16 decinput(tOutput* output,tInt16 y,tInt16 x,tUInt64* val,tInt16 len)
{
	tInt16 i;
	tInt16 ch;
	tInt16 done=0;
	tUInt64	newval;
	char tmpbuf[21];

	newval=*val;
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,y,x,"[");
	mvwprintw(output->win,y,x+len,"]");
	while (!done)
	{
		setcolor(output,COLOR_INPUT);
		snprintf(tmpbuf,21,"%20lli",newval);
		for (i=0;i<len && i<20;i++) mvwprintw(output->win,y,x+len-i,"%c",tmpbuf[20-i]);
		wmove(output->win,y,x+len);
		ch=getkey(output->pKeyTab,1);
		if (ch==KEYENTER) done=1;
		if (ch==KEYTAB) done=1;
		if (ch==KEYESC) done=1;
		if (ch>='0' && ch<='9') 
		{
			newval=newval*10;
			newval=newval+(ch-'0');
		}
		if (ch==KEYBACKSPACE || ch==KEYDEL)
		{
			newval=newval/10;
		}		
	}
	if (ch!=KEYESC) {
		*val=newval;
	}
	return ch;
}
tInt16 hexinput(tOutput* output,tInt16 y,tInt16 x,tUInt64* val,char* relative,tInt16 len)
{
	char e;
	tInt16 i;
	tInt16 ch;
	tInt16 done=0;
	tUInt64	newval;
	tUInt64	t;

	newval=*val;
	if (relative) e=*relative; else e='=';
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,y,x,"[");
	mvwprintw(output->win,y,x+len+1,"]");
	while (!done)
	{
		setcolor(output,COLOR_BRACKETS);
		for (i=0;i<len;i++) mvwprintw(output->win,y,x+i+1," ");
		setcolor(output,COLOR_INPUT);
		t=newval;
		for (i=len-2;i>=0 && t;i--)
		{
			tUInt8	c=t&0xf;
			t>>=4;
			mvwprintw(output->win,y,x+i+2,"%1x",c);
		}
		mvwprintw(output->win,y,x+i+2,"%c",e);
		refresh();
		ch=getkey(output->pKeyTab,1);
		if (ch=='=' || ch=='-' || ch=='+') e=ch;

		if (ch>='a' && ch<='f') ch-=32;	// make it uppercase
		if (ch==KEYENTER) done=1;
		if (ch==KEYTAB) done=1;
		if (ch==KEYESC) done=1;
		if (ch>='0' && ch<='9') 
		{
			newval=newval*16;
			newval=newval+(ch-'0');
		}
		else if (ch>='A' && ch<='F')
		{
			newval=newval*16;
			newval=newval+(ch-'A'+10);
		}
		if (ch==KEYBACKSPACE || ch==KEYDEL)
		{
			newval=newval/16;
		}		
	}
	if (ch!=KEYESC && relative==NULL) 
	{
		if (e=='=') *val=newval;
		if (e=='-') *val-=newval;
		if (e=='+') *val+=newval;
	} else if (ch!=KEYESC) {
		*val=newval;
		*relative=e;
	}
	return ch;
}
tInt16 hexinput2(tOutput* output,tInt16 y,tInt16 x,char* s,tInt16* usedlen,tInt16 len)
{
	char* buf;
	tInt16	ch;
	tBool	done=0;
	tInt16	newusedlen;
	tBool	nibble=0;
	tUInt8	newchar=0;
	int i;
	
	buf=malloc(len);
	memcpy(buf,s,len);
	newusedlen=*usedlen;

	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,y,x,"[");
	mvwprintw(output->win,y,x+len*3,"]");
	while (!done)
	{
		setcolor(output,COLOR_INPUT);
		for (i=0;i<len;i++)
		{
			if (i<newusedlen) mvwprintw(output->win,y,x+1+i*3,"%02x",((tUInt16)buf[i]&0xff));
			else if (i==newusedlen && nibble) mvwprintw(output->win,y,x+1+i*3,"%1x ",newchar&0xf);
			else mvwprintw(output->win,y,x+1+i*3,"  ");
			if (i!=(len-1)) mvwprintw(output->win,y,x+3+i*3," ");
		}
		wmove(output->win,y,x+1+newusedlen*3+nibble);
		ch=getkey(output->pKeyTab,1);
		if (ch>='a' && ch<='z') ch-=32;
		if (newusedlen<len && ((ch>='0' && ch<='9') || (ch>='A' && ch<='F')))
		{
			newchar<<=4;
			newchar|=(ch>='A')?(ch-'A'+10):(ch-'0');
			if (nibble) buf[newusedlen++]=newchar;
			nibble=!nibble;
		}
		if ((newusedlen>0 || nibble) && ch==KEYBACKSPACE) 
		{
			if (!nibble && newusedlen) newchar=buf[--newusedlen];
			newchar>>=4;
			nibble=!nibble;
		}
		if (ch==KEYENTER || ch==KEYTAB || ch==KEYUP || ch==KEYDOWN) done=1;
		if (ch==KEYESC) done=1;
	}
	if (ch!=KEYESC) 
	{
		memcpy(s,buf,len);
		*usedlen=newusedlen;
	}
	setcolor(output,COLOR_TEXT);
	newusedlen=*usedlen;
	for (i=0;i<len;i++)
	{
		if (i<newusedlen) mvwprintw(output->win,y,x+1+i*3,"%02x",((tUInt16)buf[i]&0xff));
		else if (i==newusedlen && nibble) mvwprintw(output->win,y,x+1+i*3,"%1x ",newchar&0xf);
		else mvwprintw(output->win,y,x+1+i*3,"  ");
		if (i!=(len-1)) mvwprintw(output->win,y,x+3+i*3," ");
	}
	free(buf);
	return ch;
}
tInt16 stringinput(tOutput* output,tInt16 y,tInt16 x,char* s,tInt16 len)
{
	tInt16 i;
	tInt16 cursorpos;
	tInt16 ch;
	tInt16 done=0;
	char* buf;

	buf=malloc(len);
	memcpy(buf,s,len);
	
	

	cursorpos=strlen(s);
	setcolor(output,COLOR_BRACKETS);
	mvwprintw(output->win,y,x,"[");
	mvwprintw(output->win,y,x+len+1,"]");
	while (!done)
	{
		setcolor(output,COLOR_INPUT);
		for (i=0;i<len;i++)
		{
			mvwprintw(output->win,y,x+i+1,"%c",(i<strlen(buf))?buf[i]:'_');
		}

		if (cursorpos<len)
		{
			setcolor(output,COLOR_CURSOR);
			mvwprintw(output->win,y,x+cursorpos+1,"%c",(cursorpos<strlen(buf))?buf[cursorpos]:'_');
		}
		move(y,x+cursorpos+1);
		refresh();
		ch=getkey(output->pKeyTab,1);
		if (ch==KEYENTER) 
			done=1;
		if (ch==KEYTAB) done=1;
		if (ch==KEYESC) done=1;
		if (ch==KEYLEFT && cursorpos) cursorpos--;
		if (ch==KEYRIGHT && cursorpos<strlen(buf)) cursorpos++;
		if (ch>=' ' && ch<127 && cursorpos<len)
		{
			memmove(&buf[cursorpos+1],&buf[cursorpos],len-cursorpos);
			buf[cursorpos++]=ch;
		} else if (ch==KEYBACKSPACE && cursorpos>0) {
			memmove(&buf[cursorpos-1],&buf[cursorpos],len-cursorpos);
			cursorpos--;
		} else if (ch==KEYDEL && cursorpos<(strlen(buf))) {
			memmove(&buf[cursorpos],&buf[cursorpos+1],len-cursorpos);
		}
	}
	if (ch!=KEYESC) memcpy(s,buf,len);
	setcolor(output,COLOR_TEXT);
	for (i=0;i<len;i++)
	{
		mvwprintw(output->win,y,x+i+1,"%c",(i<strlen(s))?s[i]:' ');
	}
	free(buf);
	return ch;
}
int	configkeytab(tKeyTab* pKeyTab,char* line)
{
	int i;
	int j;
	unsigned int x;
	int retval=1;
	
	for (i=0;i<NUM_SPECIALKEYS;i++)
	{
		if (strncmp(pKeyTab[i].config,line,strlen(pKeyTab[i].config))==0)
		{
			retval=0;
			x=1;
			pKeyTab[i].seqlen=0;
			for (j=strlen(pKeyTab[i].config);j<strlen(line);j++)
			{
				x<<=4;
				if (line[j]>='0' && line[j]<='9') x|=(line[j]-'0');
				if (line[j]>='A' && line[j]<='F') x|=(line[j]-'A'+10);
				if (x&0x100 && pKeyTab[i].seqlen<8)
				{
					pKeyTab[i].seq[pKeyTab[i].seqlen++]=(x&0xff);
					x=1;
				}
			}
			
		}	
	}
	return retval;
}
int writeconfigfile(tOutput* output,char* configfilename)
{
	tFptr f;
	char tmpbuf[65536];
	char lineorig[512];
	char lineupcase[512];
	char c;
	int lineorigidx;
	int lineupcaseidx;
	int i,j,size;
	tBool found;
	tKeyTab* pKeyTab=(tKeyTab*)output->pKeyTab;
	
	f=fopen(configfilename,"rb");
	if (!f) return RETNOK;
	size=fread(tmpbuf,sizeof(char),65536,f);
	fclose(f);
	if (size>=65536) return RETNOK;
	tmpbuf[size++]=0;

	lineorigidx=0;
	lineupcaseidx=0;
	f=fopen(configfilename,"w");
	for (i=0;i<size;i++)
	{
		c=tmpbuf[i];
		
		lineorig[lineorigidx++]=c;
		lineorig[lineorigidx]=0;
		if (c>='a' && c<='z') c-=32;
		if (c==9) c=32;
		if (c!=32)
		{
			lineupcase[lineupcaseidx++]=c;
			lineupcase[lineupcaseidx]=0;
		}
		
		if (c<32)
		{
			found=0;
			for (j=0;j<NUM_SPECIALKEYS;j++)
			{
				if (strncmp(pKeyTab[j].config,lineupcase,strlen(pKeyTab[j].config))==0) 
				{
					found=1;
				}
			}
			if (!found)
			fprintf(f,"%s",lineorig);
			lineorigidx=0;
			lineupcaseidx=0;
		}
	}
	for (i=0;i<NUM_SPECIALKEYS;i++)
	{
		fprintf(f,"%s",pKeyTab[i].config);
		for (j=0;j<pKeyTab[i].seqlen;j++)
		{
			fprintf(f,"%02x ",((unsigned int)pKeyTab[i].seq[j])&0xff);
		}
		fprintf(f,"\n");
	}
	fclose(f);
	return	RETOK;
}
void keyboardsetup(tOutput* output,char* configfilename)
{
	int i,j;
	int ch=0;
	int lastkey=0;
	int keyesc=0;
	unsigned char seq[8];
	unsigned int seqlen;
	tBool done;
	tKeyTab* pKeyTab=(tKeyTab*)output->pKeyTab;

	mvwprintw(output->win,0,0,"Please press the following keys");
	mvwprintw(output->win,1,0,"(Press ESC if your keyboard does not have them)");
	mvwprintw(output->win,3,0,"Config file:%s",configfilename);
	
	for (i=0;i<NUM_SPECIALKEYS;i++)
	{
		lastkey=-1;
		done=0;
		seqlen=0;
		memset(seq,0,sizeof(seq));
		mvwprintw(output->win,21,0,"                                                                ");
		mvwprintw(output->win,21,0,"%s",pKeyTab[i].desc);
		if (i<12) mvwprintw(output->win,5+i,0,"%s",pKeyTab[i].config);
		else mvwprintw(output->win,i-7,40,"%s",pKeyTab[i].config);
		memset(pKeyTab[i].seq,0,8);
		while (!done)
		{	
			ch=getch();
			if (ch==-1 && lastkey!=-1) done=1;
			if (ch!=-1 && seqlen<8) seq[seqlen++]=ch;
			lastkey=ch;
		}
		pKeyTab[i].seqlen=seqlen;
		memcpy(pKeyTab[i].seq,seq,seqlen);
		if (pKeyTab[i].retval==KEYESC) keyesc=i;
		else 
			if (memcmp(pKeyTab[i].seq,pKeyTab[keyesc].seq,8)==0 && i!=keyesc) pKeyTab[i].seqlen=0;
		for (j=0;j<pKeyTab[i].seqlen;j++)
		{
			if (i<12) mvwprintw(output->win,5+i,15+j*3,"%02x",seq[j]);
			else mvwprintw(output->win,i-7,55+j*3,"%02x",seq[j]);
		}
		if (seqlen==1 && seq[0]>=32 && seq[0]<127)
		{
			mvwprintw(output->win,(i<12)?(5+i):(i-7),(i<12)?0:40,"%s(*)",pKeyTab[i].config);
			mvwprintw(output->win,20,0,"WARNING: Keystrokes with (*) cannot be used in an input field\n");
		}
	}
	for (i=0;i<NUM_SPECIALKEYS;i++)
	{
		if (memcmp(pKeyTab[i].seq,pKeyTab[keyesc].seq,8)==0 && i!=keyesc) pKeyTab[i].seqlen=0;
	}
	mvwprintw(output->win,21,0,"Would you like me to write this into the config file? (Y/N)");
	done=0;
	while (!done)
	{
		ch=getch();
		if (ch=='n' || ch=='N') done=1;
		else if (ch=='y' || ch=='Y')
		{
			writeconfigfile(output,configfilename);
			done=1;
		} else usleep(1);
	}
}
