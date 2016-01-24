#include "cfg.h"

#ifdef G
#include "links.h"

#ifdef HAVE_SVG

#include <cairo.h>
#include <librsvg/rsvg.h>
#ifdef HAVE_LIBRSVG_RSVG_CAIRO_H
#include <librsvg/rsvg-cairo.h>
#endif
#ifdef HAVE_LIBRSVG_LIBRSVG_FEATURES_H
#include <librsvg/librsvg-features.h>
#endif

#if !defined(LIBRSVG_CHECK_VERSION)
#define LIBRSVG_CHECK_VERSION(a,b,c)	0
#endif

#include "bits.h"

#ifdef __CYGWIN__
#include <fontconfig/fontconfig.h>
#include <windows.h>
#include <w32api/shlobj.h>
static void set_font_path(void)
{
	unsigned char *path = cast_uchar "/cygdrive/c/Windows/Fonts";
#if defined(HAVE_CYGWIN_CONV_PATH)
	unsigned char win32_path[PATH_MAX];
	if (SHGetFolderPathA(NULL, CSIDL_FONTS, NULL, 0, cast_char win32_path) == S_OK) {
		ssize_t l;
		unsigned char *cyg_path;
		l = cygwin_conv_path(CCP_WIN_A_TO_POSIX, win32_path, NULL, 0);
		if (l <= 0)
			goto do_default;
		cyg_path = alloca(l);
		l = cygwin_conv_path(CCP_WIN_A_TO_POSIX, win32_path, cyg_path, l);
		if (l < 0)
			goto do_default;
		path = cyg_path;
	}
	do_default:
#endif
	FcConfigAppFontAddDir(NULL, path);
}
#else
#define set_font_path()	do { } while (0)
#endif

struct svg_decoder {
	RsvgHandle *handle;
};

void svg_start(struct cached_image *cimg)
{
	struct svg_decoder *deco;
	static unsigned char initialized = 0;

	if (!initialized) {
#if !LIBRSVG_CHECK_VERSION(2,36,0)
		rsvg_init();
#elif !defined(GLIB_DEPRECATED_IN_2_36)
		g_type_init();
#endif
		set_font_path();
		initialized = 1;
	}

	deco = mem_alloc(sizeof(struct svg_decoder));

	cimg->decoder = deco;
	deco->handle = rsvg_handle_new();
}

void svg_restart(struct cached_image *cimg, unsigned char *data, int length)
{
	struct svg_decoder *deco = (struct svg_decoder *)cimg->decoder;
	GError *er = NULL;

	if (!rsvg_handle_write(deco->handle, (const guchar *)data, length, &er)) {
		g_error_free(er);
		img_end(cimg);
	}
}

void svg_finish(struct cached_image *cimg)
{
	struct svg_decoder *deco = (struct svg_decoder *)cimg->decoder;
	GError *er = NULL;
	RsvgDimensionData dim;
	cairo_surface_t *surf;
	cairo_t *cairo;
	unsigned char *end_buffer, *p;

	if (!rsvg_handle_close(deco->handle, &er)) {
		g_error_free(er);
		goto end;
	}

	rsvg_handle_get_dimensions(deco->handle, &dim);

	cimg->width = dim.width;
	cimg->height = dim.height;
	cimg->buffer_bytes_per_pixel = 4;
	cimg->red_gamma=cimg->green_gamma=cimg->blue_gamma=(float)sRGB_gamma;
	cimg->strip_optimized=0;
	if (header_dimensions_known(cimg))
		goto end;

	surf = cairo_image_surface_create_for_data(cimg->buffer, CAIRO_FORMAT_ARGB32, cimg->width, cimg->height, cimg->width * cimg->buffer_bytes_per_pixel);
	if (cairo_surface_status(surf))
		goto end_surface;

	cairo = cairo_create(surf);
	if (cairo_status(cairo))
		goto end_cairo;

	if (dim.width && dim.height)
		cairo_scale(cairo, (double)cimg->width / (double)dim.width, (double)cimg->height / (double)dim.height);

	rsvg_handle_render_cairo(deco->handle, cairo);

	end_buffer = cimg->buffer + cimg->width * cimg->height * cimg->buffer_bytes_per_pixel;
	if (htonl(0x12345678L) != 0x12345678L) {
		for (p = cimg->buffer; p < end_buffer; p += 4) {
#ifdef t4c
			t4c t = *(t4c *)p;
			t4c r = (t << 16) | (t >> 16);
			*(t4c *)p = (t & 0xff00ff00U) | (r & 0xff00ffU);
#else
			unsigned char c;
			c = p[0];
			p[0] = p[2];
			p[2] = c;
#endif
		}
	} else {
		for (p = cimg->buffer; p < end_buffer; p += 4) {
#ifdef t4c
			t4c t = *(t4c *)p;
			*(t4c *)p = (t << 8) | (t >> 24);
#else
			unsigned char c;
			c = p[0];
			p[0] = p[1];
			p[1] = p[2];
			p[2] = p[3];
			p[3] = c;
#endif
		}
	}

end_cairo:
	cairo_destroy(cairo);
end_surface:
	cairo_surface_destroy(surf);
end:
	img_end(cimg);
}

void svg_destroy_decoder(struct cached_image *cimg)
{
	struct svg_decoder *deco = (struct svg_decoder *)cimg->decoder;
	g_object_unref(deco->handle);
}

void add_svg_version(unsigned char **s, int *l)
{
	add_to_str(s, l, cast_uchar "RSVG (");
#ifdef LIBRSVG_MAJOR_VERSION
	add_num_to_str(s, l, LIBRSVG_MAJOR_VERSION);
#endif
#ifdef LIBRSVG_MINOR_VERSION
	add_chr_to_str(s, l, '.');
	add_num_to_str(s, l, LIBRSVG_MINOR_VERSION);
#endif
#ifdef LIBRSVG_MICRO_VERSION
	add_chr_to_str(s, l, '.');
	add_num_to_str(s, l, LIBRSVG_MICRO_VERSION);
#endif
	add_to_str(s, l, cast_uchar ")");
}

#endif

#endif
