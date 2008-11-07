# Adapted from Pidgin's plugins/Makefile.am, thanks

EXTRA_DIST = \
	pluginmacros.h

plugindir = $(libdir)/geany

plugins_includedir = $(includedir)/geany/
plugins_include_HEADERS = pluginmacros.h

demoplugin_la_LDFLAGS    = -module -avoid-version

if PLUGINS

# Plugins to be installed
plugin_LTLIBRARIES = \
#	classbuilder.la \

# Plugins not to be installed
noinst_LTLIBRARIES = \
	demoplugin.la

demoplugin_la_SOURCES    = demoplugin.c

# we don't need to link against GTK libraries (or do we?)
#~ demoplugin_la_LIBADD    = $(GTK_LIBS)
#~ classbuilder_la_LIBADD  = $(GTK_LIBS)
#~ htmlchars_la_LIBADD     = $(GTK_LIBS)
#~ export_la_LIBADD        = $(GTK_LIBS)
#~ vcdiff_la_LIBADD        = $(GTK_LIBS)
#~ autosave_la_LIBADD      = $(GTK_LIBS)
#~ filebrowser_la_LIBADD   = $(GTK_LIBS)

endif # PLUGINS

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/tagmanager/include \
	-I$(top_srcdir)/scintilla/include \
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
