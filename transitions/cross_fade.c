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

/* This header provides us with image manipulation libraries such
 * as gdk-pixbuf and cairo. */
#include <gdk/gdk.h>

/* This function is part of the mandatory API that each plug-in
 * must adhere to.
 *
 * name parameter of this function is a return location for
 * plug-in name. You must fill it with unique name, which will
 * be used to identify transition in transition type combo box. */
void img_transition_set_name(gchar **name)
{
	*name = "Cross Fade";
}

/* This function is part of the mandatory API that each plug-in
 * must adhere to.
 *
 * This is the function that does all the transition rendering.
 * Parameters:
 *   window		GdkDrawable on which the final product should be drawn
 *   image_from	Starting pixbuf.
 *   image_to	End pixbuf. Both images are already scaled and transformed
 *				to fit into GdkDrawable.
 *   progress	Value in range [0,1] which tells us how far the transition
 *				is (0 means we only started transition, 1 means the
 *				transition is finished).
 *	 file_desc	File descriptor for outputting finished image (this file
 *				descriptor represents the ffmpeg's stdin, on which we
 *				write produced image data). If the file descriptor is < 0,
 *				we should draw our produt on GtkDrawable.
 *
 * Note: Parts enclosed in SUGGESTED comments should not be changed, since
 *       most of the transitions will need them.
 */
void img_transition_render(GdkDrawable *window, GdkPixbuf *image_from, GdkPixbuf *image_to, gdouble progress, gint file_desc)
{
	/* SUGGESTED */
	cairo_t *cr;
	cairo_surface_t *surface;
	gint	width, height;

	/* With this call we obtain the dimensions of our drawing area.
	 * We don't use these values in this plug-in, but this is really
	 * simple plug-in. */
	gdk_drawable_get_size(window, &width, &height);

	/* This is where we check if we need to output our product on the screen
	 * or write it to the file desc. */
	if(file_desc < 0)
	{
		cr = gdk_cairo_create(window);
	}
	else
	{
		surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
		cr = cairo_create(surface);
	}
	/* END SUGGESTED */

	/* Here we do our own drawing. In this example, we are fading image_to
	 * pixbuf over the image_from pixbuf. The amount of transparency is
	 * determined by the progress value (at the start of the transition,
	 * image_to is fully transparent and at the end fully opaque). */
	gdk_cairo_set_source_pixbuf(cr,image_from,0,0);
	cairo_paint(cr);

	gdk_cairo_set_source_pixbuf(cr,image_to,0,0);
	cairo_paint_with_alpha(cr,progress);
	cairo_destroy(cr);
	
	/* SUGGESTED */
	if(file_desc < 0)
		return;

	/* This function takes the burden of converting drawing product
	 * to appropriate pixel data from plug-in write's shoulders. */
	img_export_cairo_to_ppm(surface, file_desc);
	cairo_surface_destroy(surface);
	/* END SUGGESTED */
}
