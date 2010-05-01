AM_CFLAGS = -I$(top_srcdir)/include
AUTOMAKE_OPTIONS = color-tests

check_PROGRAMS = testdump testfind testquery

QUERYTESTS = \
	querycd.1 \
	querycd.2 \
	querycd.3 \
	querycd.4 \
	querycd.5 \
	querycs.1 \
	querycs.2 \
	querycs.3 \
	querycs.4 \
	querycs.5 \
	querydd.1 \
	querydd.2 \
	querydd.3 \
	querydd.4 \
	querydd.5 \
	querydh.1 \
	querydh.2 \
	querydh.3 \
	queryds.1 \
	queryds.2 \
	queryds.3 \
	queryds.4 \
	queryds.5 \
	queryds.6 \
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
