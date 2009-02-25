# Adapted from Geany's plugins/Makefile.am, thanks

plugindir = $(libdir)/imagination

plugins_includedir = $(includedir)/imagination/

circular_blind_from_center_la_LDFLAGS		= -module -avoid-version
circular_blind_to_center_la_LDFLAGS		= -module -avoid-version
blind_down_la_LDFLAGS	= -module -avoid-version
blind_up_la_LDFLAGS		= -module -avoid-version
blind_right_la_LDFLAGS	= -module -avoid-version
blind_left_la_LDFLAGS	= -module -avoid-version
blind_top_right_la_LDFLAGS	= -module -avoid-version
blind_top_left_la_LDFLAGS	= -module -avoid-version
blind_bottom_left_la_LDFLAGS	= -module -avoid-version
blind_bottom_right_la_LDFLAGS	= -module -avoid-version
move_from_bottom_la_LDFLAGS = -module -avoid-version
move_to_bottom_la_LDFLAGS = -module -avoid-version
push_to_bottom_la_LDFLAGS = -module -avoid-version
push_from_bottom_la_LDFLAGS = -module -avoid-version
push_from_left_la_LDFLAGS = -module -avoid-version
push_from_right_la_LDFLAGS = -module -avoid-version
roller_blind_la_LDFLAGS = -module -avoid-version
cross_fade_la_LDFLAGS = -module -avoid-version
wave_right_la_LDFLAGS = -module -avoid-version
wave_left_la_LDFLAGS = -module -avoid-version
wave_up_la_LDFLAGS = -module -avoid-version
wave_down_la_LDFLAGS = -module -avoid-version
wave_left_down_la_LDFLAGS = -module -avoid-version
wave_left_up_la_LDFLAGS = -module -avoid-version
wave_right_down_la_LDFLAGS = -module -avoid-version
wave_right_up_la_LDFLAGS = -module -avoid-version

# Plugins to be installed
plugin_LTLIBRARIES = \
	circular_blind_from_center.la		\
	circular_blind_to_center.la		\
	blind_down.la	\
	blind_up.la		\
	blind_right.la	\
	blind_left.la	\
	blind_top_right.la \
	blind_top_left.la	\
	blind_bottom_left.la \
	blind_bottom_right.la \
	move_from_bottom.la	\
	move_to_bottom.la	\
	push_to_bottom.la	\
	push_from_bottom.la	\
	push_from_left.la	\
	push_from_right.la	\
	roller_blind.la		\
	cross_fade.la		\
	wave_right.la		\
	wave_left.la		\
	wave_up.la			\
	wave_down.la		\
	wave_left_down.la	\
	wave_left_up.la		\
	wave_right_down.la	\
	wave_right_up.la
	

circular_blind_from_center_la_SOURCES		= ../src/support.c circular_blind_from_center.c
circular_blind_to_center_la_SOURCES			= ../src/support.c circular_blind_to_center.c
blind_down_la_SOURCES	= ../src/support.c blind_down.c
blind_up_la_SOURCES		= ../src/support.c blind_up.c
blind_right_la_SOURCES	= ../src/support.c blind_right.c
blind_left_la_SOURCES	= ../src/support.c blind_left.c
blind_top_right_la_SOURCES	= ../src/support.c blind_top_right.c
blind_top_left_la_SOURCES	= ../src/support.c blind_top_left.c
blind_bottom_left_la_SOURCES	= ../src/support.c blind_bottom_left.c
blind_bottom_right_la_SOURCES	= ../src/support.c blind_bottom_right.c
move_from_bottom_la_SOURCES	= ../src/support.c move_from_bottom.c
move_to_bottom_la_SOURCES	= ../src/support.c move_to_bottom.c
push_to_bottom_la_SOURCES	= ../src/support.c push_to_bottom.c
push_from_bottom_la_SOURCES	= ../src/support.c push_from_bottom.c
push_from_left_la_SOURCES	= ../src/support.c push_from_left.c
push_from_right_la_SOURCES	= ../src/support.c push_from_right.c
roller_blind_la_SOURCES	= ../src/support.c roller_blind.c
cross_fade_la_SOURCES	= ../src/support.c cross_fade.c
wave_right_la_SOURCES	= ../src/support.c wave_right.c
wave_left_la_SOURCES	= ../src/support.c wave_left.c
wave_up_la_SOURCES	= ../src/support.c wave_up.c
wave_down_la_SOURCES	= ../src/support.c wave_down.c
wave_left_down_la_SOURCES	= ../src/support.c wave_left_down.c
wave_left_up_la_SOURCES	= ../src/support.c wave_left_up.c
wave_right_down_la_SOURCES	= ../src/support.c wave_right_down.c
wave_right_up_la_SOURCES	= ../src/support.c wave_right_up.c

# instead of linking against all in $(GTK_LIBS), we link only against needed libs
circular_blind_from_center_la_LIBADD	 = $(PACKAGE_LIBS)
circular_blind_to_center_la_LIBADD	 = $(PACKAGE_LIBS)
blind_down_la_LIBADD = $(PACKAGE_LIBS)
blind_up_la_LIBADD 	 = $(PACKAGE_LIBS)
blind_right_la_LIBADD= $(PACKAGE_LIBS)
blind_left_la_LIBADD = $(PACKAGE_LIBS)
blind_top_right_la_LIBADD = $(PACKAGE_LIBS)
blind_top_left_la_LIBADD = $(PACKAGE_LIBS)
blind_bottom_left_la_LIBADD = $(PACKAGE_LIBS)
blind_bottom_right_la_LIBADD = $(PACKAGE_LIBS)
move_from_bottom_la_LIBADD = $(PACKAGE_LIBS)
move_to_bottom_la_LIBADD = $(PACKAGE_LIBS)
push_to_bottom_la_LIBADD = $(PACKAGE_LIBS)
push_from_bottom_la_LIBADD = $(PACKAGE_LIBS)
push_from_left_la_LIBADD = $(PACKAGE_LIBS)
push_from_right_la_LIBADD = $(PACKAGE_LIBS)
roller_blind_la_LIBADD = $(PACKAGE_LIBS)
cross_fade_la_LIBADD = $(PACKAGE_LIBS)
wave_right_la_LIBADD = $(PACKAGE_LIBS)
wave_left_la_LIBADD = $(PACKAGE_LIBS)
wave_up_la_LIBADD = $(PACKAGE_LIBS)
wave_down_la_LIBADD = $(PACKAGE_LIBS)
wave_left_down_la_LIBADD = $(PACKAGE_LIBS)
wave_left_up_la_LIBADD = $(PACKAGE_LIBS)
wave_right_down_la_LIBADD = $(PACKAGE_LIBS)
wave_right_up_la_LIBADD = $(PACKAGE_LIBS)

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
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
