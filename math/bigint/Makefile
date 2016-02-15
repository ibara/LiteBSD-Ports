# bigint Makefile
V= 20140814
PORT= bigint
CLEANFILES+= bi_factor bic bic.c bigint.a *.o *.0

CFLAGS+= -ffunction-sections -fdata-sections
LDFLAGS+= -Wl,--gc-sections -s
LIBS= -lc -lgcc

all:		bi_factor bic


bigint.a:		bigint_ext.o bigint_int.o
	$(AR) rc bigint.a bigint_ext.o bigint_int.o
	$(RANLIB) bigint.a
	nroff -man bigint.3 > bigint.0

bigint_ext.o:		bigint_ext.c bigint.h
	$(CC) $(CFLAGS) -c bigint_ext.c

bigint_int.o:		bigint_int.c bigint.h
	$(CC) $(CFLAGS) -c bigint_int.c


bi_test:		bi_test.o bigint.a
	$(CC) $(LDFLAGS) bi_test.o -o bi_test bigint.a $(LIBS)

bi_test.o:		bi_test.c bigint.h
	$(CC) $(CFLAGS) -c bi_test.c


bi_factor:		bi_factor.o bigint.a
	$(CC) $(LDFLAGS) bi_factor.o -o bi_factor bigint.a $(LIBS)

bi_factor.o:		bi_factor.c low_primes.h bigint.h
	$(CC) $(CFLAGS) -c bi_factor.c


bic.c bic.h:	bic.y
	bison -d -o bic.c bic.y
bic.o:		bic.c bigint.h
	$(CC) $(CFLAGS) -c bic.c

bic_scan.o:		bic_scan.c bigint.h bic.h
	$(CC) $(CFLAGS) -c bic_scan.c

bic:			bic.o bic_scan.o bigint.a
	$(CC) $(LDFLAGS) bic.o bic_scan.o -o bic bigint.a $(LIBS)


birsa:			birsa.o bigint.a
	$(CC) $(LDFLAGS) birsa.o -o birsa bigint.a $(LIBS)

birsa.o:		birsa.c bigint.h
	$(CC) $(CFLAGS) -c birsa.c

beforeinstall:
	install -d $(DESTDIR)$(BINDIR)
	install -d $(DESTDIR)$(PREFIX)/include
	install -d $(DESTDIR)$(PREFIX)/lib
	install -d $(DESTDIR)$(MANDIR)3
	install -c -m 555 bi_factor $(DESTDIR)$(BINDIR)
	install -c -m 555 bic $(DESTDIR)$(BINDIR)
	install -c -m 444 bigint.h $(DESTDIR)$(PREFIX)/include
	install -c -m 444 bigint.a $(DESTDIR)$(PREFIX)/lib
	install -c -m 444 bigint.0 $(DESTDIR)$(MANDIR)3

.include <bsd.port.mk>
