/*
** Copyright (C) 2009 Tadej Borovšak <tadeboro@gmail.com>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "subtitles.h"
#include "support.h"

/* Border width around image (no text is placed there) */
#define BORDER 20

/* ****************************************************************************
 * Local declarations
 * ************************************************************************* */
static void
img_calc_text_pos( gint       surface_w,
				   gint       surface_h,
				   gint       layout_w,
				   gint       layout_h,
				   ImgSubPos  position,
				   gdouble    factor,
				   gint      *posx,
				   gint      *posy );

static void
img_text_ani_none( cairo_t              *cr,
				   gint                  width,
				   gint                  height,
				   gdouble               progress,
				   const gchar          *text,
				   ImgSubPos             position,
				   PangoFontDescription *font_desc,
				   ImgRelPlacing         placing,
				   gdouble               factor,
				   gint                  offx,
				   gint                  offy );


/* ****************************************************************************
 * Function definitions
 * ************************************************************************* */

/*
 * img_get_text_animation_list:
 * @animations: location to put list of available text animations
 *
 * This function is here to simplify accessing all available animations.
 *
 * Any newly added exporters should be listed in array returned by this function
 * or Imagination WILL NOT create combo box entries for them.
 *
 * List that is placed in exporters parameter should be considered read-only and
 * freed after usage with img_free_text_animation_list. If @animations is NULL,
 * only number of available animations is returned.
 *
 * Return value: Size of list in animations.
 */
gint
img_get_text_animation_list( TextAnimation **animations )
{
	TextAnimation *list;              /* List of all animations */
	gint           no_animations = 1; /* Number of animations */
	gint           i = 0;

	if( animations )
	{
		/* Populate list */
		list = g_slice_alloc( sizeof( TextAnimation ) * no_animations );
		list[i].name   = g_strdup( _("None") );
		list[i++].func = img_text_ani_none;
		/* FIXME: Add more animations here */
		
		*animations = list;
	}

	return( no_animations );
}

/*
 * img_free_text_animation_list:
 * @no_animations: number of animations in @animations
 * @animations: array of TextAnimation structs
 *
 * This function takes care of freeing any memory allocated by
 * img_get_text_animation_list function.
 */
void
img_free_text_animation_list( gint           no_animations,
							  TextAnimation *animations )
{
	register gint i;

	for( i = 0; i < no_animations; i++ );
		g_free( animations[i].name );

	g_slice_free1( sizeof( TextAnimation ) * no_animations, animations );
}

static void
img_calc_text_pos( gint       surface_w,
				   gint       surface_h,
				   gint       layout_w,
				   gint       layout_h,
				   ImgSubPos  position,
				   gdouble    factor,
				   gint      *posx,
				   gint      *posy )
{
	switch( position )
	{
		case IMG_SUB_POS_TOP_LEFT:
			*posx = BORDER;
			*posy = BORDER;
			break;

		case IMG_SUB_POS_TOP_CENTER:
			*posx = ( surface_w * factor - layout_w ) / 2;
			*posy = BORDER;
			break;

		case IMG_SUB_POS_TOP_RIGHT:
			*posx = surface_w * factor - BORDER - layout_w;
			*posy = BORDER;
			break;

		case IMG_SUB_POS_MIDDLE_LEFT:
			*posx = BORDER;
			*posy = ( surface_h * factor - layout_h ) / 2;
			break;

		case IMG_SUB_POS_MIDDLE_CENTER:
			*posx = ( surface_w * factor - layout_w ) / 2;
			*posy = ( surface_h * factor - layout_h ) / 2;
			break;

		case IMG_SUB_POS_MIDDLE_RIGHT:
			*posx = surface_w * factor - BORDER - layout_w;
			*posy = ( surface_h * factor - layout_h ) / 2;
			break;

		case IMG_SUB_POS_BOTTOM_LEFT:
			*posx = BORDER;
			*posy = surface_h * factor - BORDER - layout_h;
			break;

		case IMG_SUB_POS_BOTTOM_CENTER:
			*posx = ( surface_w * factor - layout_w ) / 2;
			*posy = surface_h * factor - BORDER - layout_h;
			break;

		case IMG_SUB_POS_BOTTOM_RIGHT:
			*posx = surface_w * factor - BORDER - layout_w;
			*posy = surface_h * factor - BORDER - layout_h;
			break;
	}
}


/* ****************************************************************************
 * Text animation renderers
 * ************************************************************************* */
static void
img_text_ani_none( cairo_t              *cr,
				   gint                  width,
				   gint                  height,
				   gdouble               progress,
				   const gchar          *text,
				   ImgSubPos             position,
				   PangoFontDescription *font_desc,
				   ImgRelPlacing         placing,
				   gdouble               factor,
				   gint                  offx,
				   gint                  offy )
{
	PangoLayout *layout;
	gint         lw,
				 lh;
	gint         posx,
				 posy;

	/* If placing is relative to original image, we need to transform cairo
	 * context in order to get proper measurements */
	if( placing == IMG_REL_PLACING_ORIGINAL_IMAGE )
	{
		cairo_translate( cr, offx, offy );
		cairo_scale( cr, factor, factor );
	}

	/* Create pango layout, add text to it and measure it */
	layout = pango_cairo_create_layout( cr );
	pango_layout_set_markup( layout, text, -1 );
	pango_layout_set_font_description( layout, font_desc );

	pango_layout_get_size( layout, &lw, &lh );
	lw *= PANGO_SCALE;
	lh *= PANGO_SCALE;

	/* Calculate position according to user settings */
	if( placing == IMG_REL_PLACING_ORIGINAL_IMAGE )
		img_calc_text_pos( width, height, lw, lh, position,
						   factor, &posx, &posy );
	else
		img_calc_text_pos( width, height, lw, lh, position,
						   1, &posx, &posy );

	/* Render text */
	cairo_move_to( cr, posx, posy );
	pango_cairo_show_layout( cr, layout );

	/* Destroy layout */
	g_object_unref( layout );
}

