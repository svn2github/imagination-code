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
 
#ifndef __IMAGINATION_H__
#define __IMAGINATION_H__

#include <stdlib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

/* Timeout (in miliseconds) for transition effects */
#define TRANSITION_TIMEOUT 15

#define	FAST	0.05
#define	NORMAL	0.01
#define	SLOW	0.002

typedef struct _plugin plugin;

struct _plugin
{
	const gchar	*name;			/* The name of the transition */
	gpointer	address;		/* The mem address of the routine */
};

typedef struct _slide_struct slide_struct;

struct _slide_struct
{
	gchar	*filename;
	guint	duration;
	gdouble	speed;
	gchar	*resolution;
	gchar	*type;
	void (*render) (GdkDrawable*, GdkPixbuf*, GdkPixbuf*, gdouble);
};

typedef struct _img_window_struct img_window_struct;

struct _img_window_struct
{
	GtkWidget	*imagination_window;
	GtkWidget	*remove_menu;
	GtkWidget	*remove_button;
	GtkWidget	*preview_menu;
	GtkWidget 	*preview_button;
	GtkWidget	*transition_type;
	GtkWidget	*duration;
	GtkWidget	*trans_duration;
	GtkWidget	*slide_selected_data;
	GtkWidget	*type_data;
	GtkWidget	*total_time_data;
	GtkWidget	*filename_data;
	GtkWidget	*resolution_data;	
  	GtkWidget	*thumbnail_iconview;
  	GtkWidget	*statusbar;
  	GtkWidget	*viewport;
  	GtkWidget	*image_area;
  	GdkPixbuf	*slide_pixbuf;
  	GdkPixbuf	*pixbuf1;
  	GdkPixbuf	*pixbuf2;
  	GtkWidget	*import_slide_chooser;
	GtkWidget	*dim_label;
	GtkWidget	*size_label;
  	GtkWidget	*preview_image;
  	GtkWidget	*goto_window;
  	GtkListStore *thumbnail_model;
  	slide_struct *current_slide;
  	gchar		*current_dir;
  	gchar		*slideshow_title;
  	GtkTreeIter *cur_ss_iter;
  	GSList		*plugin_list;
  	gint		slides_nr;
  	gint		slideshow_height;
  	gint		total_secs;
  	gint		nr_transitions_loaded;
  	guint		context_id;
  	guint		source_id;
  	gboolean	preview_is_running;
	gdouble     progress;
};

#endif
