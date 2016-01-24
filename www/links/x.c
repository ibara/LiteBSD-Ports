/* x.c
 * (c) 2002 Petr 'Brain' Kulhavy
 * This file is a part of the Links program, released under GPL.
 */

/* Takovej mensi problemek se scrollovanim:
 *
 * Mikulas a Xy zpusobili, ze scrollovani a prekreslovani je asynchronni. To znamena, ze
 * je v tom peknej bordylek. Kdyz BFU scrollne s oknem, tak se zavola funkce scroll. Ta
 * posle Xum XCopyArea (prekopiruje prislusny kus okna) a vygeneruje eventu
 * (GraphicsExpose) na postizenou (odkrytou) oblast. Funkce XCopyArea pripadne vygeneruje
 * dalsi GraphicsExpose eventu na postizenou oblast, ktera se muze objevit, kdyz je
 * linksove okno prekryto jinym oknem.
 *
 * Funkce scroll skonci. V event handleru se nekdy v budoucnosti (treba za tyden)
 * zpracuji eventy od Xu, mezi nimi i GraphicsExpose - tedy prekreslovani postizenych
 * oblasti.
 *
 * Problem je v tom, ze v okamziku, kdy scroll skonci, neni obrazovka prekreslena do
 * konzistentniho stavu (misty je garbaz) a navic se muze volat dalsi scroll. Tedy
 * XCopyArea muze posunout garbaz nekam do cudu a az se dostane na radu prekreslovani
 * postizenych oblasti, garbaz se uz nikdy neprekresli.
 *
 * Ja jsem navrhoval udelat scrollovani synchronni, to znamena, ze v okamziku, kdy scroll
 * skonci, bude okno v konzistentnim stavu. To by znamenalo volat ze scrollu zpracovavani
 * eventu (alespon GraphicsExpose). To by ovsem nepomohlo, protoze prekreslovaci funkce
 * neprekresluje, ale registruje si bottom halfy a podobny ptakoviny a prekresluje az
 * nekdy v budoucnosti. Navic Mikulas rikal, ze prekreslovaci funkce muze generovat dalsi
 * prekreslovani (sice jsem nepochopil jak, ale hlavne, ze je vecirek), takze by to
 * neslo.
 *
 * Proto Mikulas vymyslel genialni tah - takzvany "genitah". Ve funkci scroll se projede
 * fronta eventu od Xserveru a vyberou se GraphicsExp(l)ose eventy a ulozi se do zvlastni
 * fronty. Ve funkci na zpracovani Xovych eventu se zpracuji eventy z teto fronty. Na
 * zacatku scrollovaci funkce se projedou vsechny eventy ve zvlastni fronte a updatuji se
 * jim souradnice podle prislusneho scrollu.
 *
 * Na to jsem ja vymyslel uzasnou vymluvu: co kdyz 1. scroll vyrobi nejake postizene
 * oblasti a 2. scroll bude mit jinou clipovaci oblast, ktera bude tu postizenou oblast
 * zasahovat z casti. Tak to se bude jako ta postizena oblast stipat na casti a ty casti
 * se posunou podle toho, jestli jsou zasazene tim 2. scrollem? Tim jsem ho utrel, jak
 * zpoceny celo.
 *
 * Takze se to nakonec udela tak, ze ze scrollu vratim hromadu rectanglu, ktere se maji
 * prekreslit, a Mikulas si s tim udela, co bude chtit. Podobne jako ve svgalib, kde se
 * vrati 1 a Mikulas si prislusnou odkrytou oblast prekresli sam. Doufam jen, ze to je
 * posledni verze a ze nevzniknou dalsi problemy.
 *
 * Ve funkci scroll tedy pribude argument struct rect_set **.
 */


/* Data od XImage se alokujou pomoci malloc. get_links_icon musi alokovat taky
 * pomoci malloc.
 */

/* Pozor: po kazdem XSync nebo XFlush se musi dat
 * X_SCHEDULE_PROCESS_EVENTS
 * jinak to bude cekat na filedescriptoru, i kdyz to ma eventy uz ve fronte.
 *	-- mikulas
 */


#include "cfg.h"

#ifdef GRDRV_X

/* #define X_DEBUG */
/* #define SC_DEBUG */

#if defined(X_DEBUG) || defined(SC_DEBUG)
#define MESSAGE(a) fprintf(stderr,"%s",a);
#endif

#include "links.h"

/* Mikulas je PRASE: definuje makro "format" a navrch to jeste nechce vopravit */
#ifdef format
#undef format
#endif

#if defined(HAVE_XOPENIM) && defined(HAVE_XCLOSEIM) && defined(HAVE_XCREATEIC) && defined(HAVE_XDESTROYIC) && (defined(HAVE_XWCLOOKUPSTRING) || defined(HAVE_XUTF8LOOKUPSTRING))
#define X_INPUT_METHOD
#endif

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#if defined(HAVE_X11_XLOCALE_H) && defined(HAVE_XSETLOCALE)
#include <X11/Xlocale.h>
#else
#ifdef HAVE_SETLOCALE
#undef HAVE_SETLOCALE
#endif
#endif


#ifndef XK_MISCELLANY
#define XK_MISCELLANY
#endif

#ifndef XK_LATIN1
#define XK_LATIN1
#endif
#include <X11/keysymdef.h>

#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#define X_BORDER_WIDTH 4
#define X_HASH_TABLE_SIZE 64

#define X_MAX_CLIPBOARD_SIZE	(15*1024*1024)

#define XPIXMAPP(a) ((struct x_pixmapa*)(a))

static int x_default_window_width;
static int x_default_window_height;

static long (*x_get_color_function)(int);

static void selection_request(XEvent *event);

static int x_fd;    /* x socket */
static Display *x_display = NULL;   /* display */
static unsigned char *x_display_string = NULL;
static int x_screen;   /* screen */
static int x_display_height,x_display_width;   /* screen dimensions */
static unsigned long x_black_pixel;  /* black pixel */
static int x_depth,x_bitmap_bpp;   /* bits per pixel and bytes per pixel */
static int x_bitmap_scanline_pad; /* bitmap scanline_padding in bytes */
static int x_colors;  /* colors in the palette (undefined when there's no palette) */
static int x_have_palette;
static int x_input_encoding;	/* locales encoding */
static int x_bitmap_bit_order;


static Window x_root_window, fake_window;
static int fake_window_initialized = 0;
static GC x_normal_gc = 0, x_copy_gc = 0, x_drawbitmap_gc = 0, x_scroll_gc = 0;
static Colormap x_colormap;
static Atom x_delete_window_atom, x_wm_protocols_atom, x_sel_atom, x_targets_atom, x_utf8_string_atom;
static Visual* x_default_visual;
static Pixmap x_icon = 0;

#ifdef X_INPUT_METHOD
static XIM xim = NULL;
#endif

extern struct graphics_driver x_driver;

static unsigned char *x_driver_param=NULL;
static int n_wins;	/* number of windows */


#define X_TYPE_PIXMAP 1
#define X_TYPE_IMAGE 2

struct x_pixmapa
{
	unsigned char type;
	union
	{
		XImage *image;
		Pixmap *pixmap;
	}data;
};


static struct
{
	unsigned char count;
	struct graphics_device **pointer;
}
x_hash_table[X_HASH_TABLE_SIZE];

/* string in clipboard is in UTF-8 */
static unsigned char * x_my_clipboard=NULL;

struct window_info {
#ifdef X_INPUT_METHOD
	XIC xic;
#endif
	Window window;
};

static inline struct window_info *get_window_info(struct graphics_device *gd)
{
	return gd->driver_data;
}

/*----------------------------------------------------------------------*/

/* Tyhle opicarny tu jsou pro zvyseni rychlosti. Flush se nedela pri kazde operaci, ale
 * rekne se, ze je potreba udelat flush, a zaregistruje se bottom-half, ktery flush
 * provede. Takze jakmile se vrati rizeni do select smycky, tak se provede flush.
 */

static void x_wait_for_event(void)
{
	can_read_timeout(x_fd, -1);
}

static void x_process_events(void *data);

static unsigned char flush_in_progress=0;
static unsigned char process_events_in_progress=0;

static inline void X_SCHEDULE_PROCESS_EVENTS(void)
{
	if (!process_events_in_progress)
	{
		register_bottom_half(x_process_events,NULL);
		process_events_in_progress = 1;
	}
}

static void x_do_flush(void *ignore)
{
	/* kdyz budu mit zaregistrovanej bottom-half na tuhle funkci a nekdo mi
	 * tipne Xy, tak se nic nedeje, maximalne se zavola XFlush na blbej
	 * display, ale Xy se nepodelaj */

	flush_in_progress=0;
	XFlush(x_display);
	X_SCHEDULE_PROCESS_EVENTS();
}

static inline void X_FLUSH(void)
{
#ifdef INTERIX
	/*
	 * Interix has some bug, it locks up in _XWaitForWritable.
	 * As a workaround, do synchronous flushes.
	 */
	x_do_flush(NULL);
#else
	if (!flush_in_progress)
	{
		register_bottom_half(x_do_flush,NULL);
		flush_in_progress=1;
	}
#endif
}

static int (*old_error_handler)(Display *, XErrorEvent *) = NULL;
static int failure_happened;

static int failure_handler(Display *d, XErrorEvent *e)
{
	failure_happened = 1;
	return 0;
}

static void x_prepare_for_failure(void)
{
	if (old_error_handler)
		internal("x_prepare_for_failure: double call");
	failure_happened = 0;
	old_error_handler = XSetErrorHandler(failure_handler);
}

static int x_test_for_failure(void)
{
	XSync(x_display,False);
	X_SCHEDULE_PROCESS_EVENTS();
	XSetErrorHandler(old_error_handler);
	old_error_handler = NULL;
	return failure_happened;
}

/* suppose l<h */
static void x_clip_number(int *n,int l,int h)
{
	if ((*n)<l)*n=l;
	if ((*n)>h)*n=h;
}


static unsigned char * x_set_palette(void)
{
	XColor color;
	unsigned a;
	unsigned limit = 1U << (x_depth <= 24 ? x_depth : 24);

	x_colormap=XCreateColormap(x_display,x_root_window,x_default_visual,AllocAll);
	XInstallColormap(x_display,x_colormap);

	for (a = 0; a < limit; a++) {
		unsigned rgb[3];
		q_palette(limit, a, 65535, rgb);
		color.red = rgb[0];
		color.green = rgb[1];
		color.blue = rgb[2];
		color.pixel = a;
		color.flags = DoRed | DoGreen | DoBlue;
		XStoreColor(x_display, x_colormap, &color);
	}

	X_FLUSH();
	return NULL;
}

static unsigned char static_color_table[256];
static int use_static_color_table;

static unsigned char *x_query_palette(void)
{
	int i, j;
	XColor color[256];
	unsigned char valid[256];
	int some_valid = 0;
	memset(color, 0, sizeof color);
	memset(valid, 0, sizeof valid);
	for (i = 0; i < 1 << x_depth; i++) {
		color[i].pixel = i;
		x_prepare_for_failure();
		XQueryColor(x_display,XDefaultColormap(x_display,x_screen), &color[i]);
		if (!x_test_for_failure()) {
			valid[i] = 1;
			some_valid = 1;
		}
	}
	if (!some_valid)
		return stracpy(cast_uchar "Could not query static colormap\n");
	for (i = 0; i < 1 << x_depth; i++) {
		double best_distance = 0;
		int best = -1;
		unsigned rgb[3];
		q_palette(1U << x_depth, i, 65535, rgb);
		for (j = 0; j < 1 << x_depth; j++) {
			double distance;
			if (!valid[j]) continue;
			distance = rgb_distance(rgb[0], rgb[1], rgb[2], color[j].red, color[j].green, color[j].blue);
			if (best == -1 || distance < best_distance) {
				best = j;
				best_distance = distance;
			}
		}
		static_color_table[i] = (unsigned char)best;
	}
	use_static_color_table = 1;
	return NULL;
}


static inline int trans_key(unsigned char * str, int table)
{
	if (table==utf8_table){int a; GET_UTF_8(str,a);return a;}
	if (*str<128)return *str;
	return cp2u(*str,table);
}


/* translates X keys to links representation */
/* return value: 1=valid key, 0=nothing */
static int x_translate_key(struct graphics_device *gd, XKeyEvent *e,int *key,int *flag)
{
	KeySym ks = 0;
	static XComposeStatus comp = { NULL, 0 };
	static unsigned char str[16];
#define str_size	((int)(sizeof(str) - 1))
	int table = x_input_encoding < 0 ? g_kbd_codepage(&x_driver) : x_input_encoding;
	int len;

#ifdef X_INPUT_METHOD
	if (get_window_info(gd)->xic) {
		Status status;
#ifndef HAVE_XUTF8LOOKUPSTRING
		{
			wchar_t wc;
			len = XwcLookupString(get_window_info(gd)->xic, e, &wc, 1, &ks, &status);
			if (len == 1) {
				strcpy(cast_char str, cast_const_char encode_utf_8(wc));
				len = strlen(cast_const_char str);
			} else
				len = 0;
		}
#else
		{
			len = Xutf8LookupString(get_window_info(gd)->xic, e, cast_char str, str_size, &ks, &status);
		}
#endif
		table = utf8_table;
		/*fprintf(stderr, "len: %d, ks %ld, status %d\n", len, ks, status);*/
	} else
#endif

	{
		len = XLookupString(e,cast_char str,str_size,&ks,&comp);
	}
	str[len>str_size?str_size:len]=0;
	if (!len) str[0]=(unsigned char)ks, str[1]=0;
	*flag=0;
	*key=0;

	/* alt, control, shift ... */
	if (e->state&ShiftMask)*flag|=KBD_SHIFT;
	if (e->state&ControlMask)*flag|=KBD_CTRL;
	if (e->state&Mod1Mask)*flag|=KBD_ALT;

	/* alt-f4 */
	if (((*flag)&KBD_ALT)&&(ks==XK_F4)){*key=KBD_CTRL_C;*flag=0;return 1;}

	/* ctrl-c */
	if (((*flag)&KBD_CTRL)&&(ks==XK_c||ks==XK_C)){*key=KBD_CTRL_C;*flag=0;return 1;}

	if (ks == NoSymbol) { return 0;
	} else if (ks == XK_Return) { *key=KBD_ENTER;
	} else if (ks == XK_BackSpace) { *key=KBD_BS;
	} else if (ks == XK_Tab
#ifdef XK_KP_Tab
		|| ks == XK_KP_Tab
#endif
		) { *key=KBD_TAB;
	} else if (ks == XK_Escape) {
		*key=KBD_ESC;
	} else if (ks == XK_Left
#ifdef XK_KP_Left
		|| ks == XK_KP_Left
#endif
		) { *key=KBD_LEFT;
	} else if (ks == XK_Right
#ifdef XK_KP_Right
		|| ks == XK_KP_Right
#endif
		) { *key=KBD_RIGHT;
	} else if (ks == XK_Up
#ifdef XK_KP_Up
		|| ks == XK_KP_Up
#endif
		) { *key=KBD_UP;
	} else if (ks == XK_Down
#ifdef XK_KP_Down
		|| ks == XK_KP_Down
#endif
		) { *key=KBD_DOWN;
	} else if (ks == XK_Insert
#ifdef XK_KP_Insert
		|| ks == XK_KP_Insert
#endif
		) { *key=KBD_INS;
	} else if (ks == XK_Delete
#ifdef XK_KP_Delete
		|| ks == XK_KP_Delete
#endif
		) { *key=KBD_DEL;
	} else if (ks == XK_Home
#ifdef XK_KP_Home
		|| ks == XK_KP_Home
#endif
		) { *key=KBD_HOME;
	} else if (ks == XK_End
#ifdef XK_KP_End
		|| ks == XK_KP_End
#endif
		) { *key=KBD_END;
	} else if (0
#ifdef XK_KP_Page_Up
		|| ks == XK_KP_Page_Up
#endif
#ifdef XK_Page_Up
		|| ks == XK_Page_Up
#endif
		) { *key=KBD_PAGE_UP;
	} else if (0
#ifdef XK_KP_Page_Down
		|| ks == XK_KP_Page_Down
#endif
#ifdef XK_Page_Down
		|| ks == XK_Page_Down
#endif
		) { *key=KBD_PAGE_DOWN;
	} else if (ks == XK_F1
#ifdef XK_KP_F1
		|| ks == XK_KP_F1
#endif
		) { *key=KBD_F1;
	} else if (ks == XK_F2
#ifdef XK_KP_F2
		|| ks == XK_KP_F2
#endif
		) { *key=KBD_F2;
	} else if (ks == XK_F3
#ifdef XK_KP_F3
		|| ks == XK_KP_F3
#endif
		) { *key=KBD_F3;
	} else if (ks == XK_F4
#ifdef XK_KP_F4
		|| ks == XK_KP_F4
#endif
		) { *key=KBD_F4;
	} else if (ks == XK_F5) { *key=KBD_F5;
	} else if (ks == XK_F6) { *key=KBD_F6;
	} else if (ks == XK_F7) { *key=KBD_F7;
	} else if (ks == XK_F8) { *key=KBD_F8;
	} else if (ks == XK_F9) { *key=KBD_F9;
	} else if (ks == XK_F10) { *key=KBD_F10;
	} else if (ks == XK_F11) { *key=KBD_F11;
	} else if (ks == XK_F12) { *key=KBD_F12;
	} else if (ks == XK_KP_Subtract) { *key='-';
	} else if (ks == XK_KP_Decimal) { *key='.';
	} else if (ks == XK_KP_Divide) { *key='/';
	} else if (ks == XK_KP_Space) { *key=' ';
	} else if (ks == XK_KP_Enter) { *key=KBD_ENTER;
	} else if (ks == XK_KP_Equal) { *key='=';
	} else if (ks == XK_KP_Multiply) { *key='*';
	} else if (ks == XK_KP_Add) { *key='+';
	} else if (ks == XK_KP_0) { *key='0';
	} else if (ks == XK_KP_1) { *key='1';
	} else if (ks == XK_KP_2) { *key='2';
	} else if (ks == XK_KP_3) { *key='3';
	} else if (ks == XK_KP_4) { *key='4';
	} else if (ks == XK_KP_5) { *key='5';
	} else if (ks == XK_KP_6) { *key='6';
	} else if (ks == XK_KP_7) { *key='7';
	} else if (ks == XK_KP_8) { *key='8';
	} else if (ks == XK_KP_9) { *key='9';
	} else if (ks & 0x8000) { return 0;
	} else { *key=((*flag)&KBD_CTRL)?(int)ks&255:trans_key(str,table);
	}
	return 1;
}

static void x_hash_table_init(void)
{
	int a;

	for (a=0;a<X_HASH_TABLE_SIZE;a++)
	{
		x_hash_table[a].count=0;
		x_hash_table[a].pointer=NULL;
	}
}

static void x_clear_clipboard(void);

static void x_free_hash_table(void)
{
	int a,b;

	unregister_bottom_half(x_process_events,NULL);
	unregister_bottom_half(x_do_flush,NULL);

	for (a=0;a<X_HASH_TABLE_SIZE;a++)
	{
		for (b=0;b<x_hash_table[a].count;b++)
			mem_free(x_hash_table[a].pointer[b]);
		if (x_hash_table[a].pointer)
			mem_free(x_hash_table[a].pointer);
	}

	x_clear_clipboard();

	if (x_display) {
		if (x_icon) XFreePixmap(x_display, x_icon), x_icon = 0;
		if (fake_window_initialized) XDestroyWindow(x_display,fake_window), fake_window_initialized = 0;
		if (x_normal_gc) XFreeGC(x_display,x_normal_gc), x_normal_gc = 0;
		if (x_copy_gc) XFreeGC(x_display,x_copy_gc), x_copy_gc = 0;
		if (x_drawbitmap_gc) XFreeGC(x_display,x_drawbitmap_gc), x_drawbitmap_gc = 0;
		if (x_scroll_gc) XFreeGC(x_display,x_scroll_gc), x_scroll_gc = 0;
#ifdef X_INPUT_METHOD
		if (xim) XCloseIM(xim), xim = NULL;
#endif
		XCloseDisplay(x_display), x_display = NULL;
	}

	if (x_driver_param) mem_free(x_driver_param), x_driver_param = NULL;
	if (x_display_string) mem_free(x_display_string), x_display_string = NULL;
}



/* returns graphics device structure which belonging to the window */
static struct graphics_device *x_find_gd(Window *win)
{
	int a,b;

	a=(int)(*win)&(X_HASH_TABLE_SIZE-1);
	if (!x_hash_table[a].count)return 0;
	for (b=0;b<x_hash_table[a].count;b++)
	{
		if (get_window_info(x_hash_table[a].pointer[b])->window == *win)
			return x_hash_table[a].pointer[b];
	}
	return NULL;
}

static void x_update_driver_param(int w, int h)
{
	int l=0;

	if (n_wins!=1)return;

	x_default_window_width=w;
	x_default_window_height=h;

	if (x_driver_param)mem_free(x_driver_param);
	x_driver_param=init_str();
	add_num_to_str(&x_driver_param,&l,x_default_window_width);
	add_to_str(&x_driver_param,&l,cast_uchar "x");
	add_num_to_str(&x_driver_param,&l,x_default_window_height);
}



/* adds graphics device to hash table */
static void x_add_to_table(struct graphics_device* gd)
{
	int a=(int)get_window_info(gd)->window & (X_HASH_TABLE_SIZE-1);
	int c=x_hash_table[a].count;

	if (!c) {
		x_hash_table[a].pointer=mem_alloc(sizeof(struct graphics_device *));
	} else {
		if ((unsigned)c > MAXINT / sizeof(struct graphics_device *) - 1) overalloc();
		x_hash_table[a].pointer=mem_realloc(x_hash_table[a].pointer,(c+1)*sizeof(struct graphics_device *));
	}

	x_hash_table[a].pointer[c]=gd;
	x_hash_table[a].count++;
}


/* removes graphics device from table */
static void x_remove_from_table(Window *win)
{
	int a=(int)(*win)&(X_HASH_TABLE_SIZE-1);
	int b;

	for (b=0;b<x_hash_table[a].count;b++)
		if (get_window_info(x_hash_table[a].pointer[b])->window == *win)
		{
			memmove(x_hash_table[a].pointer+b,x_hash_table[a].pointer+b+1,(x_hash_table[a].count-b-1)*sizeof(struct graphics_device *));
			x_hash_table[a].count--;
			x_hash_table[a].pointer=mem_realloc(x_hash_table[a].pointer,x_hash_table[a].count*sizeof(struct graphics_device*));
		}
}


static void x_clear_clipboard(void)
{
	if (x_my_clipboard) {
		mem_free(x_my_clipboard);
		x_my_clipboard = NULL;
	}
}


static void x_process_events(void *data)
{
	XEvent event;
	XEvent last_event;
	struct graphics_device *gd;
	int last_was_mouse;
	int replay_event = 0;

#ifdef OPENVMS
	clear_events(x_fd, 0);
#endif

	process_events_in_progress = 0;

#ifdef SC_DEBUG
	MESSAGE("x_process_event\n");
#endif
	memset(&last_event, 0, sizeof last_event);	/* against warning */
	last_was_mouse=0;
	while (XPending(x_display) || replay_event)
	{
		if (replay_event) replay_event = 0;
		else XNextEvent(x_display,&event);
		if (last_was_mouse&&(event.type==ButtonPress||event.type==ButtonRelease))  /* this is end of mouse move block --- call mouse handler */
		{
			int a,b;

			last_was_mouse=0;
#ifdef X_DEBUG
			MESSAGE("(MotionNotify event)\n");
			{
				unsigned char txt[256];
				sprintf(txt,"x=%d y=%d\n",last_event.xmotion.x,last_event.xmotion.y);
				MESSAGE(txt);
			}
#endif
			gd=x_find_gd(&(last_event.xmotion.window));
			if (!gd)break;
			a=B_LEFT;
			b=B_MOVE;
			if ((last_event.xmotion.state)&Button1Mask)
			{
				a=B_LEFT;
				b=B_DRAG;
#ifdef X_DEBUG
				MESSAGE("left button/drag\n");
#endif
			}
			if ((last_event.xmotion.state)&Button2Mask)
			{
				a=B_MIDDLE;
				b=B_DRAG;
#ifdef X_DEBUG
				MESSAGE("middle button/drag\n");
#endif
			}
			if ((last_event.xmotion.state)&Button3Mask)
			{
				a=B_RIGHT;
				b=B_DRAG;
#ifdef X_DEBUG
				MESSAGE("right button/drag\n");
#endif
			}
			x_clip_number(&(last_event.xmotion.x),gd->size.x1,gd->size.x2);
			x_clip_number(&(last_event.xmotion.y),gd->size.y1,gd->size.y2);
			gd->mouse_handler(gd,last_event.xmotion.x,last_event.xmotion.y,a|b);
		}

		switch(event.type)
		{
			case GraphicsExpose:  /* redraw uncovered area during scroll */
			{
				struct rect r;

#ifdef X_DEBUG
				MESSAGE("(GraphicsExpose event)\n");
#endif
				gd=x_find_gd(&(event.xgraphicsexpose.drawable));
				if (!gd)break;
				r.x1=event.xgraphicsexpose.x;
				r.y1=event.xgraphicsexpose.y;
				r.x2=event.xgraphicsexpose.x+event.xgraphicsexpose.width;
				r.y2=event.xgraphicsexpose.y+event.xgraphicsexpose.height;
				gd->redraw_handler(gd,&r);
			}
			break;

			case Expose:   /* redraw part of the window */
			{
				struct rect r;

#ifdef X_DEBUG
				MESSAGE("(Expose event)\n");
#endif

				gd=x_find_gd(&(event.xexpose.window));
				if (!gd)break;
				r.x1=event.xexpose.x;
				r.y1=event.xexpose.y;
				r.x2=event.xexpose.x+event.xexpose.width;
				r.y2=event.xexpose.y+event.xexpose.height;
				gd->redraw_handler(gd,&r);
			}
			break;

			case ConfigureNotify:   /* resize window */
#ifdef X_DEBUG
			MESSAGE("(ConfigureNotify event)\n");
			{
				unsigned char txt[256];
				sprintf(txt,"width=%d height=%d\n",event.xconfigure.width,event.xconfigure.height);
				MESSAGE(txt);
			}
#endif
			gd=x_find_gd(&(event.xconfigure.window));
			if (!gd)break;
			/* when window only moved and size is the same, do nothing */
			if (gd->size.x2==event.xconfigure.width&&gd->size.y2==event.xconfigure.height)break;
			configure_notify_again:
			gd->size.x2=event.xconfigure.width;
			gd->size.y2=event.xconfigure.height;
			x_update_driver_param(event.xconfigure.width, event.xconfigure.height);
			while (XCheckWindowEvent(x_display,get_window_info(gd)->window,ExposureMask,&event)==True)
				;
			if (XCheckWindowEvent(x_display,get_window_info(gd)->window,StructureNotifyMask,&event)==True) {
				if (event.type==ConfigureNotify) goto configure_notify_again;
				replay_event=1;
			}
			gd->resize_handler(gd);
			break;

			case KeyPress:   /* a key was pressed */
			{
				int f,k;
#ifdef X_DEBUG
				MESSAGE("(KeyPress event)\n");
				{
					unsigned char txt[256];
					sprintf(txt,"keycode=%d state=%d\n",event.xkey.keycode,event.xkey.state);
					MESSAGE(txt);
				}
#endif
#ifdef X_INPUT_METHOD
				if (XFilterEvent(&event, None))
					break;
#endif
				gd=x_find_gd(&(event.xkey.window));
				if (!gd)break;
				if (x_translate_key(gd, (XKeyEvent*)(&event),&k,&f))
					gd->keyboard_handler(gd,k,f);
			}
			break;

			case ClientMessage:
			if (
				event.xclient.format!=32||
				event.xclient.message_type!=x_wm_protocols_atom||
				(Atom)event.xclient.data.l[0]!=x_delete_window_atom
			)break;

			/* This event is destroy window event from window manager */

			case DestroyNotify:
#ifdef X_DEBUG
			MESSAGE("(DestroyNotify event)\n");
#endif
			gd=x_find_gd(&(event.xkey.window));
			if (!gd)break;

			gd->keyboard_handler(gd,KBD_CLOSE,0);
			break;

			case ButtonRelease:    /* mouse button was released */
			{
				int a;
#ifdef X_DEBUG
				MESSAGE("(ButtonRelease event)\n");
				{
					unsigned char txt[256];
					sprintf(txt,"x=%d y=%d buttons=%d mask=%d\n",event.xbutton.x,event.xbutton.y,event.xbutton.button,event.xbutton.state);
					MESSAGE(txt);
				}
#endif
				gd=x_find_gd(&(event.xbutton.window));
				if (!gd)break;
				last_was_mouse=0;
				switch(event.xbutton.button)
				{
					case 1:
					a=B_LEFT;
					break;

					case 3:
					a=B_RIGHT;
					break;

					case 2:
					a=B_MIDDLE;
					break;

					case 8:
					a=B_FOURTH;
					break;

					case 9:
					a=B_FIFTH;
					break;

					default:
					goto r_xx;

				}
				x_clip_number(&(event.xmotion.x),gd->size.x1,gd->size.x2);
				x_clip_number(&(event.xmotion.y),gd->size.y1,gd->size.y2);
				gd->mouse_handler(gd,event.xbutton.x,event.xbutton.y,a|B_UP);
				r_xx:;
			}
			break;

			case ButtonPress:    /* mouse button was pressed */
			{
				int a;
#ifdef X_DEBUG
				MESSAGE("(ButtonPress event)\n");
				{
					unsigned char txt[256];
					sprintf(txt,"x=%d y=%d buttons=%d mask=%d\n",event.xbutton.x,event.xbutton.y,event.xbutton.button,event.xbutton.state);
					MESSAGE(txt);
				}
#endif
				gd=x_find_gd(&(event.xbutton.window));
				if (!gd)break;
				last_was_mouse=0;
				switch(event.xbutton.button)
				{
					case 1:
					a=B_LEFT;
					break;

					case 3:
					a=B_RIGHT;
					break;

					case 2:
					a=B_MIDDLE;
					break;

					case 4:
					a=B_WHEELUP;
					break;

					case 5:
					a=B_WHEELDOWN;
					break;

					case 6:
					a=B_WHEELLEFT;
					break;

					case 7:
					a=B_WHEELRIGHT;
					break;

					case 8:
					a=B_FOURTH;
					break;

					case 9:
					a=B_FIFTH;
					break;

					default:
					goto p_xx;
				}
				x_clip_number(&(event.xmotion.x),gd->size.x1,gd->size.x2);
				x_clip_number(&(event.xmotion.y),gd->size.y1,gd->size.y2);
				gd->mouse_handler(gd,event.xbutton.x,event.xbutton.y,a|(a != B_WHEELDOWN && a != B_WHEELUP && a != B_WHEELLEFT && a != B_WHEELRIGHT ? B_DOWN : B_MOVE));
				p_xx:;
			}
			break;

			case MotionNotify:   /* pointer moved */
			{
#ifdef X_DEBUG
				MESSAGE("(MotionNotify event)\n");
				{
					unsigned char txt[256];
					sprintf(txt,"x=%d y=%d\n",event.xmotion.x,event.xmotion.y);
					MESSAGE(txt);
				}
#endif
				/* just sign, that this was mouse event */
				last_was_mouse=1;
				last_event=event;
			}
			break;

			/* read clipboard */
			case SelectionNotify:
#ifdef X_DEBUG
			MESSAGE("xselectionnotify\n");
#endif
			/* handled in x_get_clipboard_text */
			break;

/* This long code must be here in order to implement copying of stuff into the clipboard */
			case SelectionRequest:
			{
				selection_request(&event);
			}
			break;

			case MapNotify:
			XFlush (x_display);
			break;

			default:
#ifdef X_DEBUG
			{
				unsigned char txt[256];
				sprintf(txt,"event=%d\n",event.type);
				MESSAGE(txt);
			}
#endif
			break;
		}
	}

	if (last_was_mouse)  /* that was end of mouse move block --- call mouse handler */
	{
		int a,b;

		last_was_mouse=0;
#ifdef X_DEBUG
		MESSAGE("(MotionNotify event)\n");
		/*
		{
			unsigned char txt[256];
			sprintf(txt,"x=%d y=%d\n",last_event.xmotion.x,last_event.xmotion.y);
			MESSAGE(txt);
		}
		*/
#endif
		gd=x_find_gd(&(last_event.xmotion.window));
		if (!gd)goto ret;
		a=B_LEFT;
		b=B_MOVE;
		if ((last_event.xmotion.state)&Button1Mask)
		{
			a=B_LEFT;
			b=B_DRAG;
#ifdef X_DEBUG
			MESSAGE("left button/drag\n");
#endif
		}
		if ((last_event.xmotion.state)&Button2Mask)
		{
			a=B_MIDDLE;
			b=B_DRAG;
#ifdef X_DEBUG
			MESSAGE("middle button/drag\n");
#endif
		}
		if ((last_event.xmotion.state)&Button3Mask)
		{
			a=B_RIGHT;
			b=B_DRAG;
#ifdef X_DEBUG
			MESSAGE("right button/drag\n");
#endif
		}
		x_clip_number(&(last_event.xmotion.x),gd->size.x1,gd->size.x2);
		x_clip_number(&(last_event.xmotion.y),gd->size.y1,gd->size.y2);
		gd->mouse_handler(gd,last_event.xmotion.x,last_event.xmotion.y,a|b);
	}
	ret:;
#ifdef SC_DEBUG
	MESSAGE("x_process_event end\n");
#endif
}


/* returns pointer to string with driver parameter or NULL */
static unsigned char * x_get_driver_param(void)
{
	return x_driver_param;
}

static unsigned char *x_get_af_unix_name(void)
{
	return x_display_string;
}

#ifdef X_INPUT_METHOD
static XIC x_open_xic(Window w);
#endif

/* initiate connection with X server */
static unsigned char * x_init_driver(unsigned char *param, unsigned char *display)
{
	XGCValues gcv;
	XSetWindowAttributes win_attr;
	XVisualInfo vinfo;
	int misordered=-1;

	x_hash_table_init();

	n_wins=0;

#if defined(HAVE_SETLOCALE) && defined(LC_CTYPE)
	setlocale(LC_CTYPE, "");
#endif
#ifdef X_DEBUG
	{
		unsigned char txt[256];
		sprintf(txt,"x_init_driver(%s, %s)\n",param, display);
		MESSAGE(txt);
	}
#endif
	x_input_encoding=-1;
#if defined(HAVE_NL_LANGINFO) && defined(HAVE_LANGINFO_H) && defined(CODESET) && !defined(WIN) && !defined(INTERIX)
	{
		unsigned char *cp;
		cp = cast_uchar nl_langinfo(CODESET);
		x_input_encoding=get_cp_index(cp);
	}
#endif
	if (!display||!(*display))display=NULL;

/*
	X documentation says on XOpenDisplay(display_name) :

	display_name
		Specifies the hardware display name, which determines the dis-
		play and communications domain to be used.  On a POSIX-confor-
		mant system, if the display_name is NULL, it defaults to the
		value of the DISPLAY environment variable.

	But OS/2 has problems when display_name is NULL ...

*/
	if (!display) display = cast_uchar getenv("DISPLAY");
#if !defined(__linux__) && !defined(OPENVMS)
	/* on Linux, do not assume XWINDOW present if $DISPLAY is not set
	   --- rather open links on svgalib or framebuffer console */
	if (!display) display = cast_uchar ":0.0";	/* needed for MacOS X */
#endif
	x_display_string = stracpy(display ? display : cast_uchar "");

	x_display = XOpenDisplay(cast_char display);
	if (!x_display)
	{
		unsigned char *err=init_str();
		int l=0;

		add_to_str(&err,&l,cast_uchar "Can't open display \"");
		add_to_str(&err,&l,display?display:(unsigned char *)"(null)");
		add_to_str(&err,&l,cast_uchar "\"\n");
		x_free_hash_table();
		return err;
	}

	x_bitmap_bit_order=BitmapBitOrder(x_display);
	x_screen=DefaultScreen(x_display);
	x_display_height=DisplayHeight(x_display,x_screen);
	x_display_width=DisplayWidth(x_display,x_screen);
	x_root_window=RootWindow(x_display,x_screen);

	x_default_window_width=x_display_width-50;
	x_default_window_height=x_display_height-50;

	x_driver_param=NULL;

	if (param && *param)
	{
		unsigned char *e;
		unsigned long w,h;

		x_driver_param=stracpy(param);

		if (*x_driver_param < '0' || *x_driver_param > '9') {
			invalid_param:
			x_free_hash_table();
			return stracpy(cast_uchar "Invalid parameter\n");
		}
		w=strtoul(cast_const_char x_driver_param,(char **)(void *)&e,10);
		if (upcase(*e) != 'X') goto invalid_param;
		e++;
		if (*e < '0' || *e > '9') goto invalid_param;
		h=strtoul(cast_const_char e,(char **)(void *)&e,10);
		if (*e) goto invalid_param;
		if (w && h && w <= MAXINT && h <= MAXINT) {
			x_default_window_width=(int)w;
			x_default_window_height=(int)h;
		}
	}

	/* find best visual */
	{
#define DEPTHS 5
#define CLASSES 3
		int depths[DEPTHS]={24, 16, 15, 8, 4};
		int classes[CLASSES]={TrueColor, PseudoColor, StaticColor}; /* FIXME: dodelat DirectColor */
		int a,b;

		for (a=0;a<DEPTHS;a++)
			for (b=0;b<CLASSES;b++)
			{
				if (XMatchVisualInfo(x_display, x_screen,depths[a],classes[b], &vinfo))
				{
					XPixmapFormatValues *pfm;
					int n,i;

					x_default_visual=vinfo.visual;
					x_depth=vinfo.depth;

					if (classes[b] == StaticColor && depths[a] > 8)
						continue;

					/* determine bytes per pixel */
					pfm=XListPixmapFormats(x_display,&n);
					for (i=0;i<n;i++)
						if (pfm[i].depth==x_depth)
						{
							x_bitmap_bpp=pfm[i].bits_per_pixel<8?1:((pfm[i].bits_per_pixel)>>3);
							x_bitmap_scanline_pad=(pfm[i].scanline_pad)>>3;
							XFree(pfm);
							goto bytes_per_pixel_found;
						}
					if(n) XFree(pfm);
					continue;
bytes_per_pixel_found:

					/* test misordered flag */
					switch(x_depth)
					{
						case 4:
						case 8:
						if (x_bitmap_bpp!=1)break;
						if (vinfo.red_mask>=vinfo.green_mask&&vinfo.green_mask>=vinfo.blue_mask)
						{
							misordered=0;
							goto visual_found;
						}
						break;

						case 15:
						case 16:
						if (x_bitmap_bpp!=2)break;
						if (x_bitmap_bit_order==MSBFirst&&vinfo.red_mask>vinfo.green_mask&&vinfo.green_mask>vinfo.blue_mask)
						{
							misordered=256;
							goto visual_found;
						}
						if (x_bitmap_bit_order==MSBFirst)break;
						if (vinfo.red_mask>vinfo.green_mask&&vinfo.green_mask>vinfo.blue_mask)
						{
							misordered=0;
							goto visual_found;
						}
						break;

						case 24:
						if (x_bitmap_bpp!=3&&x_bitmap_bpp!=4) break;
						if (vinfo.red_mask<vinfo.green_mask&&vinfo.green_mask<vinfo.blue_mask)
						{
							misordered=256;
							goto visual_found;
						}
						if (x_bitmap_bit_order==MSBFirst&&vinfo.red_mask>vinfo.green_mask&&vinfo.green_mask>vinfo.blue_mask)
						{
							misordered=512;
							goto visual_found;
						}
						if (vinfo.red_mask>vinfo.green_mask&&vinfo.green_mask>vinfo.blue_mask)
						{
							misordered=0;
							goto visual_found;
						}
						break;
					}
				}
			}

		x_free_hash_table();
		return stracpy(cast_uchar "No supported color depth found.\n");
visual_found:;
	}

	x_driver.depth=0;
	x_driver.depth|=x_bitmap_bpp;
	x_driver.depth|=x_depth<<3;
	x_driver.depth|=misordered;

	/* check if depth is sane */
	if (x_driver.depth == 707) x_driver.depth = 195;
	switch (x_driver.depth)
	{
		case 33:
		case 65:
		case 122:
		case 130:
		case 451:
		case 195:
		case 196:
		case 378:
		case 386:
		case 452:
		case 708:
/*			printf("depth=%d visualid=%x\n",x_driver.depth, vinfo.visualid); */
		break;

		default:
		{
			unsigned char nevidim_te_ani_te_neslysim_ale_smrdis_jako_lejno[MAX_STR_LEN];

			snprintf(cast_char nevidim_te_ani_te_neslysim_ale_smrdis_jako_lejno,MAX_STR_LEN,
			"Unsupported graphics mode: x_depth=%d, bits_per_pixel=%d, bytes_per_pixel=%d\n",x_driver.depth, x_depth, x_bitmap_bpp);
			x_free_hash_table();
			return stracpy(nevidim_te_ani_te_neslysim_ale_smrdis_jako_lejno);
		}

	}

	x_get_color_function=get_color_fn(x_driver.depth);
	if (!x_get_color_function) internal("Unknown bit depth: %d", x_driver.depth);

#ifdef X_DEBUG
	{
		unsigned char txt[256];
		sprintf(txt,"x_driver.depth=%d\n",x_driver.depth);
		MESSAGE(txt);
	}
#endif

	x_colors=1<<x_depth;
	x_have_palette=0;
	use_static_color_table = 0;
	if (vinfo.class==DirectColor||vinfo.class==PseudoColor)
	{
		unsigned char *t;

		x_have_palette=1;
		if((t=x_set_palette())){x_free_hash_table(); return t;}
	}
	if (vinfo.class==StaticColor)
	{
		unsigned char *t;
		if((t=x_query_palette())){x_free_hash_table(); return t;}
	}

	x_black_pixel=BlackPixel(x_display,x_screen);

	gcv.function=GXcopy;
	gcv.graphics_exposures=True;  /* we want to receive GraphicsExpose events when uninitialized area is discovered during scroll */
	gcv.fill_style=FillSolid;
	gcv.background=x_black_pixel;

	x_delete_window_atom = XInternAtom(x_display,"WM_DELETE_WINDOW", False);
	x_wm_protocols_atom = XInternAtom(x_display,"WM_PROTOCOLS", False);
	x_sel_atom = XInternAtom(x_display, "SEL_PROP", False);
	x_targets_atom = XInternAtom(x_display, "TARGETS", False);
	x_utf8_string_atom = XInternAtom(x_display, "UTF8_STRING", False);

	if (x_have_palette) win_attr.colormap=x_colormap;
	else win_attr.colormap=XCreateColormap(x_display, x_root_window, x_default_visual, AllocNone);

	win_attr.border_pixel=x_black_pixel;

	fake_window=XCreateWindow(
		x_display,
		x_root_window,
		0,
		0,
		10,
		10,
		0,
		x_depth,
		CopyFromParent,
		x_default_visual,
		CWColormap|CWBorderPixel,
		&win_attr
	);

	fake_window_initialized = 1;

	x_normal_gc=XCreateGC(x_display,fake_window,GCFillStyle|GCBackground,&gcv);
	if (!x_normal_gc) {x_free_hash_table(); return stracpy(cast_uchar "Cannot create graphic context.\n");}

	x_copy_gc=XCreateGC(x_display,fake_window,GCFunction,&gcv);
	if (!x_copy_gc) {x_free_hash_table(); return stracpy(cast_uchar "Cannot create graphic context.\n");}

	x_drawbitmap_gc=XCreateGC(x_display,fake_window,GCFunction,&gcv);
	if (!x_drawbitmap_gc) {x_free_hash_table(); return stracpy(cast_uchar "Cannot create graphic context.\n");}

	x_scroll_gc=XCreateGC(x_display,fake_window,GCGraphicsExposures|GCBackground,&gcv);
	if (!x_scroll_gc) {x_free_hash_table(); return stracpy(cast_uchar "Cannot create graphic context.\n");}

	XSetLineAttributes(x_display,x_normal_gc,1,LineSolid,CapRound,JoinRound);

#ifdef X_INPUT_METHOD
	{
#if defined(HAVE_SETLOCALE) && defined(LC_CTYPE)
		/*
		 * Unfortunatelly, dead keys are translated according to
		 * current locale, even if we use Xutf8LookupString.
		 * So, try to set locale to utf8 for the input method.
		 */
		unsigned char *l, *m, *d;
		l = cast_uchar setlocale(LC_CTYPE, "");
		if (l) {
			m = stracpy(l);
			d = cast_uchar strchr(cast_const_char m, '.');
			if (d) *d = 0;
			add_to_strn(&m, cast_uchar ".UTF-8");
			l = cast_uchar setlocale(LC_CTYPE, cast_const_char m);
			mem_free(m);
		}
		if (!l) {
			l = cast_uchar setlocale(LC_CTYPE, "en_US.UTF-8");
		}
#endif
		xim = XOpenIM(x_display, NULL, NULL, NULL);
		if (xim) {
			XIC xic = x_open_xic(fake_window);
			if (xic) {
				XDestroyIC(xic);
			} else {
				XCloseIM(xim), xim = NULL;
			}
		}
#if defined(HAVE_SETLOCALE) && defined(LC_CTYPE)
		setlocale(LC_CTYPE, "");
#endif
	}
#endif

	if (x_input_encoding<0
#ifdef X_INPUT_METHOD
		&& !xim
#endif
		) x_driver.flags|=GD_NEED_CODEPAGE;

	x_fd=XConnectionNumber(x_display);
#ifdef OPENVMS
	x_fd=vms_x11_fd(x_fd);
#endif
	set_handlers(x_fd, x_process_events, NULL, NULL);
	XSync(x_display, False);
	X_SCHEDULE_PROCESS_EVENTS();
	return NULL;
}


/* close connection with the X server */
static void x_shutdown_driver(void)
{
#ifdef X_DEBUG
	MESSAGE("x_shutdown_driver\n");
#endif
	set_handlers(x_fd, NULL, NULL, NULL);
	x_free_hash_table();
}

#ifdef X_INPUT_METHOD
static XIC x_open_xic(Window w)
{
	return XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, w, XNFocusWindow, w, NULL);
}
#endif

/* create new window */
static struct graphics_device* x_init_device(void)
{
	struct graphics_device *gd;
	XWMHints wm_hints;
	XClassHint class_hints;
	XTextProperty windowName;
	unsigned char *links_name=cast_uchar "Links";
	XSetWindowAttributes win_attr;
	struct window_info *wi;

#ifdef X_DEBUG
	MESSAGE("x_init_device\n");
#endif
	gd=mem_alloc(sizeof(struct graphics_device));

	wi=mem_calloc(sizeof(struct window_info));

	gd->size.x1=0;
	gd->size.y1=0;
	gd->size.x2=x_default_window_width;
	gd->size.y2=x_default_window_height;

	if (x_have_palette) win_attr.colormap=x_colormap;
	else win_attr.colormap=XCreateColormap(x_display, x_root_window, x_default_visual, AllocNone);
	win_attr.border_pixel=x_black_pixel;

	wi->window=XCreateWindow(
		x_display,
		x_root_window,
		gd->size.x1,
		gd->size.y1,
		gd->size.x2,
		gd->size.y2,
		X_BORDER_WIDTH,
		x_depth,
		InputOutput,
		x_default_visual,
		CWColormap|CWBorderPixel,
		&win_attr
	);
	if (!x_icon)
	{
		XImage *img;
		unsigned char *data;
		int w,h,skip;
		get_links_icon(&data,&w,&h,&skip,x_bitmap_scanline_pad);

		img=XCreateImage(x_display,x_default_visual,x_depth,ZPixmap,0,0,w,h,x_bitmap_scanline_pad<<3,w*((x_driver.depth)&7));
		if (!img){x_icon=0;goto nic_nebude_bobankove;}
		img->data=cast_char data;
		x_icon=XCreatePixmap(x_display,wi->window,w,h,x_depth);
		if (!x_icon){XDestroyImage(img);x_icon=0;goto nic_nebude_bobankove;}

		XPutImage(x_display,x_icon,x_copy_gc,img,0,0,0,0,w,h);
		XDestroyImage(img);
nic_nebude_bobankove:;
	}

	wm_hints.flags=InputHint;
	wm_hints.input=True;
	if (x_icon)
	{
		wm_hints.flags=InputHint|IconPixmapHint;
		wm_hints.icon_pixmap=x_icon;
	}

	XSetWMHints(x_display, wi->window, &wm_hints);
	class_hints.res_name = cast_char links_name;
	class_hints.res_class = cast_char links_name;
	XSetClassHint(x_display, wi->window, &class_hints);
	XStringListToTextProperty((char **)(void *)&links_name, 1, &windowName);
	XSetWMName(x_display, wi->window, &windowName);
	XStoreName(x_display,wi->window,cast_char links_name);
	XSetWMIconName(x_display, wi->window, &windowName);

	XMapWindow(x_display,wi->window);

	gd->clip.x1=gd->size.x1;
	gd->clip.y1=gd->size.y1;
	gd->clip.x2=gd->size.x2;
	gd->clip.y2=gd->size.y2;
	gd->driver_data=wi;
	gd->user_data=0;

	XSetWindowBackgroundPixmap(x_display, wi->window, None);
	if (x_have_palette) XSetWindowColormap(x_display,wi->window,x_colormap);
	x_add_to_table(gd);

	XSetWMProtocols(x_display,wi->window,&x_delete_window_atom,1);

	XSelectInput(x_display,wi->window,
		ExposureMask|
		KeyPressMask|
		ButtonPressMask|
		ButtonReleaseMask|
		PointerMotionMask|
		ButtonMotionMask|
		StructureNotifyMask|
		0
	);

#ifdef X_INPUT_METHOD
	if (xim) {
		wi->xic = x_open_xic(wi->window);
	}
#endif

	XSync(x_display,False);
	X_SCHEDULE_PROCESS_EVENTS();
	n_wins++;
	return gd;
}


/* close window */
static void x_shutdown_device(struct graphics_device *gd)
{
	struct window_info *wi = get_window_info(gd);
#ifdef X_DEBUG
	MESSAGE("x_shutdown_device\n");
#endif
	if (!gd)return;

	n_wins--;
	XDestroyWindow(x_display,wi->window);
#ifdef X_INPUT_METHOD
	if (wi->xic) {
		XDestroyIC(wi->xic);
	}
#endif
	XSync(x_display,False);
	X_SCHEDULE_PROCESS_EVENTS();

	x_remove_from_table(&wi->window);
	mem_free(wi);
	mem_free(gd);
}

static void x_translate_colors(unsigned char *data, int x, int y, int skip)
{
	int i, j;
	if (!use_static_color_table)
		return;
	for (j = 0; j < y; j++) {
		for (i = 0; i < x; i++)
			data[i] = static_color_table[data[i]];
		data += skip;
	}
}

static int x_get_empty_bitmap(struct bitmap *bmp)
{
	int pad;
#ifdef X_DEBUG
	MESSAGE("x_get_empty_bitmap\n");
#endif
	pad=x_bitmap_scanline_pad-((bmp->x*x_bitmap_bpp)%x_bitmap_scanline_pad);
	if (pad==x_bitmap_scanline_pad)pad=0;
	bmp->skip=bmp->x*x_bitmap_bpp+pad;
	bmp->flags=NULL;
	retry:
	if (!(bmp->data=malloc(bmp->skip*bmp->y))) {
		if (out_of_memory(0, NULL, 0))
			goto retry;
		return -1;
	}
	/* on error bmp->data should point to NULL */
	return 0;
}

static void x_register_bitmap(struct bitmap *bmp)
{
	struct x_pixmapa *p;
	XImage *image;
	Pixmap *pixmap = NULL;	/* shut up warning */
	int can_create_pixmap;

#ifdef X_DEBUG
	MESSAGE("x_register_bitmap\n");
#endif

	X_FLUSH();
	if (!bmp->data||!bmp->x||!bmp->y) goto cant_create;

	x_translate_colors(bmp->data, bmp->x, bmp->y, bmp->skip);

	/* alloc struct x_bitmapa */
	p=mem_alloc(sizeof(struct x_pixmapa));

	/* alloc XImage in client's memory */
	retry:
	image=XCreateImage(x_display,x_default_visual,x_depth,ZPixmap,0,0,bmp->x,bmp->y,x_bitmap_scanline_pad<<3,bmp->skip);
	if (!image){
		if (out_of_memory(0, NULL, 0))
			goto retry;
		mem_free(p);
		goto cant_create;
	}
	image->data=bmp->data;


	/* try to alloc XPixmap in server's memory */
	can_create_pixmap=1;

	if (bmp->x >= 32768 || bmp->y >= 32768) {
		can_create_pixmap = 0;
		goto no_pixmap;
	}

	x_prepare_for_failure();
	pixmap=mem_alloc(sizeof(Pixmap));
	(*pixmap)=XCreatePixmap(x_display,fake_window,bmp->x,bmp->y,x_depth);
	if (x_test_for_failure()) {
		if (*pixmap) {
			x_prepare_for_failure();
			XFreePixmap(x_display,*pixmap);
			x_test_for_failure();
			*pixmap=0;
		}
	}
	if (!(*pixmap)) {
		mem_free(pixmap);
		can_create_pixmap=0;
	}

no_pixmap:

	if (can_create_pixmap)
	{
#ifdef X_DEBUG
		MESSAGE("x_register_bitmap: creating pixmap\n");
#endif
		XPutImage(x_display,*pixmap,x_copy_gc,image,0,0,0,0,bmp->x,bmp->y);
		XDestroyImage(image);
		p->type=X_TYPE_PIXMAP;
		p->data.pixmap=pixmap;
	}
	else
	{
#ifdef X_DEBUG
		MESSAGE("x_register_bitmap: creating image\n");
#endif
		p->type=X_TYPE_IMAGE;
		p->data.image=image;
	}
	bmp->flags=p;
	bmp->data=NULL;
	return;

cant_create:
	if (bmp->data) free(bmp->data), bmp->data = NULL;
	bmp->flags=NULL;
	return;
}


static void x_unregister_bitmap(struct bitmap *bmp)
{
#ifdef X_DEBUG
	MESSAGE("x_unregister_bitmap\n");
#endif
	if (!bmp->flags) return;

	switch(XPIXMAPP(bmp->flags)->type)
	{
		case X_TYPE_PIXMAP:
		XFreePixmap(x_display,*(XPIXMAPP(bmp->flags)->data.pixmap));   /* free XPixmap from server's memory */
		mem_free(XPIXMAPP(bmp->flags)->data.pixmap);  /* XPixmap */
		break;

		case X_TYPE_IMAGE:
		XDestroyImage(XPIXMAPP(bmp->flags)->data.image);  /* free XImage from client's memory */
		break;
	}
	mem_free(bmp->flags);  /* struct x_pixmap */
}

static long x_get_color(int rgb)
{
	long block;
	unsigned char *b;

#ifdef X_DEBUG
	MESSAGE("x_get_color\n");
#endif
	block=x_get_color_function(rgb);
	b = (unsigned char *)&block;
	/*fprintf(stderr, "bitmap bpp %d\n", x_bitmap_bpp);*/
	switch (x_bitmap_bpp) {
		case 1:		if (use_static_color_table)
					return static_color_table[b[0]];
				return b[0];
		case 2:		if (x_bitmap_bit_order == LSBFirst)
					return b[0] | (b[1] << 8);
				else
					return b[1] | (b[0] << 8);
		case 3:		return b[0] | (b[1] << 8) | (b[2] << 16);
		default:	if (x_bitmap_bit_order == LSBFirst)
					return b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
				else
					return b[3] | (b[2] << 8) | (b[1] << 16) | (b[0] << 24);
	}
}


static void x_fill_area(struct graphics_device *gd, int x1, int y1, int x2, int y2, long color)
{
	/*int a;*/

#ifdef X_DEBUG
	{
		unsigned char txt[256];
		sprintf(txt,"x_fill_area (x1=%d y1=%d x2=%d y2=%d)\n",x1,y1,x2,y2);
		MESSAGE(txt);
	}
#endif
	/* Mikulas: v takovem pripade radsi neplnit nic ... */
	/*
	if (x1>x2){a=x2;x2=x1;x1=a;}
	if (y1>y2){a=y2;y2=y1;y1=a;}
	*/
	if (x1 < gd->clip.x1) x1 = gd->clip.x1;
	if (x2 > gd->clip.x2) x2 = gd->clip.x2;
	if (y1 < gd->clip.y1) y1 = gd->clip.y1;
	if (y2 > gd->clip.y2) y2 = gd->clip.y2;
	if (x1>=x2) return;
	if (y1>=y2) return;

	XSetForeground(x_display,x_normal_gc,color);
	XFillRectangle(
		x_display,
		get_window_info(gd)->window,
		x_normal_gc,
		x1,
		y1,
		x2-x1,
		y2-y1
	);
	X_FLUSH();
}


static void x_draw_hline(struct graphics_device *gd, int left, int y, int right, long color)
{
#ifdef X_DEBUG
	MESSAGE("x_draw_hline\n");
#endif
	if (left>=right)return;
	if ((y>=gd->clip.y2)||(y<gd->clip.y1)) return;
	if (right<=gd->clip.x1||left>=gd->clip.x2)return;
	XSetForeground(x_display,x_normal_gc,color);
	XDrawLine(
		x_display,
		get_window_info(gd)->window,
		x_normal_gc,
		left,y,right-1,y
	);
	X_FLUSH();
}


static void x_draw_vline(struct graphics_device *gd, int x, int top, int bottom, long color)
{
#ifdef X_DEBUG
	MESSAGE("x_draw_vline\n");
#endif
	if (top>=bottom)return;
	if ((x>=gd->clip.x2)||(x<gd->clip.x1)) return;
	if (bottom<=gd->clip.y1||top>=gd->clip.y2)return;
	XSetForeground(x_display,x_normal_gc,color);
	XDrawLine(
		x_display,
		get_window_info(gd)->window,
		x_normal_gc,
		x,top,x,bottom-1
	);
	X_FLUSH();
}


static void x_set_clip_area(struct graphics_device *gd, struct rect *r)
{
	XRectangle xr;

#ifdef X_DEBUG
	{
		unsigned char txt[512];
		snprintf(txt,512,"x_set_clip_area(x1=%d, y1=%d, x2=%d, y2=%d\n",r->x1,r->y1,r->x2,r->y2);
		MESSAGE(txt);
	}
#endif
	generic_set_clip_area(gd, r);

	xr.x=gd->clip.x1;
	xr.y=gd->clip.y1;
	xr.width=(gd->clip.x2)-(gd->clip.x1);
	xr.height=(gd->clip.y2)-(gd->clip.y1);

	XSetClipRectangles(x_display,x_normal_gc,0,0,&xr,1,Unsorted);
	XSetClipRectangles(x_display,x_scroll_gc,0,0,&xr,1,Unsorted);
	XSetClipRectangles(x_display,x_drawbitmap_gc,0,0,&xr,1,Unsorted);
	X_FLUSH();
}


static void x_draw_bitmap(struct graphics_device *gd, struct bitmap *bmp, int x, int y)
{
	int bmp_off_x, bmp_off_y, bmp_size_x, bmp_size_y;
#ifdef X_DEBUG
	MESSAGE("x_draw_bitmap\n");
#endif
	if (!(bmp->flags)||!bmp->x||!bmp->y) {
		return;
	}
	if ((x>=gd->clip.x2)||(y>=gd->clip.y2)) return;
	if ((x+(bmp->x)<=gd->clip.x1)||(y+(bmp->y)<=gd->clip.y1)) return;
	bmp_off_x = 0;
	bmp_off_y = 0;
	bmp_size_x = bmp->x;
	bmp_size_y = bmp->y;
	if (x < gd->clip.x1) {
		bmp_off_x = gd->clip.x1 - x;
		bmp_size_x -= gd->clip.x1 - x;
		x = gd->clip.x1;
	}
	if (x + bmp_size_x > gd->clip.x2) {
		bmp_size_x = gd->clip.x2 - x;
	}
	if (y < gd->clip.y1) {
		bmp_off_y = gd->clip.y1 - y;
		bmp_size_y -= gd->clip.y1 - y;
		y = gd->clip.y1;
	}
	if (y + bmp_size_y > gd->clip.y2) {
		bmp_size_y = gd->clip.y2 - y;
	}

	switch(XPIXMAPP(bmp->flags)->type)
	{
		case X_TYPE_PIXMAP:
		XCopyArea(x_display,*(XPIXMAPP(bmp->flags)->data.pixmap),get_window_info(gd)->window,x_drawbitmap_gc,bmp_off_x,bmp_off_y,bmp_size_x,bmp_size_y,x,y);
		break;

		case X_TYPE_IMAGE:
		XPutImage(x_display,get_window_info(gd)->window,x_drawbitmap_gc,XPIXMAPP(bmp->flags)->data.image,bmp_off_x,bmp_off_y,x,y,bmp_size_x,bmp_size_y);
		break;
	}
	X_FLUSH();
}


#if 0
static void x_draw_bitmaps(struct graphics_device *gd, struct bitmap **bmps, int n, int x, int y)
{
	int a;
#ifdef X_DEBUG
	MESSAGE("x_draw_bitmaps\n");
#endif

	if (!bmps)return;
	for (a=0;a<n;a++)
	{
		x_draw_bitmap(gd,bmps[a],x,y);
		x+=(bmps[a])->x;
	}
}
#endif



static int x_hscroll(struct graphics_device *gd, struct rect_set **set, int sc)
{
	XEvent ev;
	struct rect r;

	*set=NULL;
	if (!sc)return 0;
	*set=init_rect_set();
	if (!(*set))internal("Cannot allocate memory for rect set in scroll function.\n");

	XCopyArea(
		x_display,
		get_window_info(gd)->window,
		get_window_info(gd)->window,
		x_scroll_gc,
		gd->clip.x1,gd->clip.y1,
		gd->clip.x2-gd->clip.x1,gd->clip.y2-gd->clip.y1,
		gd->clip.x1+sc,gd->clip.y1
	);
	XSync(x_display,False);
	/* ten sync tady musi byt, protoze potrebuju zarucit, aby vsechny
	 * graphics-expose vyvolane timto scrollem byly vraceny v rect-set */

	/* take all graphics expose events for this window and put them into the rect set */
	while (XCheckWindowEvent(x_display,get_window_info(gd)->window,ExposureMask,&ev)==True)
	{
		switch(ev.type)
		{
			case GraphicsExpose:
			r.x1=ev.xgraphicsexpose.x;
			r.y1=ev.xgraphicsexpose.y;
			r.x2=ev.xgraphicsexpose.x+ev.xgraphicsexpose.width;
			r.y2=ev.xgraphicsexpose.y+ev.xgraphicsexpose.height;
			break;

			case Expose:
			r.x1=ev.xexpose.x;
			r.y1=ev.xexpose.y;
			r.x2=ev.xexpose.x+ev.xexpose.width;
			r.y2=ev.xexpose.y+ev.xexpose.height;
			break;

			default:
			continue;
		}
		if (r.x1 < gd->clip.x1 || r.x2 > gd->clip.x2 ||
		    r.y1 < gd->clip.y1 || r.y2 > gd->clip.y2) {
			switch(ev.type)
			{
				case GraphicsExpose:
				ev.xgraphicsexpose.x = 0;
				ev.xgraphicsexpose.y = 0;
				ev.xgraphicsexpose.width = gd->size.x2;
				ev.xgraphicsexpose.height = gd->size.y2;
				break;

				case Expose:
				ev.xexpose.x = 0;
				ev.xexpose.y = 0;
				ev.xexpose.width = gd->size.x2;
				ev.xexpose.height = gd->size.y2;
				break;
			}
			XPutBackEvent(x_display, &ev);
			mem_free(*set);
			*set = NULL;
			break;
		}
		add_to_rect_set(set,&r);
	}

	X_SCHEDULE_PROCESS_EVENTS();

#ifdef SC_DEBUG
	MESSAGE("hscroll\n");
#endif

	return 1;
}


static int x_vscroll(struct graphics_device *gd, struct rect_set **set, int sc)
{
	XEvent ev;
	struct rect r;

	*set=NULL;
	if (!sc)return 0;
	*set=init_rect_set();
	if (!(*set))internal("Cannot allocate memory for rect set in scroll function.\n");

	XCopyArea(
		x_display,
		get_window_info(gd)->window,
		get_window_info(gd)->window,
		x_scroll_gc,
		gd->clip.x1,gd->clip.y1,
		gd->clip.x2-gd->clip.x1,gd->clip.y2-gd->clip.y1,
		gd->clip.x1,gd->clip.y1+sc
	);
	XSync(x_display,False);
	/* ten sync tady musi byt, protoze potrebuju zarucit, aby vsechny
	 * graphics-expose vyvolane timto scrollem byly vraceny v rect-set */

	/* take all graphics expose events for this window and put them into the rect set */
	while (XCheckWindowEvent(x_display,get_window_info(gd)->window,ExposureMask,&ev)==True)
	{
		switch(ev.type)
		{
			case GraphicsExpose:
			r.x1=ev.xgraphicsexpose.x;
			r.y1=ev.xgraphicsexpose.y;
			r.x2=ev.xgraphicsexpose.x+ev.xgraphicsexpose.width;
			r.y2=ev.xgraphicsexpose.y+ev.xgraphicsexpose.height;
			break;

			case Expose:
			r.x1=ev.xexpose.x;
			r.y1=ev.xexpose.y;
			r.x2=ev.xexpose.x+ev.xexpose.width;
			r.y2=ev.xexpose.y+ev.xexpose.height;
			break;

			default:
			continue;
		}
		if (r.x1 < gd->clip.x1 || r.x2 > gd->clip.x2 ||
		    r.y1 < gd->clip.y1 || r.y2 > gd->clip.y2) {
			switch(ev.type)
			{
				case GraphicsExpose:
				ev.xgraphicsexpose.x = 0;
				ev.xgraphicsexpose.y = 0;
				ev.xgraphicsexpose.width = gd->size.x2;
				ev.xgraphicsexpose.height = gd->size.y2;
				break;

				case Expose:
				ev.xexpose.x = 0;
				ev.xexpose.y = 0;
				ev.xexpose.width = gd->size.x2;
				ev.xexpose.height = gd->size.y2;
				break;
			}
			XPutBackEvent(x_display, &ev);
			mem_free(*set);
			*set = NULL;
			break;
		}
		add_to_rect_set(set,&r);
	}

	X_SCHEDULE_PROCESS_EVENTS();

#ifdef SC_DEBUG
	MESSAGE("vscroll\n");
#endif

	return 1;
}


static void *x_prepare_strip(struct bitmap *bmp, int top, int lines)
{
	struct x_pixmapa *p=(struct x_pixmapa *)bmp->flags;
	XImage *image;
	void *x_data;

	if (!p) return NULL;

#ifdef DEBUG
	if (lines <= 0) internal("x_prepare_strip: %d lines",lines);
#endif

#ifdef X_DEBUG
	MESSAGE("x_prepare_strip\n");
#endif

	bmp->data = NULL;

	switch (p->type)
	{
		case X_TYPE_PIXMAP:

		retry:
		x_data=malloc(bmp->skip*lines);
		if (!x_data) {
			if (out_of_memory(0, NULL, 0))
				goto retry;
			return NULL;
		}

		retry2:
		image=XCreateImage(x_display,x_default_visual,x_depth,ZPixmap,0,0,bmp->x,lines,x_bitmap_scanline_pad<<3,bmp->skip);
		if (!image) {
			if (out_of_memory(0, NULL, 0))
				goto retry2;
			free(x_data);
			return NULL;
		}
		image->data = x_data;
		bmp->data=image;
		return image->data;

		case X_TYPE_IMAGE:
		return p->data.image->data+(bmp->skip*top);
	}
	internal("Unknown pixmap type found in x_prepare_strip. SOMETHING IS REALLY STRANGE!!!!\n");
	return NULL;
}


static void x_commit_strip(struct bitmap *bmp, int top, int lines)
{
	struct x_pixmapa *p=(struct x_pixmapa *)bmp->flags;

	if (!p) return;

#ifdef X_DEBUG
	MESSAGE("x_commit_strip\n");
#endif
	switch(p->type)
	{
		/* send image to pixmap in xserver */
		case X_TYPE_PIXMAP:
		if (!bmp->data) return;
		x_translate_colors((unsigned char *)((XImage*)bmp->data)->data, bmp->x, lines, bmp->skip);
		XPutImage(x_display,*(XPIXMAPP(bmp->flags)->data.pixmap),x_copy_gc,(XImage*)bmp->data,0,0,0,top,bmp->x,lines);
		XDestroyImage((XImage *)bmp->data);
		return;

		case X_TYPE_IMAGE:
		x_translate_colors((unsigned char *)p->data.image->data+(bmp->skip*top), bmp->x, lines, bmp->skip);
		/* everything has been done by user */
		return;
	}
}


static void x_set_window_title(struct graphics_device *gd, unsigned char *title)
{
	struct conv_table *ct;
	unsigned char *t;
	XTextProperty windowName;
	int output_encoding;
	Status ret;

#if defined(HAVE_XSUPPORTSLOCALE) && defined(HAVE_XMBTEXTLISTTOTEXTPROPERTY)
	if (XSupportsLocale()) {
		output_encoding = x_input_encoding >= 0 ? x_input_encoding : 0;
	} else
retry_encode_ascii:
#endif
	{
		output_encoding = 0;
	}

	ct = get_translation_table(utf8_table,output_encoding);

	if (!gd)internal("x_set_window_title called with NULL graphics_device pointer.\n");
	t = convert_string(ct, title, (int)strlen(cast_const_char title), NULL);
	clr_white(t);
	/*XStoreName(x_display,get_window_info(gd)->window,"blabla");*/

#if defined(HAVE_XSUPPORTSLOCALE) && defined(HAVE_XMBTEXTLISTTOTEXTPROPERTY)
	if (XSupportsLocale()) {
		ret = XmbTextListToTextProperty(x_display, (char**)(void *)(&t), 1, XStdICCTextStyle, &windowName);
#ifdef X_HAVE_UTF8_STRING
		if (ret > 0) {
			XFree(windowName.value);
			ret = XmbTextListToTextProperty(x_display, (char**)(void *)(&t), 1, XUTF8StringStyle, &windowName);
			if (ret < 0) {
				ret = XmbTextListToTextProperty(x_display, (char**)(void *)(&t), 1, XStdICCTextStyle, &windowName);
			}
		}
#endif
		if (ret < 0) {
			if (output_encoding) {
				mem_free(t);
				goto retry_encode_ascii;
			} else {
				goto retry_print_ascii;
			}
		}
	} else
retry_print_ascii:
#endif
	{
		ret = XStringListToTextProperty((char**)(void *)(&t), 1, &windowName);
		if (!ret) {
			mem_free(t);
			return;
		}
	}
	mem_free(t);
	XSetWMName(x_display, get_window_info(gd)->window, &windowName);
	XSetWMIconName(x_display, get_window_info(gd)->window, &windowName);
	XFree(windowName.value);
	X_FLUSH();
}

/* gets string in UTF8 */
static void x_set_clipboard_text(struct graphics_device *gd, unsigned char * text)
{
	x_clear_clipboard();
	if (text) {
		x_my_clipboard = stracpy(text);

		XSetSelectionOwner (x_display, XA_PRIMARY, get_window_info(gd)->window, CurrentTime);
		XFlush (x_display);
		X_SCHEDULE_PROCESS_EVENTS();
	}
}

static void selection_request(XEvent *event)
{
	XSelectionRequestEvent *req;
	XSelectionEvent sel;
	size_t l;

	req = &(event->xselectionrequest);
	sel.type = SelectionNotify;
	sel.requestor = req->requestor;
	sel.selection = XA_PRIMARY;
	sel.target = req->target;
	sel.property = req->property;
	sel.time = req->time;
	sel.display = req->display;
#ifdef X_DEBUG
	{
	unsigned char txt[256];
	sprintf (txt,"xselectionrequest from %i\n",(int)event.xselection.requestor);
	MESSAGE(txt);
	sprintf (txt,"property:%i target:%i selection:%i\n", req->property,req->target, req->selection);
	MESSAGE(txt);
	}
#endif
	if (req->target == XA_STRING) {
		unsigned char *str, *p;
		struct conv_table *ct = NULL;
		int iso1 = get_cp_index(cast_uchar "iso-8859-1");
		if (iso1 >= 0) ct = get_translation_table(utf8_table, iso1);
		if (!x_my_clipboard) str = stracpy(cast_uchar "");
		else if (!ct) str = stracpy(x_my_clipboard);
		else str = convert_string(ct, x_my_clipboard, (int)strlen(cast_const_char x_my_clipboard), NULL);
		for (p = cast_uchar strchr(cast_const_char str, 1); p; p = cast_uchar strchr(cast_const_char(str + 1), 1)) *p = 0xa0;
		l = strlen(cast_const_char str);
		if (l > X_MAX_CLIPBOARD_SIZE) l = X_MAX_CLIPBOARD_SIZE;
		XChangeProperty (x_display,
				 sel.requestor,
				 sel.property,
				 XA_STRING,
				 8,
				 PropModeReplace,
				 str,
				 (int)l
		);
		mem_free(str);
	} else if (req->target == x_utf8_string_atom) {
		l = x_my_clipboard ? strlen(cast_const_char x_my_clipboard) : 0;
		if (l > X_MAX_CLIPBOARD_SIZE) l = X_MAX_CLIPBOARD_SIZE;
		XChangeProperty (x_display,
				 sel.requestor,
				 sel.property,
				 x_utf8_string_atom,
				 8,
				 PropModeReplace,
				 x_my_clipboard,
				 (int)l
		);
	} else if (req->target == x_targets_atom) {
		unsigned tgt_atoms[3];
		tgt_atoms[0] = (unsigned)x_targets_atom;
		tgt_atoms[1] = XA_STRING;
		tgt_atoms[2] = (unsigned)x_utf8_string_atom;
		XChangeProperty (x_display,
				 sel.requestor,
				 sel.property,
				 XA_ATOM,
				 32,
				 PropModeReplace,
				 (unsigned char*)&tgt_atoms,
				 3
		);
	} else {
#ifdef X_DEBUG
		{
		    unsigned char txt[256];
		    sprintf (txt,"Non-String wanted: %i\n",(int)req->target);
		    MESSAGE(txt);
		}
#endif
		sel.property = None;
	}
	XSendEvent(x_display, sel.requestor, 0, 0, (XEvent*)&sel);
	XFlush(x_display);
	X_SCHEDULE_PROCESS_EVENTS();
}

static unsigned char *x_get_clipboard_text(void)
{
	XEvent event;
	Atom type_atom = x_utf8_string_atom;

	retry:
	XConvertSelection(x_display, XA_PRIMARY, type_atom, x_sel_atom, fake_window, CurrentTime);

	while (1) {
		XSync(x_display, False);
		if (XCheckTypedEvent(x_display,SelectionRequest, &event)) {
			selection_request(&event);
			continue;
		}
		if (XCheckTypedEvent(x_display,SelectionNotify, &event)) break;
		x_wait_for_event();
	}
	if (event.xselection.property) {
		unsigned char *buffer;
		unsigned long pty_size, pty_items;
		int pty_format, ret;
		Atom pty_type;

		if (event.xselection.target != type_atom) goto no_new_sel;
		if (event.xselection.property != x_sel_atom) goto no_new_sel;


		/* Get size and type of property */
		ret = XGetWindowProperty(
			x_display,
			fake_window,
			event.xselection.property,
			0,
			0,
			False,
			AnyPropertyType,
			&pty_type,
			&pty_format,
			&pty_items,
			&pty_size,
			&buffer);
		if (ret != Success) goto no_new_sel;
		XFree(buffer);

		ret = XGetWindowProperty(
			x_display,
			fake_window,
			event.xselection.property,
			0,
			(long)pty_size,
			True,
			AnyPropertyType,
			&pty_type,
			&pty_format,
			&pty_items,
			&pty_size,
			&buffer
		);
		if (ret != Success) goto no_new_sel;

		pty_size = (pty_format>>3) * pty_items;

		x_clear_clipboard();
		if (type_atom == x_utf8_string_atom) {
			x_my_clipboard = stracpy(buffer);
		} else {
			struct conv_table *ct = NULL;
			int iso1 = get_cp_index(cast_uchar "iso-8859-1");
			if (iso1 >= 0) ct = get_translation_table(iso1, utf8_table);
			if (!ct) {
				x_my_clipboard = stracpy(buffer);
			} else {
				x_my_clipboard = convert_string(ct, buffer, (int)strlen(cast_const_char buffer), NULL);
			}
		}
		XFree(buffer);
	} else {
		if (type_atom == x_utf8_string_atom) {
			type_atom = XA_STRING;
			goto retry;
		}
	}

no_new_sel:
	X_SCHEDULE_PROCESS_EVENTS();
	if (!x_my_clipboard) return NULL;
	return stracpy(x_my_clipboard);
}

/* This is executed in a helper thread, so we must not use mem_alloc */

static void addchr(unsigned char **str, size_t *l, unsigned char c)
{
	unsigned char *s;
	if (!*str) return;
	if ((*str)[*l]) *l = strlen(cast_const_char *str);
	if (*l > MAXINT - 2) overalloc();
	s = realloc(*str, *l + 2);
	if (!s) {
		free(*str);
		*str = NULL;
		return;
	}
	*str = s;
	s[(*l)++] = c;
	s[*l] = 0;
}

static int x_exec(unsigned char *command, int fg)
{
	unsigned char *pattern, *final;
	size_t i, j, l;
	int retval;

	if (!fg) {
		retval = system(cast_const_char command);
		return retval;
	}

	l = 0;
	if (*x_driver.shell) {
		pattern = cast_uchar strdup(cast_const_char x_driver.shell);
	} else {
		pattern = cast_uchar strdup(cast_const_char links_xterm());
		if (*command) {
			addchr(&pattern, &l, ' ');
			addchr(&pattern, &l, '%');
		}
	}
	if (!pattern) return -1;

	final = cast_uchar strdup("");
	l = 0;
	for (i = 0; pattern[i]; i++) {
		if (pattern[i] == '%') {
			for (j = 0; j < strlen(cast_const_char command); j++)
				addchr(&final, &l, command[j]);
		} else {
			addchr(&final, &l, pattern[i]);
		}
	}
	free(pattern);
	if (!final) return -1;

	retval = system(cast_const_char final);
	free(final);
	return retval;
}

struct graphics_driver x_driver={
	cast_uchar "x",
	x_init_driver,
	x_init_device,
	x_shutdown_device,
	x_shutdown_driver,
	NULL,
	NULL,
	x_get_driver_param,
	x_get_af_unix_name,
	NULL,
	NULL,
	x_get_empty_bitmap,
	/*x_get_filled_bitmap,*/
	x_register_bitmap,
	x_prepare_strip,
	x_commit_strip,
	x_unregister_bitmap,
	x_draw_bitmap,
	/*x_draw_bitmaps,*/
	x_get_color,
	x_fill_area,
	x_draw_hline,
	x_draw_vline,
	x_hscroll,
	x_vscroll,
	x_set_clip_area,
	dummy_block,
	dummy_unblock,
	x_set_window_title,
	x_exec,
	x_set_clipboard_text,
	x_get_clipboard_text,
	0,				/* depth (filled in x_init_driver function) */
	0, 0,				/* size (in X is empty) */
	0,				/* flags */
	0,				/* codepage */
	NULL,				/* shell */
};

#endif /* GRDRV_X */
