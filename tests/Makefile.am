AM_LDFLAGS = -L/usr/local/include -lcunit
AM_CFLAGS = -Wall -g -I/usr/local/include -I../src -O0 

check_PROGRAMS = dirutils_test
dirutils_test_CFLAGS = $(AM_CFLAGS) -DTEST_SUITE="\"dirutils_test\""
dirutils_test_SOURCES = dirutils_test.c ../src/dirutils.c

