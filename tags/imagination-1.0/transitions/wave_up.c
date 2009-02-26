/*
 *  Copyright (c) 2009 Tadej Borovšak 	<tadeboro@gmail.com>
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
	*name = "Wave Up";
}

void img_transition_render(GdkDrawable *window, GdkPixbuf *image_from, GdkPixbuf *image_to, gdouble progress, gint file_desc)
{
	cairo_t *cr;
	cairo_surface_t *surface;
	cairo_pattern_t *pattern;
	gint	width, height;
	gdouble wave_factor = 1.07;	// 50px
	gdouble stop;

	gdk_drawable_get_size(window, &width, &height);

	if(file_desc < 0)
	{
		cr = gdk_cairo_create(window);
	}
	else
	{
		surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
		cr = cairo_create(surface);
	}

	gdk_cairo_set_source_pixbuf(cr,image_from,0,0);
	cairo_paint(cr);

	gdk_cairo_set_source_pixbuf(cr,image_to,0,0);
	pattern = cairo_pattern_create_linear( width / 2, height, width / 2, 0 );
	/* Add color stops */
	cairo_pattern_add_color_stop_rgba(pattern, (1 - wave_factor) / 2, 0, 0, 0, 1 );	// start

	stop = progress * wave_factor - ( wave_factor - 1 );
	cairo_pattern_add_color_stop_rgba(pattern, stop, 0, 0, 0, 1 );

	stop = progress * wave_factor;
	cairo_pattern_add_color_stop_rgba(pattern, stop, 0, 0, 0, 0 );

	cairo_pattern_add_color_stop_rgba(pattern, wave_factor, 0, 0, 0, 0 ); // stop

	cairo_mask(cr, pattern);
	cairo_destroy(cr);
	cairo_pattern_destroy(pattern);
	
	if(file_desc < 0)
		return;

	img_export_cairo_to_ppm(surface, file_desc);
	cairo_surface_destroy(surface);
}
