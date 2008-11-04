SUBDIRS = include src tests

EXTRA_DIST =			\
	libxdg-basedir.pc.in	\
	doxygen.cfg			\
	autogen.sh

include_HEADERS = include/basedir.h include/basedir_fs.h

include $(top_srcdir)/aminclude.am

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=libxdg-basedir.pc
