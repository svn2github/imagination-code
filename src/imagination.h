/*
 *  Copyright (c) 2009 Giuseppe Torelli <colossus73@gmail.com>
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
 
#ifndef __IMAGINATION_H__
#define __IMAGINATION_H__

#include <stdlib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

/* Timeout (in miliseconds) for transition effects. I increased this value
 * to 40 to obtain 25 fps animation, which should be drawn within the same
 * time frame on any computer (67 fps is too intensive for even most
 * powerfull machines to handle). */
#define TRANSITION_TIMEOUT 40

/* Increments are modified to accomodate 25 fps animation set above.
 * With new lover fps anmation, we can be pretty sure that animation time
 * will be fixed and approximatelly the same on all computers. */
#define	FAST	0.04	/*  25 frames - 1 s */
#define	NORMAL	0.01	/* 100 frames - 4 s */
#define	SLOW	0.005	/* 200 frames - 8 s */

#define comment_string	"Imagination 1.0 Slideshow Project - http://imagination.sf.net"

typedef struct _plugin plugin;

struct _plugin
{
	const gchar	*name;		/* The name of the transition */
	gpointer	address;	/* The mem address of the routine */
};

typedef struct _slide_struct slide_struct;

struct _slide_struct
{
	gchar	*filename;
	guint	duration;
	gdouble	speed;
	gchar	*resolution;
	gchar	*type;
	void	(*render) (GdkDrawable*, GdkPixbuf*, GdkPixbuf*, gdouble, gint);
	gint    combo_transition_type_index;
};

typedef struct _img_window_struct img_window_struct;

struct _img_window_struct
{
	/* Main GUI related variables */
	GtkWidget	*imagination_window;	// Main window
	GtkWidget	*open_menu;
	GtkWidget	*save_menu;
	GtkWidget	*save_as_menu;
	GtkWidget	*properties_menu;
	GtkWidget	*close_menu;
	GtkWidget	*open_button;
	GtkWidget	*save_button;
	GtkWidget	*import_menu;
	GtkWidget	*remove_menu;
	GtkWidget	*import_button;
	GtkWidget	*remove_button;
	GtkWidget	*preview_menu;
	GtkWidget 	*preview_button;
	GtkWidget   *export_menu;
	GtkWidget   *export_button;
	GtkWidget	*transition_type;
	GtkWidget	*duration;				// Duration spin button
	GtkWidget	*trans_duration;
	GtkWidget	*slide_selected_data;
	GtkWidget	*type_data;
	GtkWidget	*total_time_data;
	GtkWidget	*filename_data;
	GtkWidget	*resolution_data;
	GtkWidget	*thumb_scrolledwindow;
  	GtkWidget	*thumbnail_iconview;
  	GtkWidget	*statusbar;
  	GtkWidget	*progress_bar;
  	GtkWidget	*viewport;
  	GtkWidget	*image_area;
  	guint		context_id;
  	GtkListStore *thumbnail_model;
  	gchar		*current_dir;

	/* Import slides dialog variables */
	GtkWidget	*dim_label;
	GtkWidget	*size_label;
  	GtkWidget	*preview_image;

	/* Renderers and module stuff */
  	gint		nr_transitions_loaded;
  	GSList		*plugin_list;

	/* Project related variables */
	gchar       *project_filename;		// project name for saving
  	gchar		*slideshow_filename;	// exported file name
  	gchar		*aspect_ratio;
	gboolean     distort_images;
	gint		slideshow_format_index;
	guint32     background_color;
  	gint		total_secs;
  	gint		slides_nr;

	/* Variables common to export and preview functions */
  	slide_struct *current_slide;
  	GdkPixbuf	*slide_pixbuf;
  	GdkPixbuf	*pixbuf1;
  	GdkPixbuf	*pixbuf2;
  	GtkTreeIter *cur_ss_iter;
  	guint		source_id;
	gdouble     progress;

	/* Preview related variables */
  	gboolean	preview_is_running;
  	GtkWidget	*import_slide_chooser;
  	GtkWidget	*slide_number_entry;

	/* Export dialog related stuff */
	gboolean    export_is_running;
	gint        file_desc;
	guchar      *pixbuf_data;
	GtkWidget   *export_pbar1;
	GtkWidget   *export_pbar2;
	GtkWidget   *export_label;
	GtkWidget   *export_dialog;
	guint        export_frame_nr;	// Total number of frames
	guint        export_frame_cur;	// Current frame
	guint        export_slide_nr;	// Number of frames fo current slide
	guint        export_slide_cur;	// Current slide frame
	guint        export_slide;		// Number of slide being exported
};

#endif
