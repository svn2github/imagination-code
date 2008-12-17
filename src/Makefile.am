## Process this file with automake to produce Makefile.in

EXTRA_DIST = transition_data.h

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
	callbacks.c callbacks.h \
	new_slideshow.c new_slideshow.h

imagination_CFLAGS = \
	-DDATADIR=\"$(datadir)\"

imagination_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

