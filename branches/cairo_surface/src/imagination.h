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

/* Transition preview frame rate. I decided to use 25 fps, which
 * should be handled on time by most machines. */
#define PREVIEW_FPS 25

/* The transition speed is defined as a duration in seconds. */
#define	FAST	1
#define	NORMAL	4
#define	SLOW	8

#define old_comment_string \
	"Imagination Slideshow Project - http://imagination.sf.net"
#define comment_string \
	"Imagination 2.0 Slideshow Project - http://imagination.sf.net"


/* ****************************************************************************
 * Subtitles related definitions
 * ************************************************************************* */
/* Enum that holds all available positions of text. */
typedef enum
{
	IMG_SUB_POS_TOP_LEFT = 0,
	IMG_SUB_POS_TOP_CENTER,
	IMG_SUB_POS_TOP_RIGHT,
	IMG_SUB_POS_MIDDLE_LEFT,
	IMG_SUB_POS_MIDDLE_CENTER,
	IMG_SUB_POS_MIDDLE_RIGHT,
	IMG_SUB_POS_BOTTOM_LEFT,
	IMG_SUB_POS_BOTTOM_CENTER,
	IMG_SUB_POS_BOTTOM_RIGHT
}
ImgSubPos;

/* Enum with relative placings */
typedef enum
{
	IMG_REL_PLACING_EXPORTED_VIDEO = 0,
	IMG_REL_PLACING_ORIGINAL_IMAGE
}
ImgRelPlacing;

/*
 * TextAnimationFunc:
 * @cr: cairo context that should be used for drawing
 * @layout: PangoLayout to be rendered
 * @sw: surface width
 * @sh: surface height
 * @lw: layout width
 * @lh: layout height
 * @posx: final position (x coord)
 * @posy: final position (y coord)
 * @progress: progress of animation
 * @font_color: array of RGBA values
 *
 * This is prototype for subtitle animation function. It's task it to render
 * @layout to @cr according to  @progress, @posx and @posy.
 *
 * When @progress > 1, @layout should be drawn at (@posx, @posy).
 */
typedef void (*TextAnimationFunc)( cairo_t     *cr,
								   PangoLayout *layout,
								   gint         sw,
								   gint         sh,
								   gint         lw,
								   gint         lh,
								   gint         posx,
								   gint         posy,
								   gdouble      progress,
								   gdouble     *font_color );


/* ****************************************************************************
 * Transition related definitions
 * ************************************************************************* */
/* Prototype of transition renderer */
typedef void (*ImgRender)( cairo_t *,
						   cairo_surface_t *,
						   cairo_surface_t *,
						   gdouble );

/* ****************************************************************************
 * Ken Burns effect related definitions
 * ************************************************************************* */
typedef struct _ImgStopPoint ImgStopPoint;
struct _ImgStopPoint
{
	gint    time; /* Duration of this stop point */
	gint    offx; /* X and Y offsets of zoomed image */
	gint    offy;
	gdouble zoom; /* Zoom level */
};

/* ****************************************************************************
 * Common definitions that are used all over the place
 * ************************************************************************* */
typedef struct _slide_struct slide_struct;
struct _slide_struct
{
	gchar	*filename;
	gchar	*slide_original_filename;
	guint	duration;
	guint  	speed;
	gchar	*resolution;
	gchar	*type;
	gchar   *path;	/* transition model string path representation */
	gint     transition_id;
	ImgRender render;

	/* Ken Burns effect variables */
	GList  *points;    /* List with stop points */
	gint    no_points; /* Number of stop points in list */
	gint    cur_point; /* Currently active stop point */

	/* Subtitle variables */
	gchar                *subtitle;      /* Subtitle text */
	gboolean              has_subtitle;  /* Does slide has subtitle */
	TextAnimationFunc     anim;          /* Animation functions */
	gint                  anim_id;       /* Animation id */
	gint                  anim_duration; /* Duration of animation */
	ImgSubPos             position;      /* Final position of subtitle */
	ImgRelPlacing         placing;       /* Relative placing */
	PangoFontDescription *font_desc;     /* Font description */
	gdouble               font_color[4]; /* Font color (RGBA format) */
};

typedef struct _img_window_struct img_window_struct;
struct _img_window_struct
{
	/* Main GUI related variables */
	GtkWidget	*imagination_window;	// Main window
	GtkWidget	*open_menu;
	GtkWidget	*save_menu;
	GtkWidget	*save_as_menu;
	GtkWidget	*open_button;
	GtkWidget	*save_button;
	GtkWidget   *rotate_left_menu;
	GtkWidget   *rotate_right_menu;
	GtkWidget   *rotate_left_button;
	GtkWidget   *rotate_right_button;
	GtkWidget	*preview_menu;
	GtkWidget 	*preview_button;
	GtkWidget	*transition_type;
	GtkWidget	*random_button;
	GtkWidget	*duration;				// Duration spin button
	GtkWidget	*stop_point_duration;
	GtkWidget	*trans_duration;
	GtkWidget	*total_time_data;
	GtkWidget	*filename_data;
	GtkTextBuffer *slide_text_buffer;
	GtkWidget	*text_animation_combo;
	GtkWidget   *font_button;
	GtkWidget	*scrolled_win;
	GtkWidget	*expand_button;
  	GtkWidget	*thumbnail_iconview;
  	GtkWidget	*statusbar;
  	GtkWidget	*progress_bar;
  	GtkWidget	*image_area;
  	guint		context_id;
  	GtkListStore *thumbnail_model;
  	gchar		*current_dir;
	GSList		*rotated_files;

	/* Import slides dialog variables */
	GtkWidget	*dim_label;
	GtkWidget	*size_label;
  	GtkWidget	*preview_image;

	/* Current image position parameters */
	GtkWidget    *zoom_scale;    /* Zoom slider */
	gint          x;             /* Last button press coordinates */
	gint          y;
	gint          bak_offx;      /* Stored offset at button press */
	gint          bak_offy;
	gint          maxoffx;       /* Maximal offsets for current zoom */
	gint          maxoffy;
	ImgStopPoint  current_point; /* Data for rendering current image */
  	slide_struct *current_slide;
	
	/* Subtitle update */
	gint subtitle_update_id;

	/* Renderers and module stuff */
  	gint		nr_transitions_loaded;
  	GSList		*plugin_list;

	/* Project related variables */
	gchar       *project_filename;		// project name for saving
	gboolean	distort_images;
	gboolean	project_is_modified;
	gint        video_size[2];
	gdouble     background_color[3];
  	gint		total_secs;
	gint		total_music_secs;
  	gint		slides_nr;
	slide_struct final_transition;  /* Only speed, render and duration fields
									   of this structure are used (and duration
									   is always 0). */

	/* Variables common to export and preview functions */
	slide_struct    *work_slide;
	cairo_surface_t *current_image;      /* Image in preview area */
	cairo_surface_t *exported_image;     /* Image being exported */
	cairo_surface_t *image1;             /* Images used in transition rendering */
	cairo_surface_t *image2;
	cairo_surface_t *image_from;
	cairo_surface_t *image_to;
	ImgStopPoint    *point1;             /* Last stop point of image1 */
	ImgStopPoint    *point2;             /* First stop point of image2 */
  	GtkTreeIter     *cur_ss_iter;
  	guint		     source_id;
	gdouble          progress;           /* This will be DEPRECATED */

	guint            total_nr_frames;    /* Total number of frames */
	guint            displayed_frame;    /* Current frame */
	guint            slide_nr_frames;    /* Number of frames fo current slide */
	guint            slide_cur_frame;    /* Current slide frame */
	guint            slide_trans_frames; /* Number of frames in transition */
	guint            slide_still_frames; /* Number of frames in still part */
	guint            next_slide_off;     /* Time offset of next slide */

	gint   still_counter; /* Currently displayed still frame */
	gint   still_max;     /* Number of frames per stop point */
	gint   still_offset;  /* Offset in seconds for next stop point */
	guint  still_cmlt;    /* Cumulative number of still frames */
	GList *cur_point;     /* Current stop point */

	/* Preview related variables */
  	gboolean	preview_is_running;
  	GtkWidget	*import_slide_chooser;
	GtkWidget	*current_stop_point_entry;
	GtkWidget	*total_stop_points_label;
  	GtkWidget	*total_slide_number_label;
	GtkWidget	*slide_number_entry;

	/* Export dialog related stuff */
	gint        export_is_running;  /* 0 - export is not running
									     . no cleaning needed
									   1 - geting info from user
									     . no cleaning needed
									   2 - preparing audio
									     . terminate sox thread
										 . delete any created files
										 . free cmd_line
										 . free audio_file
									   3 - exporting video (pre-spawn)
									     . free cmd_line
										 . free audio_file
									   4 - exporting video (post-spawn)
									     . kill ffmpeg
										 . free cmd_line
										 . free audio_file
										 */
	gint        file_desc;
	GtkWidget   *export_pbar1;
	GtkWidget   *export_pbar2;
	GtkWidget   *export_label;
	GtkWidget   *export_dialog;
	gdouble      export_fps;        /* Frame rate for exported video */
	gchar       *export_cmd_line;   /* ffmpeg spawn cmd line */
	gchar       *export_audio_file; /* Full path to audio */
	guint        export_slide;		/* Number of slide being exported */
	GSourceFunc  export_idle_func;	/* Stored procedure for pause */
	GPid         ffmpeg_export;     /* ffmpeg's process id */
	GPid         sox_export;        /* sox's process id - FIXME: This will only
									   be needed if we decide to mangle audio
									   files using external sox binary. */
	
	/* Audio related stuff */
	GtkWidget	*music_file_treeview;
	GtkWidget	*play_audio_button;
	GtkWidget	*remove_audio_button;
	GtkListStore *music_file_liststore;
	GtkWidget	*music_time_data;
	GPid		play_child_pid;

	/* Application related stuff */
	gdouble  image_area_zoom; /* Zoom to be applied to image area */
	gboolean low_quality;     /* Preview quality:
								  TRUE  - preview in low-res
								  FALSE - preview in hi-res */
};

#endif
