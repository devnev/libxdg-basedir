SUBDIRS = include src tests

EXTRA_DIST =			\
	Doxyfile		\
	autogen.sh

include_HEADERS = include/basedir.h

#DISTCLEANFILES =	\
#	doc		\
#	source-doc

if HAS_DOXYGEN
doc:
	@doxygen Doxyfile > /dev/null && echo "Documentation generated"
endif
