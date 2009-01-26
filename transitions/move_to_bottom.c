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
	*name = "Move From Bottom To Top";
}

void img_transition_render(GdkDrawable *window, GdkPixbuf *image_from, GdkPixbuf *image_to, gdouble progress)
{
	cairo_t *cr;
	gdouble x,y;
	gint	width, height;

	gdk_drawable_get_size(window, &width, &height);
	x = (width  - gdk_pixbuf_get_width (image_from)) / 2;
	y = (height - gdk_pixbuf_get_height(image_from)) / 2;

	cr = gdk_cairo_create(window);
	gdk_cairo_set_source_pixbuf(cr,image_from,(gint)x,(gint)y);
	cairo_paint(cr);

	x = (width  - gdk_pixbuf_get_width (image_to)) / 2;
	y = height;

	y *= (1 - progress);
	if ( y < (height - gdk_pixbuf_get_height(image_from)) / 2)
		y = height - gdk_pixbuf_get_height(image_from) / 2;
	gdk_cairo_set_source_pixbuf(cr,image_to,x,y);

	cairo_paint(cr);
	cairo_destroy(cr);
}
