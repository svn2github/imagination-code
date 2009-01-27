## Process this file with automake to produce Makefile.in

INCLUDES = \
	$(DEVELOPMENT_CFLAGS) \
	-DPACKAGE_LIB_DIR=\""$(libdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = imagination

imagination_SOURCES = \
	sexy-icon-entry.c sexy-icon-entry.h \
	imagination.c imagination.h \
	support.c support.h \
	eog-pixbuf-cell-renderer.c eog-pixbuf-cell-renderer.h \
	main-window.c main-window.h \
	callbacks.c callbacks.h \
	new_slideshow.c new_slideshow.h \
	slideshow_project.c slideshow_project.h

imagination_CFLAGS = \
	-DDATADIR=\"$(datadir)\"

imagination_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)
