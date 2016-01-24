#include "cfg.h"

#ifdef GRDRV_GRX

#include "links.h"

#include <grx20.h>

#include "arrow.inc"

#define NUMBER_OF_DEVICES	12

void _GrCloseVideoDriver(void);

extern struct graphics_driver grx_driver;

static struct itrm *svgalib_kbd;

static unsigned char *grx_driver_param = NULL;
static struct graphics_device *grx_old_vd;
static unsigned char in_graphics_mode = 0;

#define TEST_INACTIVITY		if (dev != current_virtual_device || !dev->clip.x2) return;
#define TEST_INACTIVITY_0	if (dev != current_virtual_device || !dev->clip.x2) return 0;

static void grx_set_palette(void)
{
	int i;
	if (grx_driver.depth == 33) {
		for (i = 0; i < 16; i++) {
			GrSetColor(i,
				i & 8 ? 255 : 0,
				((i >> 1) & 3) * 85,
				i & 1 ? 255 : 0);
		}
	}
	if (grx_driver.depth == 65) {
		for (i = 0; i < 256; i++) {
			GrSetColor(i,
				((i >> 5) & 7) * (255./7.),
				((i >> 2) & 7) * (255./7.),
				(i & 3) * 85);
		}
	}
}

static void grx_set_text(void)
{
	if (in_graphics_mode) {
		GrSetMode(GR_default_text);
		in_graphics_mode = 0;
		_GrCloseVideoDriver();
#ifdef DOS
		dos_restore_screen();
#endif
	}
}

static void grx_restore(void)
{
	if (grx_driver_param) mem_free(grx_driver_param), grx_driver_param = NULL;
	grx_set_text();
}

static void grx_set_clip(void)
{
	GrSetClipBox(
		current_virtual_device->clip.x1,
		current_virtual_device->clip.y1,
		current_virtual_device->clip.x2 - 1,
		current_virtual_device->clip.y2 - 1
		);
}

static void grx_key_in(void *p, unsigned char *ev_, int size)
{
	int vd;
	struct links_event *ev = (struct links_event *)(void *)ev_;
	if (size != sizeof(struct links_event)) return;
	if (ev->ev == EV_ABORT) terminate_loop = 1;
	if (ev->ev != EV_KBD) return;
	if ((ev->y & KBD_ALT) && ev->x >= '0' && ev->x <= '9') {
		vd = (ev->x - '1' + 10) % 10;
		goto switch_vd;
	}
	if ((ev->y & KBD_ALT) && ev->x == '-') {
		vd = 10;
		goto switch_vd;
	}
	if ((ev->y & KBD_ALT) && ev->x == '=') {
		vd = 11;
		goto switch_vd;
	}
	if ((ev->y & KBD_ALT) && ev->x <= KBD_F1 && ev->x >= KBD_F12) {
		vd = KBD_F1 - ev->x;
		goto switch_vd;
	}
	if (g_kbd_codepage(&grx_driver) != utf8_table && ev->x >= 128 && ev->x <= 255)
		if ((ev->x = cp2u(ev->x, g_kbd_codepage(&grx_driver))) == -1) return;
	if (current_virtual_device && current_virtual_device->keyboard_handler) current_virtual_device->keyboard_handler(current_virtual_device, ev->x, ev->y);
	return;

switch_vd:
	switch_virtual_device(vd);
	grx_set_clip();
}

int grx_mouse_initialized = 0;
static GrCursor *grx_mouse_cursor;

static void grx_mouse_init(void)
{
	int x, y;
	static unsigned char cursor[arrow_area];
	static GrColor table[3];

	if (!GrMouseDetect()) return;
	grx_mouse_initialized = 1;
	GrMouseInit();
	GrMouseEventEnable(0, 1);

	memset(&table, 0, sizeof table);
	table[0] = 2;
	table[1] = grx_driver.get_color(0);
	table[2] = grx_driver.get_color(0xffffff);
	for (y = 0; y < arrow_height; y++)
		for (x = 0; x < arrow_width; x++)
			cursor[y * arrow_width + x] =
			   (arrow[2 * y] >> (arrow_width - x - 1)) & 1 ? 1 :
			   (arrow[2 * y + 1] >> (arrow_width - x - 1)) & 1 ? 2 :
			   0;

	grx_mouse_cursor = GrBuildCursor(cast_char cursor, arrow_width, arrow_width, arrow_height, 0, 0, table);

	if (!grx_mouse_cursor) GrMouseSetColors(GrWhite(), GrBlack());
	else GrMouseSetCursor(grx_mouse_cursor);

	GrMouseDisplayCursor();
}

static void grx_mouse_terminate(void)
{
	if (!grx_mouse_initialized) return;
	GrMouseUnInit();
	if (grx_mouse_cursor) GrDestroyCursor(grx_mouse_cursor);
	grx_mouse_initialized = 0;
}

void grx_mouse_poll(void)
{
	int x = GrMousePendingEvent();
	x = x;
}

static void grx_do_event(GrMouseEvent *ev, int b)
{
	if (!current_virtual_device) return;
	if (current_virtual_device->mouse_handler) current_virtual_device->mouse_handler(current_virtual_device, ev->x, ev->y, b);
}

int grx_mouse_event(void)
{
	int e;
	GrMouseEvent ev;
	e = 0;
get_another:
	GrMouseGetEvent(GR_M_MOTION | GR_M_BUTTON_CHANGE | GR_M_POLL, &ev);
	if (ev.flags & GR_M_LEFT_DOWN) grx_do_event(&ev, B_LEFT | B_DOWN), e = 1;
	if (ev.flags & GR_M_LEFT_UP) grx_do_event(&ev, B_LEFT | B_UP), e = 1;
	if (ev.flags & GR_M_RIGHT_DOWN) grx_do_event(&ev, B_RIGHT | B_DOWN), e = 1;
	if (ev.flags & GR_M_RIGHT_UP) grx_do_event(&ev, B_RIGHT | B_UP), e = 1;
	if (ev.flags & GR_M_MIDDLE_DOWN) grx_do_event(&ev, B_MIDDLE | B_DOWN), e = 1;
	if (ev.flags & GR_M_MIDDLE_UP) grx_do_event(&ev, B_MIDDLE | B_UP), e = 1;
	if (ev.flags & GR_M_P4_DOWN) grx_do_event(&ev, B_WHEELUP | B_MOVE), e = 1;
	if (ev.flags & GR_M_P5_DOWN) grx_do_event(&ev, B_WHEELDOWN | B_MOVE), e = 1;
	if (!e && ev.flags & GR_M_MOTION) {
		if (GrMousePendingEvent())
			goto get_another;
		grx_do_event(&ev,
			ev.buttons & GR_M_LEFT ? B_LEFT | B_DRAG :
			ev.buttons & GR_M_RIGHT ? B_RIGHT | B_DRAG :
			ev.buttons & GR_M_MIDDLE ? B_DRAG | B_MIDDLE : B_MOVE), e = 1;
	}
	return e;
}

static unsigned char *set_mode_according_to_param(unsigned char *param)
{
	unsigned char e[MAX_STR_LEN];
#ifdef DOS
	dos_save_screen();
#endif
	if (!param || !*param) {
		return stracpy(cast_uchar "There is no default videomode.\nUse the switch \"-mode WIDTHxHEIGHTxCOLORS\" to select video mode.\nThe COLORS argument may be 16, 256, 32k, 64k, 16M.\n");
		if (!GrSetMode(GR_default_graphics))
			return stracpy(cast_uchar "GrSetMode failed.\n");
		in_graphics_mode = 1;
	} else {
		unsigned long x, y;
		int pl;
		unsigned char *ptr = param, *end;
		x = strtoul(cast_const_char ptr, (char **)(void *)&end, 10);
		if (end == ptr || *end != 'x' || x >= MAXINT)
			goto bad_param;
		ptr = end + 1;
		y = strtoul(cast_const_char ptr, (char **)(void *)&end, 10);
		if (end == ptr || *end != 'x' || y >= MAXINT)
			goto bad_param;
		ptr = end + 1;
		if (!strcmp(cast_const_char ptr, "16")) pl = 4;
		else if (!strcmp(cast_const_char ptr, "256")) pl = 8;
		else if (!strcasecmp(cast_const_char ptr, "32k") || !strcmp(cast_const_char ptr, "32768")) pl = 15;
		else if (!strcasecmp(cast_const_char ptr, "64k") || !strcmp(cast_const_char ptr, "65536")) pl = 16;
		else if (!strcasecmp(cast_const_char ptr, "16M") || !strcasecmp(cast_const_char ptr, "16M32") || !strcmp(cast_const_char ptr, "16777216")) pl = 24;
		else goto bad_param;
		if (x == 800 && y == 600 && pl == 4) y = 599; /* a bug in GRX */
		if (!GrSetMode(GR_width_height_bpp_graphics, (int)x, (int)y, pl)) {
			snprintf(cast_char e, MAX_STR_LEN, "GrSetMode(%lu,%lu,%d) failed.\n", x, y, pl);
			return stracpy(e);
		}
		in_graphics_mode = 1;
	}
	return NULL;

bad_param:
	return stracpy(cast_uchar "Bad -mode parameter.\n");
}

static void grx_set_graphics(void)
{
	if (!in_graphics_mode) {
		unsigned char *er;
		er = set_mode_according_to_param(grx_driver_param);
		if (er)
			fatal_exit("grx: Unable to restore video mode '%s': %s", grx_driver_param, er);
	}
}

static unsigned char *grx_init_driver(unsigned char *param, unsigned char *ignore)
{
	unsigned char e[MAX_STR_LEN];
	unsigned char *er;
	const GrVideoMode *mode;
	const GrFrameDriver *fd;

	kbd_set_raw = 1;

	grx_old_vd = NULL;

	er = set_mode_according_to_param(param);
	if (er)
		return er;

	mode = GrCurrentVideoMode();
	fd = GrScreenFrameDriver();
	if (GrNumPlanes() != (mode->bpp == 4 ? 4 : 1)) {
unsupported_videomode:
		snprintf(cast_char e, MAX_STR_LEN, "Unsupported videomode: x %d, y %d, bpp (%d, %d), planes %d.\n", mode->width, mode->height, mode->bpp, fd->bits_per_pixel, (int)GrNumPlanes());
		grx_restore();
		return stracpy(e);
	}
	switch (mode->bpp) {
		case 4:
			grx_driver.depth = 33;
			break;
		case 8:
			grx_driver.depth = 65;
			break;
		case 15:
			grx_driver.depth = 122;
			break;
		case 16:
			grx_driver.depth = 130;
			break;
		case 24:
		case 32:
			if (fd->bits_per_pixel == 24)
				grx_driver.depth = 195;
			else if (fd->bits_per_pixel == 32)
				grx_driver.depth = 196;
			else
				goto unsupported_videomode;
			break;
		default:
			goto unsupported_videomode;
	}
	grx_driver.get_color = get_color_fn(grx_driver.depth);
	grx_driver.x = mode->width;
	grx_driver.y = mode->height;
	if (mode->bpp >= 8) grx_driver.flags |= GD_DONT_USE_SCROLL;
	else grx_driver.flags &= ~GD_DONT_USE_SCROLL;
	/*fatal_exit("current mode: %d,%d,%d planes %d bits %d %d", mode->width, mode->height, mode->bpp, GrNumPlanes(), GrScreenFrameDriver()->bits_per_pixel, mode->bpp);*/

	grx_set_palette();

	grx_driver_param = stracpy(param ? param : cast_uchar "");

	if (init_virtual_devices(&grx_driver, NUMBER_OF_DEVICES)) {
		grx_restore();
		return stracpy(cast_uchar "Allocation of virtual devices failed.\n");
	}

	svgalib_kbd = handle_svgalib_keyboard(grx_key_in);
	grx_mouse_init();
	return NULL;
}

static void grx_shutdown_driver(void)
{
	shutdown_virtual_devices();
	grx_mouse_terminate();
	svgalib_free_trm(svgalib_kbd);
	grx_restore();
}

static void grx_emergency_shutdown(void)
{
	grx_restore();
}

static unsigned char *grx_get_driver_param(void)
{
	return grx_driver_param;
}

struct grx_bmp {
	GrContext c;
};

static int grx_get_empty_bitmap(struct bitmap *bmp)
{
	struct grx_bmp *b;

	if (!in_graphics_mode)
		internal("grx_get_empty_bitmap: not in graphics mode");

	b = mem_alloc_mayfail(sizeof(struct grx_bmp));
	if (!b) goto fail0;
again1:
	if (!GrCreateContext(bmp->x, bmp->y, NULL, &b->c)) {
		if (out_of_memory(0, NULL, 0))
			goto again1;
		goto fail1;
	}
	bmp->flags = b;
	if (GrNumPlanes() == 1) {
		bmp->data = b->c.gc_baseaddr[0];
		bmp->skip = b->c.gc_lineoffset;
	} else {
		if (bmp->x && (unsigned)bmp->x * (unsigned)bmp->y / (unsigned)bmp->x != (unsigned)bmp->y) goto fail2;
		if ((unsigned)bmp->x * (unsigned)bmp->y > (unsigned)MAXINT) goto fail2;
		bmp->skip = bmp->x;
		bmp->data = mem_alloc_mayfail(bmp->skip * bmp->y);
		if (!bmp->data) goto fail2;
	}
	return 0;

fail2:
	GrDestroyContext(&b->c);
fail1:
	mem_free(b);
fail0:
	bmp->data = NULL;
	bmp->flags = NULL;
	return -1;
}

static void grx_copy_planes(struct bitmap *bmp, int y, int n)
{
	struct grx_bmp *b = bmp->flags;
	int j;
	if (!b) return;
	for (j = 0; j < n; y++, j++) {
		int i;
		for (i = 0; i < GrNumPlanes(); i++) {
			int x;
			unsigned char *src = (unsigned char *)bmp->data + bmp->skip * j;
			unsigned char *dst = (unsigned char *)b->c.gc_baseaddr[i] + y * b->c.gc_lineoffset;
			unsigned char byt = 0;
			for (x = 0; x < bmp->x; ) {
				unsigned char bit = (*src++ >> i) & 1;
				byt |= bit << ((x & 7) ^ 7);
				x++;
				if (!(x & 7)) *dst++ = byt, byt = 0;
			}
			if (x & 7) *dst = byt;
		}
	}
}

static void grx_register_bitmap(struct bitmap *bmp)
{
	if (GrNumPlanes() != 1) {
		grx_copy_planes(bmp, 0, bmp->y);
		mem_free(bmp->data);
	}
}

static void *grx_prepare_strip(struct bitmap *bmp, int top, int lines)
{
	struct grx_bmp *b = bmp->flags;
	if (!b) return NULL;
	if (GrNumPlanes() != 1) {
		bmp->data = mem_alloc_mayfail(bmp->skip * lines);
		return bmp->data;
	}
	return ((unsigned char *)bmp->data) + bmp->skip * top;
}


static void grx_commit_strip(struct bitmap *bmp, int top, int lines)
{
	if (GrNumPlanes() != 1) {
		if (!bmp->data) return;
		grx_copy_planes(bmp, top, lines);
		mem_free(bmp->data);
	}
}

static void grx_unregister_bitmap(struct bitmap *bmp)
{
	struct grx_bmp *b = bmp->flags;
	if (!b) return;
	GrDestroyContext(&b->c);
	mem_free(b);
}

static void grx_draw_bitmap(struct graphics_device *dev, struct bitmap *bmp, int x, int y)
{
	struct grx_bmp *b = bmp->flags;
	if (!b) return;
	TEST_INACTIVITY
	/* note: GrImageDisplay has buggy clipping.
	   It doesn't draw if the target x or y coordinate is one pixel
	   before the end of clip area */
	GrBitBlt(NULL, x, y, &b->c, 0, 0, bmp->x - 1, bmp->y - 1, GrWRITE);
}

static void grx_fill_area(struct graphics_device *dev, int x1, int y1, int x2, int y2, long color)
{
	TEST_INACTIVITY
	if (x1 >= x2 || y1 >= y2) return;
	GrFilledBox(x1, y1, x2 - 1, y2 - 1, color);
}

static void grx_draw_hline(struct graphics_device *dev, int x1, int y, int x2, long color)
{
	TEST_INACTIVITY
	if (x1 >= x2) return;
	GrHLine(x1, x2 - 1, y, color);
}

static void grx_draw_vline(struct graphics_device *dev, int x, int y1, int y2, long color)
{
	TEST_INACTIVITY
	if (y1 >= y2) return;
	GrVLine(x, y1, y2 - 1, color);
}

static int grx_hscroll(struct graphics_device *dev, struct rect_set **ignore, int sc)
{
	TEST_INACTIVITY_0
	if (!sc) return 0;
	if (sc < 0) {
		if (-sc >= dev->clip.x2 - dev->clip.x1) return 1;
		GrBitBlt(NULL, dev->clip.x1, dev->clip.y1,
			NULL, dev->clip.x1 - sc, dev->clip.y1,
			dev->clip.x2 - 1, dev->clip.y2 - 1, GrWRITE);
	} else {
		if (sc >= dev->clip.x2 - dev->clip.x1) return 1;
		GrBitBlt(NULL, dev->clip.x1 + sc, dev->clip.y1,
			NULL, dev->clip.x1, dev->clip.y1,
			dev->clip.x2 - sc - 1, dev->clip.y2 - 1, GrWRITE);
	}
	return 1;
}

static int grx_vscroll(struct graphics_device *dev, struct rect_set **ignore, int sc)
{
	TEST_INACTIVITY_0
	if (!sc) return 0;
	if (sc < 0) {
		if (-sc >= dev->clip.y2 - dev->clip.y1) return 1;
		GrBitBlt(NULL, dev->clip.x1, dev->clip.y1,
			NULL, dev->clip.x1, dev->clip.y1 - sc,
			dev->clip.x2 - 1, dev->clip.y2 - 1, GrWRITE);
	} else {
		if (sc >= dev->clip.y2 - dev->clip.y1) return 1;
		GrBitBlt(NULL, dev->clip.x1, dev->clip.y1 + sc,
			NULL, dev->clip.x1, dev->clip.y1,
			dev->clip.x2 - 1, dev->clip.y2 - sc - 1, GrWRITE);
	}
	return 1;
}

static void grx_set_clip_area(struct graphics_device *dev, struct rect *r)
{
	generic_set_clip_area(dev, r);
	if (dev != current_virtual_device) return;
	grx_set_clip();
}

static int grx_block(struct graphics_device *dev)
{
	if (grx_old_vd) return 1;
	grx_old_vd = current_virtual_device;
	current_virtual_device = NULL;
	grx_mouse_terminate();
	svgalib_block_itrm(svgalib_kbd);
	grx_set_text();
	return 0;
}

static int grx_unblock(struct graphics_device *dev)
{
	if (current_virtual_device) {
		internal("grx_unblock called without grx_block");
		return 0;
	}
	grx_set_graphics();
	svgalib_unblock_itrm(svgalib_kbd);
	current_virtual_device = grx_old_vd;
	grx_old_vd = NULL;
	grx_set_palette();
	grx_set_clip();
	grx_mouse_init();
	if (current_virtual_device) current_virtual_device->redraw_handler(current_virtual_device, &current_virtual_device->size);
	return 0;
}

struct graphics_driver grx_driver = {
	cast_uchar "grx",
	grx_init_driver,
	init_virtual_device,
	shutdown_virtual_device,
	grx_shutdown_driver,
	grx_emergency_shutdown,
	NULL,
	grx_get_driver_param,
	NULL,
	NULL,
	NULL,
	grx_get_empty_bitmap,
	/*grx_get_filled_bitmap,*/
	grx_register_bitmap,
	grx_prepare_strip,
	grx_commit_strip,
	grx_unregister_bitmap,
	grx_draw_bitmap,
	/*grx_draw_bitmaps,*/
	NULL,
	grx_fill_area,
	grx_draw_hline,
	grx_draw_vline,
	grx_hscroll,
	grx_vscroll,
	grx_set_clip_area,
	grx_block,
	grx_unblock,
	NULL, /* set_title */
	NULL, /* exec */
	NULL, /* set_clipboard_text */
	NULL, /* get_clipboard_text */
	0,				/* depth */
	0, 0,				/* size */
	GD_NEED_CODEPAGE,		/* flags */
	0,				/* codepage */
	NULL,				/* shell */
};

#endif
