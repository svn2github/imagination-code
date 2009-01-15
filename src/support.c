/*
 *  Copyright (c) 2009 Giuseppe Torelli <colossus73@gmail.com>
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
#include "imagination.h"

GtkWidget *img_load_icon(gchar *filename, GtkIconSize size)
{
    GtkWidget *file_image;
	gchar *path;
	GdkPixbuf *file_pixbuf = NULL;
	path = g_strconcat(DATADIR, "/pixmaps/imagination/",filename,NULL);
	file_pixbuf = gdk_pixbuf_new_from_file(path,NULL);
	g_free (path);

	if (file_pixbuf == NULL)
		file_image = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE, size);
    else
	{
		file_image = gtk_image_new_from_pixbuf(file_pixbuf);
	    g_object_unref (file_pixbuf);
	}
    return file_image;
}
/*
cairo_surface_t *img_get_cairo_surface_from_gdk_pixbuf(GdkPixbuf *pixbuf)
{
	cairo_surface_t *image;
	cairo_t *cr;

	image = cairo_image_surface_create (gdk_pixbuf_get_has_alpha (pixbuf) ?
					      CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24,gdk_pixbuf_get_width (pixbuf),gdk_pixbuf_get_height (pixbuf));

	cr = cairo_create (image);
	gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
	cairo_paint (cr);
	cairo_destroy (cr);
	//g_object_unref(pixbuf);
	return image;
}
*/
void img_idle_function (gint seconds, img_window_struct *img)
{
	GTimer *time;
	gdouble secs;

	time = g_timer_new();
	g_print ("Aspetto %d sec\n",seconds);
	while(1)
	{
		secs = g_timer_elapsed(time,NULL);
		if ((gint)secs > seconds || img->preview_is_running == FALSE)
		{
			g_timer_stop(time);
			break;
		}
		while(gtk_events_pending())
			gtk_main_iteration();
	}
}

GtkWidget *_gtk_combo_box_new_text()
{
	GtkWidget *combo_box;
	GtkCellRenderer *cell;
	GtkListStore *store;

	store = gtk_list_store_new (1, G_TYPE_STRING);
	combo_box = gtk_combo_box_new_with_model (GTK_TREE_MODEL (store));
	g_object_unref (store);

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo_box), cell, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo_box), cell,"text", 0, NULL);
	g_object_set(cell,"ypad", 0.0, NULL);
	return combo_box;
}
