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

#include "support.h"

static GList *pixmaps_directories = NULL;

void add_pixmap_directory (const gchar *directory)
{
  pixmaps_directories = g_list_prepend (pixmaps_directories,g_strdup (directory));
}

void img_set_window_title(img_window_struct *img,gchar *text)
{
	gchar *title = g_strconcat("Imagination " VERSION,NULL);
	gtk_window_set_title ((GtkWindow *)img->imagination_window,title);
	g_free(title);
}
