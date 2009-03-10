# Adapted from Geany's plugins/Makefile.am, thanks

plugindir = $(libdir)/imagination

plugins_includedir = $(includedir)/imagination/

bar_wipe_la_LDFLAGS = -module -avoid-version
box_wipe_la_LDFLAGS = -module -avoid-version
four_box_wipe_la_LDFLAGS = -module -avoid-version
barn_door_wipe_la_LDFLAGS = -module -avoid-version
fade_la_LDFLAGS = -module -avoid-version
ellipse_wipe_la_LDFLAGS = -module -avoid-version
push_wipe_la_LDFLAGS = -module -avoid-version

# Plugins to be installed
plugin_LTLIBRARIES = \
	bar_wipe.la \
	box_wipe.la \
	four_box_wipe.la \
	barn_door_wipe.la \
	fade.la				\
	ellipse_wipe.la		\
	push_wipe.la

bar_wipe_la_SOURCES	= ../src/support.c bar_wipe.c
box_wipe_la_SOURCES = ../src/support.c box_wipe.c
four_box_wipe_la_SOURCES = ../src/support.c four_box_wipe.c
barn_door_wipe_la_SOURCES = ../src/support.c barn_door_wipe.c
fade_la_SOURCES = ../src/support.c fade.c
ellipse_wipe_la_SOURCES = ../src/support.c ellipse_wipe.c
push_wipe_la_SOURCES = ../src/support.c push_wipe.c

# instead of linking against all in $(GTK_LIBS), we link only against needed libs
bar_wipe_la_LIBADD = $(PACKAGE_LIBS)
box_wipe_la_LIBADD = $(PACKAGE_LIBS)
four_box_wipe_la_LIBADD = $(PACKAGE_LIBS)
barn_door_wipe_la_LIBADD = $(PACKAGE_LIBS)
fade_la_LIBADD = $(PACKAGE_LIBS)
ellipse_wipe_la_LIBADD = $(PACKAGE_LIBS)
push_wipe_la_LIBADD = $(PACKAGE_LIBS)

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DPACKAGE_LIB_DIR=\""$(libdir)"\" \
	-I$(top_srcdir)/src \
	$(PACKAGE_CFLAGS) \
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
