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

#ifndef __SLIDE_H__
#define __SLIDE_H__

#include "imagination.h"

ImgSlide *
img_slide_new( ImgSlideType type );

ImgStopPoint *
img_stop_point_copy( ImgStopPoint *point );

void
img_stop_point_free( ImgStopPoint *point );

ImgSubtitle *
img_subtitle_copy( ImgSubtitle *sub );

void
img_subtitle_free( ImgSubtitle *sub );

ImgSlide *
img_slide_copy( ImgSlide *slide );

void
img_slide_free( ImgSlide *slide );

gboolean
img_slide_set_file_info( ImgSlide    *slide,
						 gchar const *o_filename,
						 gchar const *r_filename,
						 ImgAngle     angle );

gboolean
img_slide_get_file_info( ImgSlide     *slide,
						 gchar const **o_filename,
						 gchar const **r_filename,
						 ImgAngle     *angle,
						 gchar const **resolution,
						 gchar const **image_type );

gboolean
img_slide_set_gradient_info( ImgSlide       *slide,
							 gint            gradient,
							 ImgColor const *start_color,
							 ImgColor const *stop_color,
							 ImgPoint const *start_point,
							 ImgPoint const *stop_point );

gboolean
img_slide_get_gradient_info( ImgSlide *slide,
							 gint     *type,
							 ImgColor *start_color,
							 ImgColor *stop_color,
							 ImgPoint *start_point,
							 ImgPoint *stop_point );

gboolean
img_slide_set_still_info( ImgSlide *slide,
						  gdouble   duration );

gboolean
img_slide_get_still_info( ImgSlide *slide,
						  gdouble  *duration );

gboolean
img_slide_set_transition_info( ImgSlide    *slide,
							   gchar const *path,
							   gint         id,
							   ImgRender    render,
							   gdouble      duration );

gboolean
img_slide_get_transition_info( ImgSlide     *slide,
							   gchar const **path,
							   gint         *id,
							   ImgRender    *render,
							   gdouble      *duration );

void
img_slide_set_ken_burns_info( ImgSlide *slide,
							  gint      cur_point,
							  gsize     length,
							  gdouble  *points );

ImgStopPoint *
img_slide_get_nth_stop_point( ImgSlide *slide,
							  gint      index );

gboolean
img_slide_get_ken_burns_info( ImgSlide  *slide,
							  GList    **points,
							  gint      *no_points,
							  gint      *cur_point );

void
img_slide_set_subtitle_info( ImgSlide          *slide,
							 gchar const       *subtitle,
							 gint	            anim_id,
							 TextAnimationFunc  anim,
							 gdouble            anim_duration,
							 ImgSubPos          position,
							 ImgRelPlacing      placing,
							 const gchar       *font_desc,
							 ImgColor          *font_color );

gboolean
img_slide_get_subtitle_info( ImgSlide          *slide,
							 gchar const      **subtitle,
							 gint              *anim_id,
							 TextAnimationFunc *anim,
							 gdouble           *anim_duration,
							 ImgSubPos         *position,
							 ImgRelPlacing     *placing,
							 gchar            **font_desc,
							 ImgColor          *font_color );

gdouble
img_slide_calc_duration_from_points( GList *list,
									 gint   length );

gdouble
img_slide_calc_duration_from_subtitles( GList *list,
										gint   length );

gboolean
img_slide_sync_timings( ImgSlide *slide );

#endif /* __SLIDE_HH_ */
