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

#include "imagination.h"

void img_transition_render(GtkWidget *widget, img_window_struct *img)
{
	cairo_t *cr;
	gint     offset_x,offset_y;
	gdouble  radius = 512;

	offset_x = ((img->image_area)->allocation.width  - gdk_pixbuf_get_width (img->pixbuf1)) / 2;
	offset_y = ((img->image_area)->allocation.height - gdk_pixbuf_get_height(img->pixbuf1)) / 2;

	cr = gdk_cairo_create(widget->window);
	gdk_cairo_set_source_pixbuf(cr,img->pixbuf1,offset_x,offset_y);
	cairo_paint(cr);

	offset_x = ((img->image_area)->allocation.width  - gdk_pixbuf_get_width (img->pixbuf2)) / 2;
	offset_y = ((img->image_area)->allocation.height - gdk_pixbuf_get_height(img->pixbuf2)) / 2;
	gdk_cairo_set_source_pixbuf(cr,img->pixbuf2,offset_x,offset_y);

	cairo_arc(cr, gdk_pixbuf_get_width(img->pixbuf2)/2, gdk_pixbuf_get_height(img->pixbuf2)/2, radius * img->progress, 0, 2 * G_PI);
	cairo_clip(cr);
	cairo_paint(cr);
	cairo_destroy(cr);
}
