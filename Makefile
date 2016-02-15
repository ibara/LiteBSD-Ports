#
# The master ports Makefile.
#

# Hook up all directories
SUBDIR=	benchmarks comms devel editors games \
	lang math misc net shells sysutils www

.include <bsd.subdir.mk>
