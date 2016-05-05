noinst_LIBRARIES = libsecs.a
libsecs_a_SOURCES = \
	atomizer.c \
	block.c \
	liaison.c \
	mem.c \
	script.c \
	vars.c

INCLUDES = -I${top_srcdir}/include

.c.o: $<
	@echo $(COMPILE) -c `test -f '$<' || echo '$(srcdir)/'`$<
	@$(COMPILE) -c `test -f '$<' || echo '$(srcdir)/'`$<
