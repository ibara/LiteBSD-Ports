EXTRA_DIST = firetalk.m4

noinst_LIBRARIES = libfiretalk.a
libfiretalk_a_SOURCES = \
	aim.c \
	aim.h \
	firetalk.c \
	firetalk-int.h \
	irc.c \
	lily.c \
	toc2.c \
	toc2_uuids.h

check_PROGRAMS = tester
tester_SOURCES = \
	tester.c
tester_LDADD = \
	libfiretalk.a

TESTS = ${check_PROGRAMS}

include_HEADERS = firetalk.h

.c.o: $<
	@echo $(COMPILE) -c `test -f '$<' || echo '$(srcdir)/'`$<
	@$(COMPILE) -c `test -f '$<' || echo '$(srcdir)/'`$<
