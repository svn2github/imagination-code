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
#include <string.h>

static void
img_export_cairo_to_ppm( cairo_surface_t *surface,
						 gint             file_desc )
{
	cairo_format_t  format;
	gint            width, height, stride, row, col;
	guchar         *data;
	gchar          *header;

	guchar         *buffer, *tmp;
	gint            buf_size;

	/* Get info about cairo surface passed in. */
	format = cairo_image_surface_get_format( surface );

	/* For more information on diferent formats, see
	 * www.cairographics.org/manual/cairo-image-surface.html#cairo-format-t */
	/* Currently this exporter only handles CAIRO_FORMAT_(ARGB32|RGB24)
	 * formats. */
	if( ! format == CAIRO_FORMAT_ARGB32 && ! format == CAIRO_FORMAT_RGB24 )
	{
		g_print( "Unsupported cairo surface format!\n" );
		return;
	}

	/* Image info and pixel data */
	width  = cairo_image_surface_get_width( surface );
	height = cairo_image_surface_get_height( surface );
	stride = cairo_image_surface_get_stride( surface );
	data   = cairo_image_surface_get_data( surface );

	/* Accomodate difference in RGB and ARGB formats */
	//if( format == CAIRO_FORMAT_ARGB32 )
	//	data++;

	/* Output PPM file header information:
	 *   - P6 is a magic number for PPM file
	 *   - width and height are image's dimensions
	 *   - 255 is number of colors
	 * */
	header = g_strdup_printf( "P6\n%d %d\n255\n", width, height );
	write( file_desc, header, sizeof( gchar ) * strlen( header ) );
	g_free( header );

	/* PRINCIPLES BEHING EXPORT LOOP
	 *
	 * Cairo surface data is composed of height * stride 32-bit numbers. The
	 * actual data for displaying image is inside height * width boundary,
	 * and each pixel is represented with 1 32-bit number.
	 *
	 * In CAIRO_FORMAT_ARGB32, first 8 bits contain alpha value, second 8
	 * bits red value, third green and fourth 8 bits blue value.
	 *
	 * In CAIRO_FORMAT_RGB24, groups of 8 bits contain values for red, green
	 * and blue color respectively. Last 8 bits are unused.
	 *
	 * Since guchar type contains 8 bits, it's usefull to think of cairo
	 * surface as a height * stride gropus of 4 guchars, where each guchar
	 * holds value for each color. And this is the principle behing my method
	 * of export.
	 * */

	/* Output PPM data */
	buf_size = sizeof( guchar ) * width * height * 3;
	buffer = g_slice_alloc( buf_size );
	tmp = buffer;
	for( row = 0; row < height; row++ )
	{
		for( col = 0; col < width; col++ )
		{
			/* Output data. We need to output data in reverse order, because
			 * majority of machines are small endian, but the PPM file needs
			 * it's bytes in big endian. */
			tmp[0] = data[2];
			tmp[1] = data[1];
			tmp[2] = data[0];
			data += 4;
			tmp  += 3;
		}
	}
	write( file_desc, buffer, buf_size );
	g_slice_free1( buf_size, buffer );
}

void img_transition_set_name(gchar **name)
{
	*name = "Blind Down";
}

void img_transition_render(GdkDrawable *window, GdkPixbuf *image_from, GdkPixbuf *image_to, gdouble progress, gint file_desc)
{
	cairo_t *cr;
	cairo_surface_t *surface;
	gint     width, height;

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


	cairo_rectangle(cr, 0, 0, width, height * progress);
	cairo_clip(cr);
	cairo_paint(cr);
	cairo_destroy(cr);

	if(file_desc < 0)
		return;

	img_export_cairo_to_ppm(surface, file_desc);
	cairo_surface_destroy(surface);
}
