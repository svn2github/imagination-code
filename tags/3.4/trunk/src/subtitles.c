/*
** Copyright (C) 2009-2018 Giuseppe Torelli <colossus73@gmail.com>
** Copyright (C) 2009 Tadej Borovšak <tadeboro@gmail.com>
** Copyright (C) 2010 Robert Chéramy <robert@cheramy.net>
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

/* Wrap width for subtitles (fraction of image size) */
#define WRAP_WIDTH 0.75

/* ****************************************************************************
 * Local declarations
 * ************************************************************************* */
static void
img_text_ani_fade( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar		*pattern_filename,
				   gdouble      progress,
				   gdouble     *font_color,
				   gdouble     *font_brdr_color,
				   gdouble     *font_bgcolor,
				   gdouble     *border_color,
				   gboolean		top_border,
                   gboolean		bottom_border,
                   gint			border_width);

static void
img_text_draw_layout( cairo_t     *cr,
                      PangoLayout *layout,
                      gint			posx,
                      gint 			posy,
                      gint 			angle,
                      gchar	  	  *pattern_filename,
                      gdouble     *font_color,
                      gdouble     *font_brdr_color,
                      gdouble     *font_bg_color,
                      gdouble     *border_color,
                      gboolean		top_border,
                      gboolean		bottom_border,
                      gint	      border_width);

static void
img_text_from_left( cairo_t     *cr,
					PangoLayout *layout,
 					gint         sw,
 					gint         sh,
 					gint         lw,
 					gint         lh,
 					gint         posx,
 					gint         posy,
 					gint         angle,
 					gchar		*pattern_filename,
 					gdouble      progress,
                    gdouble     *font_color,
                    gdouble     *font_brdr_color,
                    gdouble     *font_bgcolor,
                    gdouble     *border_color,
                    gboolean	top_border,
                    gboolean	bottom_border,
                    gint	     border_width);

static void
img_text_from_right( cairo_t     *cr,
					 PangoLayout *layout,
 					 gint         sw,
 					 gint         sh,
 					 gint         lw,
 					 gint         lh,
 					 gint         posx,
 					 gint         posy,
 					 gint         angle,
 					 gchar		*pattern_filename,
 					 gdouble      progress,
                     gdouble     *font_color,
                     gdouble     *font_brdr_color,
                     gdouble     *font_bgcolor,
                     gdouble     *border_color,
                     gboolean	top_border,
                     gboolean	bottom_border,
                     gint	      border_width);

static void
img_text_from_top( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar		*pattern_filename,
				   gdouble      progress,
                   gdouble     *font_color,
                   gdouble     *font_brdr_color,
                   gdouble     *font_bgcolor,
                   gdouble     *border_color,
                   gboolean		top_border,
                   gboolean		bottom_border,
                   gint	      border_width);
                
static void
img_text_from_bottom( cairo_t     *cr,
					  PangoLayout *layout,
					  gint         sw,
					  gint         sh,
  					  gint         lw,
  					  gint         lh,
  					  gint         posx,
  					  gint         posy,
  					  gint         angle,
  					  gchar			*pattern_filename,
  					  gdouble      progress,
                      gdouble     *font_color,
                      gdouble     *font_brdr_color,
                      gdouble     *font_bgcolor,
                      gdouble     *border_color,
                      gboolean		top_border,
                      gboolean		bottom_border,
                      gint	      border_width);

static void
img_text_grow( cairo_t     *cr,
			   PangoLayout *layout,
			   gint         sw,
			   gint         sh,
			   gint         lw,
			   gint         lh,
			   gint         posx,
			   gint         posy,
			   gint         angle,
			   gchar		*pattern_filename,
               gdouble      progress,
			   gdouble     *font_color,
               gdouble     *font_brdr_color,
               gdouble     *font_bgcolor,
               gdouble     *border_color,
			   gboolean		top_border,
               gboolean		bottom_border,
               gint	      border_width);

static void
img_text_bottom_to_top( cairo_t     *cr,
					PangoLayout *layout,
 					gint         sw,
 					gint         sh,
 					gint         lw,
 					gint         lh,
 					gint         posx,
 					gint         posy,
 					gint         angle,
 					gchar		*pattern_filename,
 					gdouble      progress,
                    gdouble     *font_color,
                    gdouble     *font_brdr_color,
                    gdouble     *font_bgcolor,
                    gdouble     *border_color,
                    gboolean		top_border,
                    gboolean		bottom_border,
                    gint	      border_width);

static void
img_text_right_to_left( cairo_t     *cr,
					PangoLayout *layout,
 					gint         sw,
 					gint         sh,
 					gint         lw,
 					gint         lh,
 					gint         posx,
 					gint         posy,
 					gint         angle,
 					gchar		*pattern_filename,
 					gdouble      progress,
                    gdouble     *font_color,
                    gdouble     *font_brdr_color,
                    gdouble     *font_bgcolor,
                    gdouble     *border_color,
                    gboolean		top_border,
                    gboolean		bottom_border,
                    gint	    border_width);
                      
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
	gint           no_animations = 9; /* Number of animations */
	gint           i = 0;

	if( animations )
	{
		/* Populate list */
		/* DO NOT SHUFFLE THIS LIST! ONLY ADD NEW ANIMATIONS AT THE END OF THE
		 * LIST OR LOADING OF OLD PROJECTS WON'T WORK PROPERLY!!!! */
		list = g_slice_alloc( sizeof( TextAnimation ) * no_animations );

		/* No animation function (id = 0) */
		list[i].name   = g_strdup( _("None") );
		list[i].id     = i;
		list[i++].func = NULL;

		list[i].name   = g_strdup( _("Fade") );
		list[i].id     = i;
		list[i++].func = img_text_ani_fade;

		list[i].name   = g_strdup( _("Slide from left") );
		list[i].id     = i;
		list[i++].func = img_text_from_left;

		list[i].name   = g_strdup( _("Slide from right") );
		list[i].id     = i;
		list[i++].func = img_text_from_right;

		list[i].name   = g_strdup( _("Slide from top") );
		list[i].id     = i;
		list[i++].func = img_text_from_top;

		list[i].name   = g_strdup( _("Slide from bottom") );
		list[i].id     = i;
		list[i++].func = img_text_from_bottom;

		list[i].name   = g_strdup( _("Grow") );
		list[i].id     = i;
		list[i++].func = img_text_grow;

        list[i].name   = g_strdup( _("Slide bottom to top") );
        list[i].id     = i;
        list[i++].func = img_text_bottom_to_top;

        list[i].name   = g_strdup( _("Slide right to left") );
        list[i].id     = i;
        list[i++].func = img_text_right_to_left;

		/* FIXME: Add more animations here.
		 *
		 * DO NOT FORGET TO UPDATE no_animations VARIABLE AT THE TOP OF THIS
		 * FUNCTION WHEN ADDING NEW ANIMATIONS!! */
		
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

	for( i = 0; i < no_animations; i++ )
		g_free( animations[i].name );

	g_slice_free1( sizeof( TextAnimation ) * no_animations, animations );
}

void
img_render_subtitle( img_window_struct 	  *img,
					 cairo_t              *cr,
					 gint                  width,
					 gint                  height,
					 gdouble               zoom,
					 gint				   posx,
					 gint				   posy,
					 gint				   angle,
					 gdouble               factor,
					 gdouble               offx,
					 gdouble               offy,
					 gchar                *subtitle,
					 gchar                *pattern_filename,
					 PangoFontDescription *font_desc,
					 gdouble              *font_color,
                     gdouble              *font_brdr_color,
                     gdouble              *font_bg_color,
                     gdouble              *border_color,
                     gboolean				top_border,
                     gboolean				bottom_border,
                     gint	              border_width,
                     TextAnimationFunc     func,
                     gboolean				centerX,
                     gboolean				centerY,
					 gdouble               progress )
{
	gint		 lw,     /* Layout width */
				 lh;     /* Layout height */

	PangoLayout *layout;

	/* Save cairo state */
	cairo_save( cr );

	cairo_scale( cr, zoom, zoom );

	/* Create pango layout and measure it */
	layout = pango_cairo_create_layout( cr );
	pango_layout_set_font_description( layout, font_desc );
	
	/* Disable wrapping
	pango_layout_set_wrap( layout, PANGO_WRAP_WORD );
	*/
	pango_layout_set_text( layout, subtitle, -1 );
	pango_layout_get_size( layout, &lw, &lh );
	lw /= PANGO_SCALE;
	lh /= PANGO_SCALE;
	
	/* Disable wrapping
	if( lw > ( width * WRAP_WIDTH ) )
	{
		pango_layout_set_width( layout, width * WRAP_WIDTH * PANGO_SCALE );
		pango_layout_get_size( layout, &lw, &lh );
		lw /= PANGO_SCALE;
	}
	*/
	if (centerX)
	{
		posx = (img->video_size[0] - lw) /2;
		img->current_slide->posX = posx;
		gtk_range_set_value( GTK_RANGE(img->sub_posX), (gdouble) img->current_slide->posX);
	}
	if (centerY)
	{
		posy = (img->video_size[1] - lh) /2;
		img->current_slide->posY = posy;
		gtk_range_set_value( GTK_RANGE(img->sub_posY), (gdouble) img->current_slide->posY);
	}

	/* Do animation */
	if( func )
		(*func)( cr, layout, width, height, lw, lh, posx, posy, angle, pattern_filename, progress, font_color, font_brdr_color, font_bg_color, border_color,
				top_border, bottom_border, border_width );
	else
	{
		/* No animation renderer */
        img_text_draw_layout(cr, layout, posx, posy, angle, pattern_filename, font_color, font_brdr_color, font_bg_color, border_color,
        top_border, bottom_border, border_width);
	}

	/* Destroy layout */
	g_object_unref( G_OBJECT( layout ) );

	/* Restore cairo */
	cairo_restore( cr );
}

/* ****************************************************************************
 * Text animation renderers
 * ************************************************************************* */
static void
img_text_ani_fade( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar		*filename,
				   gdouble      progress,
                   gdouble     *font_color,
                   gdouble     *font_brdr_color,
                   gdouble     *font_bgcolor,
                   gdouble     *border_color,
                   gboolean		top_border,
                   gboolean		bottom_border,
                   gint			border_width)
{
    gdouble  progress_font_color[4], progress_font_brdr_color[4], progress_font_bgcolor[4], progress_border_color[4];

	/* Calculate colors */
    progress_font_color[0] = font_color[0];
    progress_font_color[1] = font_color[1];
    progress_font_color[2] = font_color[2];
    progress_font_color[3] = font_color[3] * progress;

	progress_font_brdr_color[0] = font_brdr_color[0];
    progress_font_brdr_color[1] = font_brdr_color[1];
    progress_font_brdr_color[2] = font_brdr_color[2];
    progress_font_brdr_color[3] = font_brdr_color[3] * pow(progress, 6);

    progress_font_bgcolor[0] = font_bgcolor[0];
    progress_font_bgcolor[1] = font_bgcolor[1];
    progress_font_bgcolor[2] = font_bgcolor[2];
    progress_font_bgcolor[3] = font_bgcolor[3] * pow(progress, 6);
    
    progress_border_color[0] = border_color[0];
    progress_border_color[1] = border_color[1];
    progress_border_color[2] = border_color[2];
    progress_border_color[3] = border_color[3] * pow(progress, 6);

    /* Paint text */
    img_text_draw_layout(cr, layout, posx, posy, angle, filename, progress_font_color, progress_font_brdr_color, progress_font_bgcolor, progress_border_color, top_border, bottom_border, border_width);
}

void
img_set_slide_text_info( slide_struct      *slide,
						 GtkListStore      *store,
						 GtkTreeIter       *iter,
						 const gchar       *subtitle,
						 gchar				*pattern_filename,
						 gint	            anim_id,
						 gint               anim_duration,
						 gint               posx,
						 gint               posy,
						 gint               angle,
						 const gchar       *font_desc,
						 gdouble           *font_color,
                         gdouble           *font_brdr_color,
                         gdouble           *font_bg_color,
                         gdouble           *border_color,
                         gboolean			top_border,
						 gboolean			bottom_border,
                         gint	           border_width,
						 img_window_struct *img )
{
	/* Set the slide text info parameters */
	if( store && iter )
	{
		gboolean flag;

		if( slide->subtitle )
			g_free( slide->subtitle );
		slide->subtitle = g_strdup( subtitle );
		
		if( slide->pattern_filename )
			g_free( slide->pattern_filename );
		slide->pattern_filename = g_strdup( pattern_filename );

		flag = ( subtitle ? TRUE : FALSE );
		gtk_list_store_set( store, iter, 3, flag, -1 );
	}

	if( ( anim_id > -1 ) && ( anim_id != slide->anim_id ) )
	{
		GtkTreeModel *model;
		gchar        *path;
		GtkTreeIter   iter;

		path = g_strdup_printf( "%d", anim_id );
		model = gtk_combo_box_get_model( GTK_COMBO_BOX( img->sub_anim ) );
		gtk_tree_model_get_iter_from_string( model, &iter, path );
		g_free( path );

		slide->anim_id = anim_id;
		gtk_tree_model_get( model, &iter, 1, &slide->anim, -1 );

		/* Sync timings */
		img_sync_timings( slide, img );
	}

	if( ( anim_duration > 0 ) && ( anim_duration != slide->anim_duration ) )
	{
		slide->anim_duration = anim_duration;

		/* Synchronize timings */
		img_sync_timings( slide, img );
	}

	if (posx > -1 || posy > -1)
	{
		slide->posX = posx;
		slide->posY = posy;
	}
	
	slide->subtitle_angle = angle;

	if( font_desc )
	{
		if( slide->font_desc )
			pango_font_description_free( slide->font_desc );
		slide->font_desc = pango_font_description_from_string( font_desc );
	}

	if( font_color )
	{
		slide->font_color[0] = font_color[0];
		slide->font_color[1] = font_color[1];
		slide->font_color[2] = font_color[2];
		slide->font_color[3] = font_color[3];
	}

    if( font_brdr_color )
    {
        slide->font_brdr_color[0] = font_brdr_color[0];
        slide->font_brdr_color[1] = font_brdr_color[1];
        slide->font_brdr_color[2] = font_brdr_color[2];
        slide->font_brdr_color[3] = font_brdr_color[3];
    }
    
    if( font_bg_color )
    {
        slide->font_bg_color[0] = font_bg_color[0];
        slide->font_bg_color[1] = font_bg_color[1];
        slide->font_bg_color[2] = font_bg_color[2];
        slide->font_bg_color[3] = font_bg_color[3];
    }
    
    if( border_color )
    {
        slide->border_color[0] = border_color[0];
        slide->border_color[1] = border_color[1];
        slide->border_color[2] = border_color[2];
        slide->border_color[3] = border_color[3];
    }
    
    slide->top_border = top_border;
    slide->bottom_border = bottom_border;
    
	if( border_width > 0)
		slide->border_width = border_width;
}								

static void
img_text_draw_layout( cairo_t     *cr,
                      PangoLayout *layout,
                      gint         posx,
                      gint         posy,
                      gint         angle,
                      gchar	  	  *pattern_filename,
                      gdouble     *font_color,
                      gdouble     *font_brdr_color,
                      gdouble     *font_bg_color,
                      gdouble     *border_color,
                      gboolean		top_border,
                      gboolean		bottom_border,
                      gint			border_width)
{
	cairo_pattern_t  *font_pattern = NULL;
    gint x,y,w,h;
	gdouble cairo_factor;

	pango_layout_get_pixel_size (layout, &w, &h );
	pango_layout_set_alignment( layout, PANGO_ALIGN_CENTER );

	/* Subtitle angle */
	cairo_translate (cr, posx + (w / 2), posy + (h / 2) );
	cairo_rotate (cr, angle * G_PI / 180.0);
	cairo_translate (cr, -(posx + (w / 2)), -(posy + (h / 2)) );
	pango_cairo_update_layout (cr, layout);
	
	/* Paint the background only if the user
	 * chose an alpha value greater than 0 */
	if (font_bg_color[3] > 0)
	{
		pango_layout_get_pixel_size (layout, &w, &h );
		cairo_set_source_rgba(cr, font_bg_color[0],
							  font_bg_color[1],
                              font_bg_color[2],
                              font_bg_color[3] );
                              
        cairo_rectangle(cr, posx - 5, posy, w + 12, h + 5);
		cairo_fill(cr);
	}
	
	if (top_border || bottom_border)
	{
		cairo_set_line_width(cr, (gdouble) border_width);
		cairo_set_source_rgba(cr, border_color[0],
							  border_color[1],
                              border_color[2],
                              border_color[3] );
        
        if (border_width % 2 == 0)
			cairo_factor = 0;
		else
			cairo_factor = 0.5;

        
        if (top_border)
		{
			cairo_move_to(cr, posx - 5, posy + cairo_factor);  
			cairo_line_to(cr, (posx + 3) + w + 4, posy + cairo_factor);
		}
		if (bottom_border)
		{
			cairo_move_to(cr, posx - 5, posy + h + 5 + cairo_factor);  
			cairo_line_to(cr, (posx + 3) + w + 4, posy + h + 5 + cairo_factor);
		}
		cairo_stroke(cr);
	}

	/* Draw the border only if the user
	 * chose an alpha value greater than 0 */
	if (font_brdr_color[3] > 0)
	{
		cairo_set_source_rgba(cr, font_brdr_color[0],
								  font_brdr_color[1],
								  font_brdr_color[2],
								  font_brdr_color[3] );
		for (x=-1; x <=1; x++)
		{
			for (y=-1; y<=1; y++)
			{
				cairo_move_to( cr, posx + x, posy + y );
				pango_cairo_show_layout( cr, layout );
			}
		}
	}
	/* Set the user chosen pattern
	 * to draw the subtitle */
	if (pattern_filename)
	{
		cairo_surface_t  *tmp_surf;
		cairo_matrix_t	matrix;

		tmp_surf = cairo_image_surface_create_from_png(pattern_filename);
		font_pattern = cairo_pattern_create_for_surface(tmp_surf);
		cairo_set_source(cr, font_pattern);
		cairo_pattern_set_extend (font_pattern, CAIRO_EXTEND_REPEAT);

		cairo_matrix_init_translate(&matrix, -posx, -posy);
		cairo_pattern_set_matrix (font_pattern, &matrix);
	}
	else
	{
		/* Draw the subtitle */
		/* Set source color */
		cairo_set_source_rgba( cr, font_color[0],
								font_color[1],
								font_color[2],
								font_color[3] );
	}
    /* Move to proper place and paint text */
	cairo_move_to( cr, posx, posy );
	pango_cairo_show_layout( cr, layout );
	
	if (pattern_filename)
		cairo_pattern_destroy(font_pattern);
}

static void
img_text_from_left( cairo_t     *cr,
					PangoLayout *layout,
 					gint         sw,
 					gint         sh,
 					gint         lw,
 					gint         lh,
 					gint         posx,
 					gint         posy,
 					gint         angle,
 					gchar	  	*filename,
 					gdouble      progress,
                    gdouble     *font_color,
                    gdouble     *font_brdr_color,
                    gdouble     *font_bgcolor,
                    gdouble     *border_color,
                    gboolean	top_border,
                    gboolean	bottom_border,
                    gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         posx * progress - lw * ( 1 - progress ),
                         posy,
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}

static void
img_text_from_right( cairo_t     *cr,
					 PangoLayout *layout,
 					 gint         sw,
 					 gint         sh,
 					 gint         lw,
 					 gint         lh,
 					 gint         posx,
 					 gint         posy,
 					 gint         angle,
 					 gchar	  	  *filename,
 					 gdouble      progress,
                     gdouble     *font_color,
                     gdouble     *font_brdr_color,
                     gdouble     *font_bgcolor,
                     gdouble     *border_color,
                     gboolean	top_border,
                     gboolean	bottom_border,
                    gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         posx * progress + sw * ( 1 - progress ),
                         posy,
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}

static void
img_text_from_top( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar	    *filename,
				   gdouble      progress,
                   gdouble     *font_color,
                   gdouble     *font_brdr_color,
                   gdouble     *font_bgcolor,
                   gdouble     *border_color,
                   gboolean	top_border,
                   gboolean	bottom_border,
                    gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         posx,
                         posy * progress - lh * ( 1 - progress ),
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}

static void
img_text_from_bottom( cairo_t     *cr,
					  PangoLayout *layout,
					  gint         sw,
					  gint         sh,
  					  gint         lw,
  					  gint         lh,
  					  gint         posx,
  					  gint         posy,
  					  gint         angle,
  					  gchar	  	  *filename,
  					  gdouble      progress,
                      gdouble     *font_color,
                      gdouble     *font_brdr_color,
                      gdouble     *font_bgcolor,
                      gdouble     *border_color,
                      gboolean	top_border,
                      gboolean	bottom_border,
					  gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         posx,
                         posy * progress + sh * ( 1 - progress ),
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                        border_color, top_border, bottom_border, border_width);
}

static void
img_text_grow( cairo_t     *cr,
			   PangoLayout *layout,
			   gint         sw,
			   gint         sh,
			   gint         lw,
			   gint         lh,
			   gint         posx,
			   gint         posy,
			   gint         angle,
			   gchar	    *filename,
			   gdouble      progress,
               gdouble     *font_color,
               gdouble     *font_brdr_color,
               gdouble     *font_bgcolor,
               gdouble     *border_color,
			   gboolean	top_border,
               gboolean	bottom_border,
			   gint		border_width)
{
	cairo_translate( cr, posx + lw * 0.5, posy + lh * 0.5 );
	cairo_scale( cr, progress, progress );

    img_text_draw_layout(cr, layout,
                         - lw * 0.5,
                         - lh * 0.5,
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}

static void
img_text_bottom_to_top( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar	   *filename,
				   gdouble      progress,
                   gdouble     *font_color,
                   gdouble     *font_brdr_color,
                   gdouble     *font_bgcolor,
                   gdouble     *border_color,
                    gboolean	top_border,
                    gboolean	bottom_border,
					gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         posx,
                         sh * (1 - progress) - lh * progress,
                         angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}

static void
img_text_right_to_left( cairo_t     *cr,
				   PangoLayout *layout,
				   gint         sw,
				   gint         sh,
				   gint         lw,
				   gint         lh,
				   gint         posx,
				   gint         posy,
				   gint         angle,
				   gchar	  *filename,
				   gdouble      progress,
                   gdouble     *font_color,
                   gdouble     *font_brdr_color,
                   gdouble     *font_bgcolor,
                   gdouble     *border_color,
                   gboolean	top_border,
                   gboolean	bottom_border,
				   gint		border_width)
{
    img_text_draw_layout(cr, layout,
                         sw * (1 - progress) - lw * progress,
                         posy, angle,
                         filename,
                         font_color, font_brdr_color, font_bgcolor,
                         border_color, top_border, bottom_border, border_width);
}