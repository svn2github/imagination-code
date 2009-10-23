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

/* Create new slide and fill it with default values (those depend on the type of
 * the slide being created) */
ImgSlide *
img_slide_new( ImgSlideType type )
{
	slide = g_slice_new0( ImgSlide );
	if( ! slide )
		return( NULL );

	/* Populate slide with default values */
	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_PSEUDO:
			{
				ImgSlidePseudo *file = (ImgSlidePseudo *)slide;
				
				/* Capabilities */
				file->caps = IMG_SLIDE_PSEUDO_CAPS;

				/* Transition */
				file->path = g_strdup( "0" );
				file->transition_id = -1;
				file->trans_suration = NORMAL;
			}
			break;

		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;
				
				/* Capabilities */
				file->caps = IMG_SLIDE_FILE_CAPS;

				/* Still part */
				file->still_duration = 1.0;
				
				/* Transition */
				file->path = g_strdup( "0" );
				file->transition_id = -1;
				file->trans_duration = NORMAL;
				
				/* Ken Burns */
				file->cur_point = -1;

				/* Subtitles */
				file->cur_sub = -1;
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				/* Capabilities */
				grad->caps = IMG_SLIDE_GRADIENT_CAPS;

				/* Still part */
				grad->still_duration = 1.0;
				
				/* Transition */
				grad->path = g_strdup( "0" );
				grad->transition_id = -1;
				grad->speed = NORMAL;
				
				/* Ken Burns */
				grad->cur_point = -1;

				/* Subtitles */
				grad->cur_sub = -1;
			}
			break;

		case IMG_SLIDE_TYPE_VIDEO:
			{
				ImgSlideVideo *video = (ImgSlideVideo *)slide;

				/* Capabilities */
				video->caps = IMG_SLIDE_VIDEO_CAPS;
			}
			break;
	}

	return( slide );
}

ImgStopPoint *
img_stop_point_copy( ImgStopPoint *point )
{
	return( g_slice_dup( ImgStopPoint, point ) );
}

void
img_stop_point_free( ImgStopPoint *point )
{
	g_slice_free( ImgStopPoint, point );
}

ImgSubtitle *
img_subtitle_copy( ImgSubtitle *sub )
{
	ImgSubtitle *new;

	new = g_slice_dup( ImgSubtitle, sub );
	new->text = g_strdup( sub->text );
	new->font_desc = pango_font_description_copy( sub->font_desc );

	return( new );
}

void
img_subtitle_free( ImgSubtitle *sub )
{
	if( sub->text )
		g_free( sub->text );
	pango_font_description_free( sub->font_desc );
	g_slice_free( ImgSubtitle, sub );
}

ImgSlide *
img_slide_copy( ImgSlide *slide )
{
	ImgSlide *new;

	/* Copy slide */
	new = g_slice_dup( ImgSlide, slide );
	if( ! new )
		return( NULL );

	/* Type specific copy routines */
	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_PSEUDO:
			{
				ImgSlidePseudo *from = (ImgSlidePseudo *)slide,
							   *to   = (ImgSlidePseudo *)new;

				to->path = g_strdup( from->path );
			}
			break;

		case IMG_SLIDE_TYPE_FILE:
			{
				GList        *tmp;
				ImgSlideFile *from = (ImgSlideFile *)slide,
							 *to   = (ImgSlideFile *)new;

				to->resolution = g_strdup( from->resolution );
				to->image_type = g_strdup( from->image_type );
				to->o_filename = g_strdup( from->o_filename );
				to->r_filename = g_strdup( from->r_filename );
				to->path       = g_strdup( from->path );

				to->points = NULL;
				for( tmp = from->points; tmp; tmp = g_list_next( tmp ) )
				{
					ImgStopPoint *point;

					point = img_stop_point_copy( (ImgStopPoint *)tmp->data );
					to->points = g_list_append( to->points, point );
				}

				to->subs = NULL;
				for( tmp = from->subs; tmp; tmp = g_list_next( tmp ) )
				{
					ImgSubtitle *sub;

					sub = img_subtitle_copy( (ImgSubtitle *)tmp->data );
					to->subs = g_list_append( to->subs, sub );
				}
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				GList        *tmp;
				ImgSlideGradient *from = (ImgSlideGradient *)slide,
								 *to   = (ImgSlideGradient *)new;

				to->path       = g_strdup( from->path );

				to->points = NULL;
				for( tmp = from->points; tmp; tmp = g_list_next( tmp ) )
				{
					ImgStopPoint *point;

					point = img_stop_point_copy( (ImgStopPoint *)tmp->data );
					to->points = g_list_append( to->points, point );
				}

				to->subs = NULL;
				for( tmp = from->subs; tmp; tmp = g_list_next( tmp ) )
				{
					ImgSubtitle *sub;

					sub = img_subtitle_copy( (ImgSubtitle *)tmp->data );
					to->subs = g_list_append( to->subs, sub );
				}
			}
			break;
	}

	return( new );
}

void
img_slide_free( ImgSlide *slide )
{
	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;
				GList        *tmp;

				if( file->angle != ANGLE_0 )
					g_unlink( file->r_filename );
				if( file->o_filename )
				{
					g_free( file->o_filename );
					g_free( file->r_filename );
					g_free( file->resolution );
					g_free( file->imge_type );
				}
				g_free( file->path );
				
				for( tmp = file->points; tmp; tmp = g_list_next( tmp ) )
					img_stop_point_free( (ImgStopPoint *)tmp->data );
				g_list_free( file->points );

				for( tmp = file->subs; tmp; tmp = g_list_next( tmp ) )
					img_subtitle_free( (ImgSubtitle *)tmp->data );
				g_list_free( file->subs );
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *file = (ImgSlideGradient *)slide;
				GList            *tmp;

				g_free( file->path );
				
				for( tmp = file->points; tmp; tmp = g_list_next( tmp ) )
					img_stop_point_free( (ImgStopPoint *)tmp->data );
				g_list_free( file->points );

				for( tmp = file->subs; tmp; tmp = g_list_next( tmp ) )
					img_subtitle_free( (ImgSubtitle *)tmp->data );
				g_list_free( file->subs );

			}
			break;

		case IMG_SLIDE_TYPE_VIDEO:
			{
				ImgSlideVideo *video = (ImgSlideVideo *)slide;
			}
			break;
	}

	g_slice_free( ImgSlide, slide );
}

gboolean
img_slide_set_file_info( ImgSlide    *slide,
						 gchar const *filename )
{
	GdkPixbufFormat *format;
	gint             width,
					 height;
	ImgSlideFile    *file;

	g_return_val_if_fail( IMG_SLIDE_IS_TYPE_FILE( slide ), FALSE );
	g_return_val_if_fail( filename != NULL, FALSE );

	file = (ImgSlideFile *)slide;

	format = gdk_pixbuf_get_file_info( filename, &width, &height );

	file->o_filename = g_strdup( filename );
	file->r_filename = g_strdup( filename );
	file->angle = ANGLE_0;
	
	file->resolution = g_strdup_printf( "%d x %d", width, height );
	file->image_type = gdk_pixbuf_format_get_name( format );

	return( TRUE );
}

gboolean
img_slide_get_file_info( ImgSlide     *slide,
						 gchar const **filename,
						 ImgAngle     *angle,
						 gchar const **resolution,
						 gchar const **image_type )
{
	gboolean ret;

	switch( IMG_SLIDE_GET_TYPE )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;
				
				if( filename )
					*filename = file->o_filename;
				
				if( angle )
					*angle = file->angle;
				
				if( resolution )
					*resolution = file->resolution;

				if( image_type )
					*image_type = file->image_type;
			}
			ret = TRUE;
			break;

		default:
			ret= FALSE;
			break;
	}

	return( ret );
}

gboolean
img_slide_set_gradient_info( ImgSlide       *slide,
							 gint            gradient,
							 ImgColor const *start_color,
							 ImgColor const *stop_color,
							 ImgColor const *start_point,
							 ImgColor const *stop_point )
{
	ImSlideGradient *grad;

	g_return_val_if_fail(
			IMG_SLIDE_GET_TYPE( slide ) == IMG_SLIDE_TYPE_GRADIENT, FALSE );

	grad = (ImgSlideGradient *)slide;

	if( gradient > -1 )
		grad->gradient = gradient;

	if( start_color )
		grad->start_color = *start_color;

	if( stop_color )
		grad->stop_color = *stop_color;

	if( start_point )
		grad->start_point = *start_point;

	if( stop_point )
		grad->stop_point = *stop_point;

	return( TRUE );
}

gboolean
img_slide_get_gradient_info( ImgSlide *slide,
							 gint     *type,
							 ImgColor *start_color,
							 ImgColor *stop_color,
							 ImgPoint *start_point,
							 ImgPoint *stop_point )
{
	ImgSlideGradient *grad;

	g_return_val_if_fail(
			IMG_SLIDE_GET_TYPE( slide ) == IMG_SLIDE_TYPE_GRADIENT, FALSE );

	grad = (ImgSlideGradient *)slide;

	if( type )
		*type = grad->gradient;

	if( start_color )
		*start_color = grad->start_color;
	
	if( stop_color )
		*stop_color = grad->stop_color;
	
	if( start_point )
		*start_point = grad->start_point;
	if( stop_point )
		*stop_point = grad->stop_point;

	return( TRUE );
}

gboolean
img_slide_set_still_duration( ImgSlide *slide,
							  gdouble   duration )
{
	g_return_val_if_fail( slide->caps & IMG_SLIDE_CAP_DURATION, FALSE );

	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;
				
				file->still_duration = duration;
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;
				
				grad->still_duration = duration;
			}
			break;
	}

	return( TRUE );
}

gboolean
img_slide_get_still_duration( ImgSlide *slide,
							  gdouble  *duration )
{
	gboolean ret;

	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;

				*duration = file->still_duration;
			}
			ret = TRUE;
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				*duration = grad->still_duration;
			}
			ret = TRUE;
			break;

		default:
			ret = FALSE;
			break;
	}
	
	return( ret );
}

gboolean
img_slide_set_transition_info( ImgSlide    *slide,
							   gchar const *path,
							   gint         id,
							   ImgRender    render,
							   gdouble      duration )
{
	g_return_val_if_fail( slide->caps & IMG_SLIDE_CAP_TRANSITION, FALSE );

	switch( IMG_GET_SLIDE_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_PSEUDO:
			{
				ImgSlidePseudo *file = (ImgSlidePseudo *)slide;

				if( path && ( file->transition_id != id ) )
				{
					if( file->path )
						g_free( file->path );

					file->path = g_strdup( path );
					grad->transition_id = id;
					grad->render = render;
				}

				if( duration >= 0.0 && file->trans_duration != duration )
					file->trans_duration = duration;
			}
			break;

		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;

				if( path && ( file->transition_id != id ) )
				{
					if( file->path )
						g_free( file->path );

					file->path = g_strdup( path );
					grad->transition_id = id;
					grad->render = render;
				}

				if( duration >= 0.0 && file->trans_duration != duration )
					file->trans_duration = duration;
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				if( path && ( grad->transition_id != id ) )
				{
					if( grad->path )
						g_free( grad->path );

					grad->path = g_strdup( path );
					grad->transition_id = id;
					grad->render = render;
				}

				if( duration >= 0.0 && file->trans_duration != duration )
					grad->trans_duration = duration;
			}
			break;
	}

	return( TRUE );
}

gboolean
img_slide_get_transition_info( ImgSlide      *slide,
							   gchar * const *path,
							   gint          *id,
							   ImgRender     *render,
							   gdouble       *duration )
{
	g_return_val_if_fail( slide->caps & IMG_SLIDE_CAP_TRANSITION, FALSE );

	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_PSEUDO:
			{
				ImgSlidePseudo *file = (ImgSlidePseudo *)slide;

				if( path )
					*path = file->path;

				if( id )
					*id = file->transition_id;

				if( render )
					*render = file->render;

				if( duration )
					*duration = file->trans_duration;
			}
			break;

		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;

				if( path )
					*path = file->path;

				if( id )
					*id = file->transition_id;

				if( render )
					*render = file->render;

				if( duration )
					*duration = file->trans_duration;
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				if( path )
					*path = grad->path;

				if( id )
					*id = grad->transition_id;

				if( render )
					*render = grad->render;

				if( duration )
					*duration = grad->trans_duration;
			}
			break;
	}
}

void
img_slide_set_ken_burns_info( ImgSlide *slide,
							  gint      cur_point,
							  gsize     length,
							  gdouble  *points )
{
	GList        *tmp = NULL,
				 *free;
	ImgStopPoint *point;
	gint          i,
				  counter = 0;
	gdouble       full;

	g_return_if_fail( slide->caps & IMG_SLIDE_CAP_KEN_BURNS );

	for( i = 0; i < length; i += 10 )
	{
		/* Create new point */
		point = g_slice_new( ImgStopPoint );

		/* Set values */
		point->center.x   = points[i + 0];
		point->center.y   = points[i + 1];
		point->z          = points[i + 2];
		point->b1.x       = points[i + 3];
		point->b1.y       = points[i + 4];
		point->b2.x       = points[i + 5];
		point->b2.y       = points[i + 6];
		point->still_time = points[i + 7];
		point->move_time  = points[i + 8];
		point->smooth     = ( points[i + 9] > 0 );
		
		/* Append it to the list */
		tmp = g_list_append( tmp, point );
		counter++;
	}

	full = img_slide_calc_duration_from_points( tmp, counter );
	if( full > 0.0 )
		img_slide_set_still_duration( slide, full );

	switch( IMG_GET_SLIDE_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;

				free = file->points;
				file->points = tmp;
				file->no_points = counter;
				file->cur_point = CLAMP( cur_point, -1, counter - 1 );
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				free = grad->points;
				grad->points = tmp;
				grad->no_points = counter;
				grad->cur_point = CLAMP( cur_point, -1, counter - 1 );
			}
			break;
	}

	if( free )
	{
		for( tmp = free; tmp; tmp = g_list_next( tmp ) )
			g_slice_free( ImgStopPoint, tmp->data );
		g_list_free( free );
	}
}

void
img_slide_set_subtitle_info( ImgSlide          *slide,
							 gchar const       *subtitle,
							 gint	            anim_id,
							 TextAnimationFunc  anim,
							 gdouble            anim_duration,
							 ImgSubPos          position,
							 ImgRelPlacing      placing,
							 const gchar       *font_desc,
							 ImgColor          *font_color )
{
	ImgSubtitle *sub;

	g_return_if_fail( slide->caps & IMG_SLIDE_CAP_SUBTITLE );

	switch( IMG_GET_SLIDE_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;

				if( file->no_subs )
					sub = file->subs->data;
				else
				{
					sub = g_slice_new( ImgSubtitle );
					file->subs = g_list_append( NULL, sub );
					img_subtitles_populate_default( sub );
				}
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *grad = (ImgSlideGradient *)slide;

				if( file->no_subs )
					sub = file->subs->data;
				else
				{
					sub = g_slice_new( ImgSubtitle );
					file->subs = g_list_append( NULL, sub );
					img_subtitles_populate_default( sub );
				}
			}
			break;

		default:
			break;
	}

	if( sub->text )
		g_free( sub->text );
	sub->text = g_strdup( subtitle );

	if( anim_id > -1 )
	{
		sub->anim = anim;
		sub->anim_id = anim_id;

		/* Sync timings */
		img_slide_sync_timings( slide );
	}

	if( anim_duration >= 0.0 )
	{
		slide->anim_duration = anim_duration;

		/* Synchronize timings */
		img_slide_sync_timings( slide );
	}

	if( position > -1 )
		sub->position = position;

	if( placing > -1 )
		sub->placing = placing;

	if( font_desc )
	{
		if( sub->font_desc )
			pango_font_description_free( slide->font_desc );
		sub->font_desc = pango_font_description_from_string( font_desc );
	}

	if( font_color )
		sub->font_color = *font_color;
}								

gdouble
img_slide_calc_duration_from_points( GList *list,
									 gint   length )
{
	GList        *tmp;
	gint          i;
	gdouble       duration = 0.0;
	ImgStopPoint *point;

	/* Calculate length */
	for( tmp = list, i = 0; i < length; tmp = g_list_next( tmp ), i++ )
	{
		/* FIXME: One move time needs to be removed: first or last, depending on
		 * the drawing model that will be implemented. */
		point = (ImgStopPoint *)tmp->data;
		duration += point->still_time + point->move_time;
	}

	return( duration );
}

gdouble
img_slide_calc_duration_from_subtitles( GList *list,
										gint   length )
{
	GList       *tmp;
	gint         i;
	gdouble      duration = 0.0;
	ImgSubtitle *sub;

	/* Calc */
	for( tmp = list; tmp; tmp = g_list_next( tmp ) )
	{
		sub = (ImgSubtitle *)tmp->data;
		duration += sub->anim_duration;
	}

	return( duration );
}

gboolean
img_slide_sync_timings( ImgSlide *slide )
{
	GList   *points,
			*subs;
	gint     no_points,
			 no_subs;
	gdouble  duration,
			 anim_duration;

	g_return_val_if_fail( slide->caps & IMG_SLIDE_CAP_SUBTITLE );

	switch( IMG_SLIDE_GET_TYPE( slide ) )
	{
		case IMG_SLIDE_TYPE_FILE:
			{
				ImgSlideFile *file = (ImgSlideFile *)slide;
				
				points    = file->points;
				subs      = file->subs;
				no_points = file->no_points;
				no_subs   = file->no_subs;
				duration  = file->still_duration;
			}
			break;

		case IMG_SLIDE_TYPE_GRADIENT:
			{
				ImgSlideGradient *file = (ImgSlideFile *)slide;
				
				points    = file->points;
				subs      = file->subs;
				no_points = file->no_points;
				no_subs   = file->no_subs;
				duration  = file->still_duration;
			}
			break;
	}

	anim_duration = img_slide_calc_duration_from_subtitles( subs, no_subs );

	/* If times are already synchronized, return */
	if( duration >= anim_duration )
		return;

	/* Do the right thing;) */
	if( no_points )
	{
		gdouble       diff;
		ImgStopPoint *point;

		/* Calculate difference that we need to accomodate */
		diff = anim_duration - duration;

		/* Elongate last point */
		point = (ImgStopPoint *)g_list_last( points )->data;
		point->still_time += diff;
	}

	img_slide_set_still_duration( slide, anim_duration );
}
