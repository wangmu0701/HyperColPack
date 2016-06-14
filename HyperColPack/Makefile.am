AUTOMAKE_OPTIONS = foreign

SUBDIRS = include src . test

lib_LTLIBRARIES = libhypercolpack.la

libhypercolpack_la_SOURCES = dummy.cpp
libhypercolpack_la_LIBADD = src/libhypercolpacksrc.la

test:
	cd test; $(MAKE) test

.PHONY: test
