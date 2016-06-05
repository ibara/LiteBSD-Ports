#
# The master ports Makefile.
#

# Hook up all directories
SUBDIR=	archivers benchmarks comms devel editors games \
	lang math misc net shells sysutils textproc www

# There are here so we don't have to .include <bsd.port.mk>
packages: package

clean-work: clean cleandir
	find . -type d -name 'work' -exec rm -rf {} +

clean-package clean-packages:
	rm -f packages/*.ar

clean-all: clean-work clean-packages

.include <bsd.subdir.mk>
