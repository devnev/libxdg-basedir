AM_CFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = testdump testfind testquery

QUERYTESTS = \
	#

TESTS = testdump ${QUERYTESTS}

EXTRA_DIST = query-harness.sh ${QUERYTESTS}

testdump_SOURCES = testdump.c
testdump_LDFLAGS = $(all_libraries)
testdump_LDADD = ../src/libxdg-basedir.la

testfind_SOURCES = testfind.c
testfind_LDFLAGS = $(all_libraries)
testfind_LDADD = ../src/libxdg-basedir.la

testquery_SOURCES = testquery.c
testquery_LDFLAGS = $(all_libraries)
testquery_LDADD = ../src/libxdg-basedir.la
