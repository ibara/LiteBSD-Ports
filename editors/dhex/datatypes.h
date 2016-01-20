#ifndef	DATATYPES_H
#define	DATATYPES_H
#include <ncurses.h>
#include "machine_type.h"

/* INPUT DATATYPES */
// this is for the special keys
// i was not satisfied with how ncurses handled the function keys
typedef struct  _tKeyTab
{
        tInt16	retval;
        tBool	allowed_in_inputfield;
        tUInt8	seqlen;
        char seq[8];		// which sequence will be returned when this key is pressed
        char config[16];	// what string is written in the config-file
	char desc[64];
} tKeyTab;


/* OUTPUT DATATYPES */

typedef enum {COLOR_NONE,COLOR_BRACKETS,COLOR_INPUT,COLOR_CURSOR,COLOR_TEXT,COLOR_HEXFIELD,COLOR_DIFF,COLOR_HEADLINE,COLOR_HEADER,COLOR_MENUHOTKEY,COLOR_MENUNORMAL,COLOR_FRAME,COLOR_MENUACTIVE,COLOR_MENUHOTKEYACTIVE,COLOR_CURSORDIFF,UICOLORNUM} uicolors;
typedef struct _tColors
{
	short	fg;
	short	bg;
	int	attrs;
} tColors;
typedef struct _tOutput
{
	WINDOW*	win;
	tColors colors[UICOLORNUM];
	int	attrs[UICOLORNUM];
	void*	pKeyTab;
} tOutput;


/* BUFFER DATATYPES */
typedef	struct	_tChange
{
	tUInt64	pos;
	tUInt8	before;
	tUInt8	after;
} tChange;

typedef struct _tBuffer
{
	tBool	valid;
	tUInt8	data[BUFFERSIZE];
	tUInt64 filesize;	// the size of the actual file
	tUInt64 bufsize;	// the size of the buffer, in case some bytes are added at the end
	tUInt64 bufferpos;	// the offset from where this chunk has been read
	tBool	fresh;		// =1 if there is no data in this buffer
	tFptr	file;		// the file pointer
	char	filename[512];	// the file name
	tChange	changes[CHANGEBUFSIZE];
	tUInt32	changesnum;	// number of changes
	tBool	nibble;
	tUInt8	nexthex;
	tUInt64	changepos;
	tInt64	baseaddr;
} tBuffer;


/* CORRELATION DATATYPES */
typedef	enum {CORR_BEST_MATCH,CORR_LONGEST_MATCH,CORR_MIN_DIFF} corr_algorithms;
typedef struct _tCorrelation
{	
	tBool		correlated;
	corr_algorithms algorithm;
	tInt64		start_mindiff;
	tInt64		mindiff;
	tInt64		mindiffpos;
	tInt64		bestmatch;
	tInt64		bestmatchpos;
} tCorrelation;

/* MENU DATATYPES */
typedef struct _tMenuItem
{
	char menutext[64];
	tUInt16 x;
	tUInt16 y;
	char hotkey;
} tMenuItem;

typedef struct _tMenu
{
	tMenuItem MenuItems[64];
	int menuitemnum;
	int menuitemactive;
} tMenu;

/* SEARCH DATATYPES */
typedef	struct	_tSearch
{
	char	writelogfilename[64];
	char	readlogfilename[64];
	tBool	writesearchlog;
	tBool	readsearchlog;
	tBool	forwardnotbackward;
	char	searchstring[32];
	tInt16	searchlen;
	tInt64	occurancesfound;
	tInt64	lastoccurance;
	tInt64	lastsearchlogpos;
} tSearch;


/* MARKER DATATYPES */
#define	NUMMARKERS	10

typedef struct	_tMarkers
{
	char	relative[NUMMARKERS];
	tUInt64	cursorpos[NUMMARKERS];	
} tMarkers;



#endif	
