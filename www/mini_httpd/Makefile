# mini_httpd Makefile
V= 1.23
PORT= mini_httpd
CLEANFILES+= mime_encodings.h *.o mini_httpd htpasswd *.0

CFLAGS+= -DHAVE_DAEMON -DHAVE_HSTRERROR -DHAVE_RAND -DHAVE_SCANDIR -DHAVE_SETLOGIN -DHAVE_TM_GMTOFF -DHAVE_WAITPID -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s

all:		mini_httpd htpasswd

mini_httpd:	mini_httpd.o match.o tdate_parse.o
	$(CC) $(LDFLAGS) mini_httpd.o match.o tdate_parse.o -o mini_httpd -lc -lgcc

mini_httpd.o:	mini_httpd.c version.h port.h match.h tdate_parse.h mime_encodings.h mime_types.h
	$(CC) $(CFLAGS) -c mini_httpd.c

match.o:	match.c match.h
	$(CC) $(CFLAGS) -c match.c

tdate_parse.o:	tdate_parse.c tdate_parse.h
	$(CC) $(CFLAGS) -c tdate_parse.c

mime_encodings.h: mime_encodings.txt
	rm -f mime_encodings.h
	sed < mime_encodings.txt > mime_encodings.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

mime_types.h: mime_types.txt
	rm -f mime_types.h
	sed < mime_types.txt > mime_types.h \
	  -e 's/#.*//' -e 's/[ 	]*$$//' -e '/^$$/d' \
	  -e 's/[ 	][ 	]*/", 0, "/' -e 's/^/{ "/' -e 's/$$/", 0 },/'

htpasswd:	htpasswd.o
	$(CC) $(LDFLAGS) htpasswd.o -o htpasswd -lc -lgcc
	nroff -man mini_httpd.8 > mini_httpd.0
	nroff -man htpasswd.1 > htpasswd.0

htpasswd.o:	htpasswd.c
	$(CC) $(CFLAGS) -c htpasswd.c

beforeinstall: all
	install -d ${DESTDIR}${BINDIR}
	install -d ${DESTDIR}${MANDIR}1
	install -d ${DESTDIR}${MANDIR}8
	install -d ${DESTDIR}${PREFIX}/var/www
	install -c -m 555 mini_httpd ${DESTDIR}${BINDIR}
	install -c -m 555 htpasswd ${DESTDIR}${BINDIR}
	install -c -m 444 mini_httpd.0 ${DESTDIR}${MANDIR}8
	install -c -m 444 htpasswd.0 ${DESTDIR}${MANDIR}1
	install -c -m 444 index.html ${DESTDIR}${PREFIX}/var/www

.include <bsd.port.mk>
