/*
 *  Copyright (c) 2008 Giuseppe Torelli <colossus73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License,or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not,write to the Free Software
 *  Foundation,Inc.,59 Temple Place - Suite 330,Boston,MA 02111-1307,USA.
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "main-window.h"
#include "support.h"

int main (int argc, char *argv[])
{
	img_window_struct *img_window;

	#ifdef ENABLE_NLS
  		bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  		textdomain (GETTEXT_PACKAGE);
	#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);

	add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

	img_window = img_create_window();
	gtk_widget_show (img_window->imagination_window);
	gtk_statusbar_push((GtkStatusbar*)img_window->statusbar,img_window->message_id,_("Welcome to Imagination " VERSION));

	gtk_main ();
	g_free(img_window);
	return 0;
}

