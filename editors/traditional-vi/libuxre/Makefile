CFLAGS = $(COPT) $(RPMCFLAGS) -I.
OBJS = bracket.o _collelem.o _collmult.o regcomp.o regdfa.o regerror.o regexec.o regfree.o regnfa.o regparse.o stubs.o

.c.o: ; $(CC) $(CFLAGS) -c $<

all: libuxre.a

libuxre.a: $(OBJS)
	ar cr libuxre.a $(OBJS)

clean:
	rm -f libuxre.a $(OBJS) core
