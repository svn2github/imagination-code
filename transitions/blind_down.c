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

#include <gdk/gdk.h>

void img_transition_set_name(gchar **name)
{
	*name = "Blind Down";
}

void img_transition_render(GdkDrawable *window, GdkPixbuf *image_from, GdkPixbuf *image_to, gdouble progress)
{
	cairo_t *cr;
	gint     offset_x,offset_y, width, height;

	gdk_drawable_get_size(window, &width, &height);
	offset_x = (width  - gdk_pixbuf_get_width (image_from)) / 2;
	offset_y = (height - gdk_pixbuf_get_height(image_from)) / 2;

	cr = gdk_cairo_create(window);
	gdk_cairo_set_source_pixbuf(cr,image_from,offset_x,offset_y);
	cairo_paint(cr);

	offset_x = (width  - gdk_pixbuf_get_width (image_to)) / 2;
	offset_y = (height - gdk_pixbuf_get_height(image_to)) / 2;
	gdk_cairo_set_source_pixbuf(cr,image_to,offset_x,offset_y);

	cairo_rectangle(cr, 0, 0, width, height * progress);
	cairo_clip(cr);
	cairo_paint(cr);
	cairo_destroy(cr);
}
