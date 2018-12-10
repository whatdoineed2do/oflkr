ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CXXFLAGS = $(IMAGEMAGICK_CFLAGS) $(EXIV_CFLAGS) $(RESTBED_CFLAGS) $(LOG4CPP_CFLAGS) -Wno-conversion-null -Wno-deprecated -g3
AM_LDFLAGS  = $(IMAGEMAGICK_LIBS) $(EXIV_LIBS) $(RESTBED_LIBS) $(LOG4CPP_LIBS) $(LIBCONFIG_LIBS)

bin_PROGRAMS = oflkr test_meta

oflkr_CPPFLAGS =
oflkr_CXXFLAGS = $(AM_CXXFLAGS)
oflkr_LDFLAGS = -Wl,-rpath=/usr/local/lib64 $(AM_LDFLAGS)
oflkr_LDADD =
oflkr_SOURCES = \
	Document.cc \
	Ingest.cc \
	DataLayer.cc \
	MetaParser.cc \
	RESTsvr.cc \
	oflkr.cc

test_meta_CPPFLAGS =
test_meta_CXXFLAGS = $(AM_CXXFLAGS)
test_meta_LDFLAGS = -Wl,-rpath=/usr/local/lib64 $(AM_LDFLAGS)
test_meta_LDADD =
test_meta_SOURCES = \
	Document.cc \
	MetaParser.cc \
	test_meta.cc

