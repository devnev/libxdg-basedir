SUBDIRS = include src

EXTRA_DIST =			\
	xdg.dox			\
	xdg-source.dox		\
	gendock.sh		\
	autogen.sh

#DISTCLEANFILES =	\
#	doc		\
#	source-doc

if HAS_DOXYGEN
doc:
	doxygen xdg.dox > /dev/null && echo "Documentation generated"
	doxygen xdg-source.dox > /dev/null && echo "Source documentatino generated"
endif
