SUBDIRS = include src tests

EXTRA_DIST =			\
	xdg.dox			\
	autogen.sh

include_HEADERS = include/basedir.h

#DISTCLEANFILES =	\
#	doc		\
#	source-doc

if HAS_DOXYGEN
dox:
	doxygen xdg.dox > /dev/null && echo "Documentation generated"
endif
