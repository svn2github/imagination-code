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

#ifndef __IMAGINATION_SUPPORT_H
#define __IMAGINATION_SUPPORT_H

#define PLUGINS_INSTALLED 0

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "imagination.h"
#include "slide.h"
#include "sexy-icon-entry.h"
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
void img_show_file_chooser(SexyIconEntry *, SexyIconEntryPosition, int, img_window_struct *);
void img_select_nth_slide(img_window_struct *, gint);

gboolean
img_set_total_slideshow_duration( img_window_struct *img );

gboolean
img_scale_image( const gchar      *filename,
				 gint              s_width,
				 gint              s_height,
				 gint              min_width,
				 gint              min_height,
				 gboolean          distort,
				 ImgColor const    *color,
				 GdkPixbuf       **pixbuf,
				 cairo_surface_t **surface );

void
img_set_project_mod_state( img_window_struct *img,
						   gboolean           modified );

GdkPixbuf *
img_convert_surface_to_pixbuf( cairo_surface_t *surface );

gboolean
img_scale_gradient( gint              gradient,
					ImgPoint         *p_start,
					ImgPoint         *p_stop,
					ImgColor         *c_start,
					ImgColor         *c_stop,
					gint              width,
					gint              height,
					gint              video_width,
					gint              video_height,
					GdkPixbuf       **pixbuf,
					cairo_surface_t **surface );

cairo_surface_t *
img_create_preview_image( ImgSlide *slide,
						  gint      video_width,
						  gint      video_height,
						  gboolean  low_quality,
						  gboolean  distort,
						  ImgColor *background );

GdkPixbuf *
img_create_thumbnail_image( ImgSlide *slide,
							gint      video_width,
							gint      video_height,
							gboolean  distort,
							ImgColor *background );
#endif
