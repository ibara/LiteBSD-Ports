# links Makefile
V= 2.12
PORT= links
PROG= ${PORT}

SRCS= af_unix.c auth.c beos.c bfu.c block.c bookmark.c cache.c charsets.c compress.c connect.c cookies.c data.c default.c dip.c directfb.c dither.c dns.c dos.c drivers.c error.c file.c finger.c fn_impl.c font_inc.c framebuf.c ftp.c gif.c grx.c hpux.c html.c html_gr.c html_r.c html_tbl.c http.c https.c img.c imgcache.c jpeg.c jsint.c kbd.c language.c listedit.c lru.c mailto.c main.c memory.c menu.c objreq.c os_dep.c pmshell.c png.c sched.c select.c session.c smb.c string.c svg.c svgalib.c terminal.c tiff.c types.c url.c view.c view_gr.c vms.c x.c xbm.c

CFLAGS+= -ffunction-sections -fdata-sections -DHAVE_CONFIG_H -I. -I$(srcdir) -I.
LDFLAGS+= -Wl,--gc-sections -s
LDADD+= -lssl -lcrypto -lz 

.include <bsd.port.mk>
