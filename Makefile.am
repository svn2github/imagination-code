## Process this file with automake to produce Makefile.in

SUBDIRS = icons src transitions po

EXTRA_DIST = \
	autogen.sh \
	imagination.desktop.in \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in
	
uninstall-local:
	rm -rf $(DESTDIR)$(pkgdatadir);
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-import.png;
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-generate.png;
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-1.png;
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-2.png;
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-3.png;
	rm -f $(DESTDIR)$(datadir)/pixmaps/imagination-4.png;

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi

desktopdir = $(datadir)/applications
desktop_in_files = imagination.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@
