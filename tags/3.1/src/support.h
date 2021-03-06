/*
 *  Copyright (c) 2009 Giuseppe Torelli <colossus73@gmail.com>
 *  Copyright (c) 2011 Robert Chéramy   <robert@cheramy.net>
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

#ifndef __IMAGINATION_SUPPORT_H
#define __IMAGINATION_SUPPORT_H

#define PLUGINS_INSTALLED 1

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "imagination.h"
#include "imgcellrendereranim.h"

#ifdef ENABLE_NLS
#  include <glib/gi18n.h>
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define Q_(String) g_strip_context ((String), (String))
#  define N_(String) (String)
#endif

GtkWidget *img_load_icon(gchar *, GtkIconSize );
gchar *img_convert_seconds_to_time(gint );
GtkWidget *_gtk_combo_box_new_text(gint);
void img_set_statusbar_message(img_window_struct *, gint);
void img_load_available_transitions(img_window_struct *);
void img_show_file_chooser(GtkWidget *, GtkEntryIconPosition, int, img_window_struct *);
void img_select_nth_slide(img_window_struct *, gint);

slide_struct *
img_create_new_slide( void );

void
img_set_slide_file_info( slide_struct *slide,
						 const gchar  *filename );

void
img_set_slide_gradient_info( slide_struct *slide,
							 gint          gradient,
							 gdouble      *start_color,
							 gdouble      *stop_color,
							 gdouble      *start_point,
							 gdouble      *stop_point );

void
img_set_slide_still_info( slide_struct      *slide,
						  gdouble           duration,
						  img_window_struct *img );

void
img_set_slide_transition_info( slide_struct      *slide,
							   GtkListStore      *store,
							   GtkTreeIter       *iter,
							   GdkPixbuf         *pix,
							   const gchar       *path,
							   gint               transition_id,
							   ImgRender          render,
							   guint              speed,
							   img_window_struct *img );

void
img_set_slide_ken_burns_info( slide_struct *slide,
							  gint          cur_point,
							  gsize         length,
							  gdouble      *points );

void
img_set_slide_text_info( slide_struct      *slide,
						 GtkListStore      *store,
						 GtkTreeIter       *iter,
						 const gchar       *subtitle,
						 gint	            anim_id,
						 gint               anim_duration,
						 gint               position,
						 gint               placing,
						 const gchar       *font_desc,
						 gdouble           *font_color,
                         gdouble           *font_bgcolor,
						 img_window_struct *img );

void img_free_slide_struct( slide_struct * );


gboolean
img_set_total_slideshow_duration( img_window_struct *img );

gint
img_calc_slide_duration_points( GList *list,
								gint   length );

gboolean
img_scale_image( const gchar      *filename,
				 gdouble           ratio,
				 gint              width,
				 gint              height,
				 gboolean          distort,
				 gdouble          *color,
				 GdkPixbuf       **pixbuf,
				 cairo_surface_t **surface );

void
img_set_project_mod_state( img_window_struct *img,
						   gboolean           modified );

void
img_sync_timings( slide_struct      *slide,
				  img_window_struct *img );

GdkPixbuf *
img_convert_surface_to_pixbuf( cairo_surface_t *surface );

gboolean
img_scale_gradient( gint              gradient,
					gdouble          *p_start,
					gdouble          *p_stop,
					gdouble          *c_start,
					gdouble          *c_stop,
					gint              width,
					gint              height,
					GdkPixbuf       **pixbuf,
					cairo_surface_t **surface );

void
img_message (img_window_struct *img,
             gboolean alert_user,
             gchar *message, ...);

void
img_check_for_encoder(img_window_struct *img);

#endif
