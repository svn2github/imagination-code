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
	gint	width, height;

	gdk_drawable_get_size(window, &width, &height);

	cr = gdk_cairo_create(window);
	gdk_cairo_set_source_pixbuf(cr,image_from,0,0);
	cairo_paint(cr);

	gdk_cairo_set_source_pixbuf(cr,image_to,0,height * (1 - progress));

	cairo_paint(cr);
	cairo_destroy(cr);
}
