# Adapted from Geany's plugins/Makefile.am, thanks

EXTRA_DIST = \
	macros.h

plugindir = $(libdir)/imagination

plugins_includedir = $(includedir)/imagination/
plugins_include_HEADERS = macros.h

fade_la_LDFLAGS  = -module -avoid-version

# Plugins to be installed
plugin_LTLIBRARIES = \
	fade.la 

fade_la_SOURCES  = fade.c

# instead of linking against all in $(GTK_LIBS), we link only against needed libs
fade_la_LIBADD  = $(GTK_LIBS)

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src \
	$(GTK_CFLAGS) \
	$(PLUGIN_CFLAGS)

#
# This part allows people to build their own plugins in here.
# Yes, it's a mess.
#
SUFFIXES = .c .so
.c.so:
	$(LIBTOOL) --mode=compile $(CC) -DHAVE_CONFIG_H -I$(top_srcdir) $(AM_CPPFLAGS) $(CFLAGS) -c $< -o tmp$@.lo $(PLUGIN_CFLAGS)
	$(LIBTOOL) --mode=link    $(CC) $(CFLAGS) -o libtmp$@.la -rpath $(plugindir) tmp$@.lo $(LIBS) $(LDFLAGS) -module -avoid-version $(PLUGIN_LIBS)
	@rm -f tmp$@.lo tmp$@.o libtmp$@.la
	@cp .libs/libtmp$@.so* $@
	@rm -f .libs/libtmp$@.*
