## Process this file with automake to produce Makefile.in

INCLUDES = \
	$(DEVELOPMENT_CFLAGS) \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = imagination

imagination_SOURCES = \
	imagination.c \
	support.c support.h \
	main-window.c main-window.h \
	callbacks.c callbacks.h

imagination_CFLAGS = \
	-DDATADIR=\"$(datadir)\"

imagination_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

