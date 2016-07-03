AM_CPPFLAGS = --std=c++11 -I$(top_builddir)/HyperColPack/include

noinst_PROGRAMS = hyper_graph_test hyper_random

hyper_random_SOURCES = hyper_random.cpp
hyper_random_LDADD = $(top_builddir)/HyperColPack/libhypercolpack.la

hyper_graph_test_SOURCES = hyper_graph_test.cpp
hyper_graph_test_LDADD = $(top_builddir)/HyperColPack/libhypercolpack.la
