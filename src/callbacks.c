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

#include "callbacks.h"

static void img_file_chooser_add_preview(img_window_struct *);
static void img_update_preview_file_chooser(GtkFileChooser *,img_window_struct *);
static gboolean img_on_expose_event(GtkWidget *,GdkEventExpose *,img_window_struct *);
static gboolean img_transition_timeout(img_window_struct *);
static gboolean img_sleep_timeout(img_window_struct *);
static gboolean img_prepare_pixbufs(img_window_struct *);
static void img_swap_toolbar_images( img_window_struct *, gboolean);
static void img_clean_after_preview(img_window_struct *);
static void img_increase_progressbar(img_window_struct *, gint);
static gboolean img_run_encoder(img_window_struct *);

/* Export related functions */
static gboolean img_export_transition(img_window_struct *);
static gboolean img_export_still(img_window_struct *);
static void img_clean_after_export(img_window_struct *);
static void img_export_pixbuf_to_ppm(GdkPixbuf *, guchar **, guint *);
static void img_export_calc_slide_frames(img_window_struct *);
static void img_export_pause_unpause(GtkToggleButton *, img_window_struct *);

void img_set_window_title(img_window_struct *img, gchar *text)
{
	gchar *title = NULL;

	if (text == NULL)
	{
		title = g_strconcat("Imagination " VERSION, NULL);
		gtk_window_set_title (GTK_WINDOW (img->imagination_window), title);
		g_free(title);
	}
	else
	{
		
		title = g_strconcat(text, " - Imagination " VERSION, NULL);
		gtk_window_set_title (GTK_WINDOW (img->imagination_window), title);
		g_free(title);
	}
}

void img_new_slideshow(GtkMenuItem *item,img_window_struct *img_struct)
{
	img_new_slideshow_settings_dialog(img_struct, FALSE);
}

void img_project_properties(GtkMenuItem *item, img_window_struct *img_struct)
{
	img_new_slideshow_settings_dialog(img_struct, TRUE);
}

void img_add_slides_thumbnails(GtkMenuItem *item,img_window_struct *img)
{
	GSList	*slides = NULL, *bak;
	GdkPixbuf *thumb;
	GtkTreeIter iter;
	slide_struct *slide_info;
	gint slides_cnt = 0;

	slides = img_import_slides_file_chooser(img);

	if (slides == NULL)
		return;

	img->slides_nr += g_slist_length(slides);
	gtk_widget_show(img->progress_bar);

	bak = slides;
	while (slides)
	{
		thumb = img_load_pixbuf_from_file(slides->data);
		if (thumb)
		{
			slide_info = img_set_slide_info(1, NORMAL, NULL, 0, slides->data);
			if (slide_info)
			{
				gtk_list_store_append (img->thumbnail_model,&iter);
				gtk_list_store_set (img->thumbnail_model, &iter, 0, thumb, 1, slide_info, -1);
				g_object_unref (thumb);
				slides_cnt++;
			}
			g_free(slides->data);
		}
		img_increase_progressbar(img, slides_cnt);
		slides = slides->next;
	}
	gtk_widget_hide(img->progress_bar);
	gtk_widget_show(img->thumb_scrolledwindow);
	g_slist_free(bak);
	img_set_total_slideshow_duration(img);
	img_set_statusbar_message(img,0);
	img->project_is_modified = TRUE;
}

static void img_increase_progressbar(img_window_struct *img, gint nr)
{
	gchar *message;
	gdouble percent;

	percent = (gdouble)nr / img->slides_nr;
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (img->progress_bar), percent);
	message = g_strdup_printf(_("Please wait, importing slide %d out of %d"), nr, img->slides_nr);
	gtk_statusbar_push(GTK_STATUSBAR(img->statusbar), img->context_id, message);
	g_free(message);

	while (gtk_events_pending())
		gtk_main_iteration();
}

GSList *img_import_slides_file_chooser(img_window_struct *img)
{
	GtkFileFilter *all_images_filter, *all_files_filter;
	gchar **mime_types;
	GSList *slides = NULL, *formats = NULL, *_formats = NULL;
	int response,i;

	img->import_slide_chooser = gtk_file_chooser_dialog_new (_("Import slides, use SHIFT key for multiple select"),
						GTK_WINDOW (img->imagination_window),
						GTK_FILE_CHOOSER_ACTION_OPEN,
						GTK_STOCK_CANCEL,
						GTK_RESPONSE_CANCEL,
						GTK_STOCK_OPEN,
						GTK_RESPONSE_ACCEPT,
						NULL);
	img_file_chooser_add_preview(img);

	/* Image files filter */
	all_images_filter = gtk_file_filter_new ();
	gtk_file_filter_set_name(all_images_filter,_("All image files"));
	formats = gdk_pixbuf_get_formats ();
	for (_formats = formats; _formats != NULL; _formats = _formats->next)
	{
		mime_types = gdk_pixbuf_format_get_mime_types ((GdkPixbufFormat *) _formats->data);
		for (i = 0; mime_types[i] != NULL; i++)
			gtk_file_filter_add_mime_type (all_images_filter, mime_types[i]);

		g_strfreev (mime_types);
	}
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(img->import_slide_chooser),all_images_filter);

	/* All files filter */
	all_files_filter = gtk_file_filter_new ();
	gtk_file_filter_set_name(all_files_filter,_("All files"));
	gtk_file_filter_add_pattern(all_files_filter,"*");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(img->import_slide_chooser),all_files_filter);

	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(img->import_slide_chooser),TRUE);
	if (img->current_dir)
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(img->import_slide_chooser),img->current_dir);
	response = gtk_dialog_run (GTK_DIALOG(img->import_slide_chooser));
	if (response == GTK_RESPONSE_ACCEPT)
	{
		slides = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(img->import_slide_chooser));
		if (img->current_dir)
			g_free(img->current_dir);
		img->current_dir = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(img->import_slide_chooser));
	}
	gtk_widget_destroy (img->import_slide_chooser);
	return slides;
}

void img_free_allocated_memory(img_window_struct *img_struct)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	slide_struct *entry;

	if (img_struct->slides_nr)
	{
		model = gtk_icon_view_get_model (GTK_ICON_VIEW(img_struct->thumbnail_iconview));

		gtk_tree_model_get_iter_first(model,&iter);
		do
		{
			gtk_tree_model_get(model, &iter,1,&entry,-1);
			g_free(entry->filename);
			g_free(entry->resolution);
			g_free(entry->type);
			g_free(entry);
			img_struct->slides_nr--;
		}
		while (gtk_tree_model_iter_next (model,&iter));
		gtk_list_store_clear(GTK_LIST_STORE(img_struct->thumbnail_model));
	}

	if (img_struct->slideshow_filename)
	{
		g_free(img_struct->slideshow_filename);
		img_struct->slideshow_filename = NULL;
	}

	if (img_struct->current_dir)
	{
		g_free(img_struct->current_dir);
		img_struct->current_dir = NULL;
	}

	if (img_struct->project_filename)
	{
		g_free(img_struct->project_filename);
		img_struct->project_filename = NULL;
	}
}

gint img_ask_user_confirmation(img_window_struct *img_struct)
{
	GtkWidget *dialog;
	gint response;

	dialog = gtk_message_dialog_new(GTK_WINDOW(img_struct->imagination_window),GTK_DIALOG_MODAL,GTK_MESSAGE_QUESTION,GTK_BUTTONS_OK_CANCEL,_("You didn't save your slideshow yet. Are you sure you want to close it?"));
	gtk_window_set_title(GTK_WINDOW(dialog),"Imagination");
	response = gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (GTK_WIDGET (dialog));
	return response;
}

gboolean img_quit_application(GtkWidget *widget, GdkEvent *event, img_window_struct *img_struct)
{
	gint response;

	if (img_struct->project_is_modified)
	{
		response = img_ask_user_confirmation(img_struct);
		if (response != GTK_RESPONSE_OK)
			return TRUE;
	}
	img_free_allocated_memory(img_struct);

	/* Unloads the plugins */
	g_slist_foreach(img_struct->plugin_list,(GFunc)g_module_close,NULL);
	g_slist_free(img_struct->plugin_list);
	return FALSE;
}

static void img_file_chooser_add_preview(img_window_struct *img_struct)
{
	GtkWidget *vbox;

	vbox = gtk_vbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER(vbox), 10);

	img_struct->preview_image = gtk_image_new ();

	img_struct->dim_label  = gtk_label_new (NULL);
	img_struct->size_label = gtk_label_new (NULL);

	gtk_box_pack_start (GTK_BOX (vbox), img_struct->preview_image, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), img_struct->dim_label, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), img_struct->size_label, FALSE, TRUE, 0);
	gtk_widget_show_all (vbox);

	gtk_file_chooser_set_preview_widget (GTK_FILE_CHOOSER(img_struct->import_slide_chooser), vbox);
	gtk_file_chooser_set_preview_widget_active (GTK_FILE_CHOOSER(img_struct->import_slide_chooser), FALSE);

	g_signal_connect (img_struct->import_slide_chooser, "update-preview",G_CALLBACK (img_update_preview_file_chooser), img_struct);
}

static void	img_update_preview_file_chooser(GtkFileChooser *file_chooser,img_window_struct *img_struct)
{
	gchar *filename,*size;
	gboolean has_preview = FALSE;
	gint width,height;
	GdkPixbuf *pixbuf;
	GdkPixbufFormat *pixbuf_format;

	filename = gtk_file_chooser_get_filename(file_chooser);
	if (filename == NULL)
	{
		gtk_file_chooser_set_preview_widget_active (file_chooser, has_preview);
		return;
	}
	pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, 93, 70, TRUE, NULL);
	has_preview = (pixbuf != NULL);
	if (has_preview)
	{
		pixbuf_format = gdk_pixbuf_get_file_info(filename,&width,&height);
		gtk_image_set_from_pixbuf (GTK_IMAGE(img_struct->preview_image), pixbuf);
		g_object_unref (pixbuf);

		size = g_strdup_printf(ngettext("%d x %d pixels", "%d x %d pixels", height),width,height);
		gtk_label_set_text(GTK_LABEL(img_struct->dim_label),size);
		g_free(size);
	}
	g_free(filename);
	gtk_file_chooser_set_preview_widget_active (file_chooser, has_preview);
}

void img_delete_selected_slides(GtkMenuItem *item,img_window_struct *img_struct)
{
	GList *selected, *bak;
	GtkTreeIter iter;
	GtkTreeModel *model;
	slide_struct *entry;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img_struct->thumbnail_iconview));
	
	selected = gtk_icon_view_get_selected_items (GTK_ICON_VIEW(img_struct->thumbnail_iconview));
	if (selected == NULL)
		return;
	
	/* Free the slide struct for each slide and remove it from the iconview */
	/* Stored list start to avoid memory leak + pulled signal blocks one level
	 * higher to avoid calling them separately for each entry in list (we are
	 * removing items synchronously from withing the callback, so the GUI is
	 * already "frozen" and selection cannot be changed through the user
	 * intervention until the callback returns). */
	bak = selected;
	g_signal_handlers_block_by_func((gpointer)img_struct->thumbnail_iconview, (gpointer)img_iconview_selection_changed, img_struct);
	while (selected)
  	{
  		gtk_tree_model_get_iter(model, &iter,selected->data);
  		gtk_tree_model_get(model, &iter,1,&entry,-1);
  		g_free(entry->filename);
  		g_free(entry->resolution);
  		g_free(entry->type);
  		g_free(entry);

  		gtk_list_store_remove(GTK_LIST_STORE(img_struct->thumbnail_model),&iter);
 
  		img_struct->slides_nr--;
  		selected = selected->next;
  	}
	g_signal_handlers_unblock_by_func((gpointer)img_struct->thumbnail_iconview, (gpointer)img_iconview_selection_changed, img_struct);  		
	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(selected);
	img_set_statusbar_message(img_struct,0);
	gtk_image_set_from_pixbuf(GTK_IMAGE(img_struct->image_area),NULL);
	img_struct->project_is_modified = TRUE;

	if (img_struct->slides_nr == 0)
		gtk_widget_hide(img_struct->thumb_scrolledwindow);

	img_iconview_selection_changed(GTK_ICON_VIEW(img_struct->thumbnail_iconview),img_struct);
}

void img_show_about_dialog (GtkMenuItem *item,img_window_struct *img_struct)
{
	static GtkWidget *about = NULL;
	static gchar version[] = VERSION "-" REVISION;
    const char *authors[] = {"\nDevelopers:\nGiuseppe Torelli <colossus73@gmail.com>\nTadej Borovšak <tadeboro@gmail.com>\n\nImagination logo:\nJaws, Dadster, Gemini and Lunoob\nfrom http://linuxgraphicsusers.com\n\n",NULL};
    const char *documenters[] = {NULL};

	if (about == NULL)
	{
		about = gtk_about_dialog_new ();
		/*gtk_about_dialog_set_email_hook (img_activate_link,NULL,NULL);
		gtk_about_dialog_set_url_hook (img_activate_link,NULL,NULL);*/
		gtk_window_set_position (GTK_WINDOW (about),GTK_WIN_POS_CENTER_ON_PARENT);
		gtk_window_set_transient_for (GTK_WINDOW (about),GTK_WINDOW (img_struct->imagination_window));
		gtk_window_set_destroy_with_parent (GTK_WINDOW (about),TRUE);
		g_object_set (about,
			"name", "Imagination",
			"version", strcmp(REVISION, "-1") == 0 ? VERSION : version,
			"copyright","Copyright \xC2\xA9 2009 Giuseppe Torelli",
			"comments","A simple and lightweight DVD slideshow maker",
			"authors",authors,
			"documenters",documenters,
			"translator_credits",_("translator-credits"),
			"logo_icon_name","imagination",
			"website","http://imagination.sf.net",
			"license","Copyright \xC2\xA9 2009 Giuseppe Torelli - Colossus <colossus73@gmail.com>\n\n"
		    			"This is free software; you can redistribute it and/or\n"
    					"modify it under the terms of the GNU Library General Public License as\n"
    					"published by the Free Software Foundation; either version 2 of the\n"
    					"License,or (at your option) any later version.\n"
    					"\n"
    					"This software is distributed in the hope that it will be useful,\n"
    					"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    					"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU\n"
    					"Library General Public License for more details.\n"
    					"\n"
    					"You should have received a copy of the GNU Library General Public\n"
    					"License along with the Gnome Library; see the file COPYING.LIB.  If not,\n"
    					"write to the Free Software Foundation,Inc.,59 Temple Place - Suite 330,\n"
    					"Boston,MA 02111-1307,USA.\n",
		      NULL);
	}
	gtk_dialog_run ( GTK_DIALOG(about));
	gtk_widget_hide (about);
}

void img_set_total_slideshow_duration(img_window_struct *img)
{
	gchar *time;
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;
	gint h, m, s;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	if (!gtk_tree_model_get_iter_first (model,&iter))
		return;

	img->total_secs = 0;
	do
	{
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		img->total_secs += entry->duration;

		if(entry->render)
			img->total_secs += entry->speed;
	}
	while (gtk_tree_model_iter_next (model,&iter));

	/* Fix secs -> hour, mins and secs conversion */
	h =  img->total_secs / 3600;
	m = (img->total_secs % 3600) / 60;
	s =  img->total_secs - (h * 3600) - (m * 60);
	time = g_strdup_printf("%02d:%02d:%02d", h, m, s);
	gtk_label_set_text(GTK_LABEL (img->total_time_data),time);
	g_free(time);

	img->project_is_modified = TRUE;
}

void img_start_stop_preview(GtkWidget *button, img_window_struct *img)
{
	GtkTreeIter iter, prev;
	GtkTreePath *path = NULL;
	slide_struct *entry;
	GtkTreeModel *model;
	GList *list = NULL;

	if(img->export_is_running)
		return;

	if (img->preview_is_running)
	{
		/* Preview is already running */

		/* Remove timeout function from main loop */
		g_source_remove(img->source_id);

		/* Clean resources used by preview and prepare application for
		 * next preview. */
		img_clean_after_preview(img);
	}
	else
	{
		/* Start the preview */
		model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
		list = gtk_icon_view_get_selected_items( GTK_ICON_VIEW( img->thumbnail_iconview ) );
		if( list )
			gtk_icon_view_get_cursor(GTK_ICON_VIEW(img->thumbnail_iconview), &path, NULL);
		if( list && path )
		{
			/* Start preview from this slide */
			gtk_tree_model_get_iter( model, &iter, path );
			g_list_foreach( list, (GFunc)gtk_tree_path_free, NULL );
			g_list_free( list );
		}
		else
		{
			/* Start preview from the beginning */
			if(!gtk_tree_model_get_iter_first (model,&iter))
				return;
		}

		/* Replace button and menu images */
		img_swap_toolbar_images( img, FALSE );

		/* Store currently displayed image and then clear image_area.
		 * If the image is not cleared, the transition from*/
		img->slide_pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(img->image_area));
		if (img->slide_pixbuf)
			g_object_ref(G_OBJECT(img->slide_pixbuf));
		gtk_image_clear(GTK_IMAGE(img->image_area));

		/* Connect expose event to handler */
		gtk_widget_set_app_paintable(img->image_area, TRUE);
		g_signal_connect( G_OBJECT(img->image_area), "expose-event",G_CALLBACK(img_on_expose_event),img);

		/* Load the first image in the pixbuf */
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		img->pixbuf2 = img_scale_pixbuf(img,entry->filename);
		img->current_slide = entry;

		/* If we started our preview from beginning, create empty pixbuf and
		 * fill it with background color. Else load image that is before
		 * currently selected slide. */
		if( path != NULL && gtk_tree_path_prev( path ) )
		{
			if( ! img->cur_ss_iter )
				img->cur_ss_iter = g_slice_new( GtkTreeIter );

			gtk_tree_model_get_iter( model, &prev, path );
			gtk_tree_model_get( model, &prev, 1, &entry, -1 );
			img->pixbuf1 = img_scale_pixbuf(img, entry->filename);
			*img->cur_ss_iter = iter;
		}
		else
		{
			img->pixbuf1 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,img->image_area->allocation.width,img->image_area->allocation.height);
			gdk_pixbuf_fill(img->pixbuf1,img->background_color);
		}
		if( path )
			gtk_tree_path_free( path );

		/* Add transition timeout function */
		img->preview_is_running = TRUE;
		img->progress = 0;
		img->source_id = g_timeout_add(1000 / PREVIEW_FPS,(GSourceFunc)img_transition_timeout,img);
	}
	return;
}

static gboolean img_on_expose_event(GtkWidget *widget,GdkEventExpose *event,img_window_struct *img)
{
	/* We always pass negative number as a last parameter when we want to
	 * draw on screen. */
	if ((img->current_slide)->render)
		(img->current_slide)->render (widget->window, img->pixbuf1, img->pixbuf2,img->progress, -1);
	else
	{
		/* This is "None" transition renderer */
		cairo_t *cr;
		gint     offset_x, offset_y, width, height;

		gdk_drawable_get_size(widget->window, &width, &height);
		offset_x = (width  - gdk_pixbuf_get_width (img->pixbuf2)) / 2;
		offset_y = (height - gdk_pixbuf_get_height(img->pixbuf2)) / 2;
		
		cr = gdk_cairo_create(widget->window);
		gdk_cairo_set_source_pixbuf(cr,img->pixbuf2,offset_x,offset_y);
		cairo_paint(cr);
		
		cairo_destroy(cr);
	}
	return FALSE;
}

GdkPixbuf *img_scale_pixbuf(img_window_struct *img, gchar *filename)
{
	GdkPixbuf *pixbuf, *compose;
	gint       i_width, i_height;		/* Image dimensions */
	gint       a_width, a_height;		/* Display dimensions */
	gint       offset_x, offset_y;
	gdouble    a_ratio, i_ratio;
	gdouble    max_stretch = 0.1280;	/* Maximum amount of stretch */
	gdouble    max_crop    = 0.8500;	/* Maximum amount of crop */
	gboolean   too_small;

	/* Obtaint information about display area */
	a_width  = img->image_area->allocation.width;
	a_height = img->image_area->allocation.height;
	a_ratio  = (gdouble)a_width / a_height;

	/* Obtain information about image being loaded */
	gdk_pixbuf_get_file_info( filename, &i_width, &i_height );
	i_ratio = (gdouble)i_width / i_height;

	/* If the image is smaller than display area, just center it and fill
	 * the background with color. We do approximatelly the same thing if
	 * the user doesn't want to distort images. */
	too_small = ( i_width < a_width && i_height < a_height );
	if( ( ! img->distort_images ) || too_small )
	{
		if( too_small )
			pixbuf = gdk_pixbuf_new_from_file( filename, NULL );
		else
			pixbuf = gdk_pixbuf_new_from_file_at_size( filename, a_width, a_height, NULL );

		i_width  = gdk_pixbuf_get_width( pixbuf );
		i_height = gdk_pixbuf_get_height( pixbuf );
		offset_x = ( a_width  - i_width  ) / 2;
		offset_y = ( a_height - i_height ) / 2;
		compose = gdk_pixbuf_new( GDK_COLORSPACE_RGB, FALSE, 8,
								  a_width, a_height );
		gdk_pixbuf_fill( compose, img->background_color );
		gdk_pixbuf_composite( pixbuf, compose, offset_x, offset_y, i_width,
							  i_height, offset_x, offset_y, 1, 1,
							  GDK_INTERP_BILINEAR, 255 );
		g_object_unref( G_OBJECT( pixbuf ) );
		return( compose );
	}

	/* If the user doesn't want to have images distorted, we only scale
	 * image. */

	/* If we are here, the image is too big for display area, so we need to
	 * scale it. Depending on values of i_ratio and a_ratio, we'll do some
	 * transformations to, but won't distort the image for more than 6.25%
	 * of it's size (just enough to snuggly convert 4:3 image to PAL) or crop
	 * it more than 10%. */

	if( i_ratio < a_ratio )
	{
		if( i_ratio > a_ratio * ( 1 - max_stretch ) )
		{
			/* We can shrink image vertically enough to fit. */
			return( gdk_pixbuf_new_from_file_at_scale( filename, a_width, a_height, FALSE, NULL ) );
		}
		else
		{
			if( i_ratio > a_ratio * ( 1 - max_stretch ) * max_crop )
			{
				/* We can shrink image vertically and crop it to fit. */
				pixbuf = gdk_pixbuf_new_from_file_at_scale(
							filename, a_width, a_height * ( 1 + max_stretch ), FALSE, NULL );
			}
			else
			{
				/* We cannot avoid white stripes on the left/right. Sorry. */
				pixbuf = gdk_pixbuf_new_from_file_at_size( filename, a_width, a_height, NULL );
			}
		}
	}
	else
	{
		if( i_ratio < a_ratio * ( 1 + max_stretch ) )
		{
			/* We can shrink image horizontally enough to fit. */
			return( gdk_pixbuf_new_from_file_at_scale( filename, a_width, a_height, FALSE, NULL ) );
		}
		else
		{
			if( i_ratio < a_ratio * ( 1 + max_stretch ) / max_crop )
			{
				/* We can shrink image horizontally and crop it to fit. */
				pixbuf = gdk_pixbuf_new_from_file_at_scale(
							filename, a_width * ( 1 + max_stretch ), a_height, FALSE, NULL );
			}
			else
			{
				/* We cannot avoid white stripes at the bottom/top. Sorry. */
				pixbuf = gdk_pixbuf_new_from_file_at_size( filename, a_width, a_height, NULL );
			}
		}
	}

	/* Do composing */
	i_width  = gdk_pixbuf_get_width( pixbuf );
	i_height = gdk_pixbuf_get_height( pixbuf );
	offset_x = ( a_width - i_width ) / 2;
	offset_y = ( a_height - i_height ) / 2;

	compose = gdk_pixbuf_new( GDK_COLORSPACE_RGB, FALSE, 8, a_width, a_height );
	gdk_pixbuf_fill( compose, img->background_color );
	gdk_pixbuf_composite( pixbuf, compose,
						  offset_x < 0 ? 0 : offset_x,
						  offset_y < 0 ? 0 : offset_y,
						  i_width < a_width ? i_width : a_width,
						  i_height < a_height ? i_height : a_height,
						  offset_x, offset_y, 1, 1,
						  GDK_INTERP_BILINEAR, 255 );
	g_object_unref( G_OBJECT( pixbuf ) );

	return( compose );
}

static gboolean img_transition_timeout(img_window_struct *img)
{
	/* Increment progress variable (this is being passed as a parameter
	 * to plug-in provided transition function).
	 * 1000 ms / TRANSITION_TIMEOUT gives us a frame rate of preview
	 * (currently, this is 1000 / 40 = 25 fps). */
	img->progress += (gdouble)1 / ( img->current_slide->speed * PREVIEW_FPS );
	
	/* if the transition render is NULL (no transition is set for this
	 * slide), we stop this timeout function, but still invalidate
	 * preview area so expose event gets called. */
	if( img->current_slide->render == NULL )
	{
		img->progress = 0;
		img->source_id = g_timeout_add( img->current_slide->duration * 1000, (GSourceFunc)img_sleep_timeout, img );
		gtk_widget_queue_draw( img->image_area );
		return FALSE;
	}

	/* If the progress reached 1, the transition should be finished and
	 * it's time to stop this timeout function and add sleep timeout
	 * function to the main loop. The 0.0000005 is added to accomodate
	 * floating number rounding errors (if we would write this condition
	 * as img->progress > 1, we would miss the last frame, since
	 * (gdouble)1 > 1 */
	if( img->progress > 1.0000005 )
	{
		img->progress = 0;
		img->source_id = g_timeout_add( img->current_slide->duration * 1000,(GSourceFunc)img_sleep_timeout, img );
		return FALSE;
	}

	/* Schedule our image redraw */
	gtk_widget_queue_draw( img->image_area );

	return TRUE;
}

static gboolean img_sleep_timeout(img_window_struct *img)
{
	if(img_prepare_pixbufs(img))
	{
		img->source_id = g_timeout_add( 1000 / PREVIEW_FPS,(GSourceFunc)img_transition_timeout,img );
	}
	else
	{
		/* Clean resources used in preview and prepare application for
		 * next preview. */
		img_clean_after_preview(img);
	}

	return FALSE;
}

static void img_swap_toolbar_images( img_window_struct *img,gboolean flag )
{
	GtkWidget *tmp_image;

	if( flag )
	{
		tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY,GTK_ICON_SIZE_MENU);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img->preview_menu),tmp_image);
		
		tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY,GTK_ICON_SIZE_LARGE_TOOLBAR);
		gtk_widget_show(tmp_image);
		g_object_set(img->preview_button,"icon-widget",tmp_image,NULL);
		gtk_widget_set_tooltip_text(img->preview_button,_("Starts the preview"));
	}
	else
	{
		tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_STOP,GTK_ICON_SIZE_MENU);
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img->preview_menu),tmp_image);
		
		tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_STOP,GTK_ICON_SIZE_LARGE_TOOLBAR);
		gtk_widget_show(tmp_image);
		g_object_set(img->preview_button,"icon-widget",tmp_image,NULL);
		gtk_widget_set_tooltip_text(img->preview_button,_("Stops the preview"));
	}
}

static void img_clean_after_preview(img_window_struct *img)
{
	/* Disconnect expose event handler */
	g_signal_handlers_disconnect_by_func(img->image_area,img_on_expose_event,img);
	gtk_widget_set_app_paintable(img->image_area, FALSE);

	/* Restore image that was used before preview */
	gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area), img->slide_pixbuf);
	if( img->slide_pixbuf )
		g_object_unref(G_OBJECT(img->slide_pixbuf));

	/* Swap toolbar and menu icons */
	img_swap_toolbar_images( img, TRUE );

	/* Indicate that preview is not running */
	img->preview_is_running = FALSE;

	/* Clean the resources used by timeout handlers */
	g_slice_free( GtkTreeIter, img->cur_ss_iter );
	img->cur_ss_iter = NULL;

	return;
}

void img_choose_slideshow_filename(GtkWidget *widget, img_window_struct *img)
{
	GtkWidget *fc;
	GtkFileChooserAction action = 0;
	gint response;

	/* Determine the mode of the chooser. */
	if (widget == img->open_menu || widget == img->open_button)
		action = GTK_FILE_CHOOSER_ACTION_OPEN;
	else if (widget == img->save_as_menu || widget == img->save_menu || widget == img->save_button)
		action = GTK_FILE_CHOOSER_ACTION_SAVE;

	if (img->project_filename == NULL || widget == img->save_as_menu || action == GTK_FILE_CHOOSER_ACTION_OPEN)
	{
		fc = gtk_file_chooser_dialog_new (action == GTK_FILE_CHOOSER_ACTION_OPEN ? _("Load an Imagination slideshow project") : 
					_("Save an Imagination slideshow project"),
					GTK_WINDOW (img->imagination_window),
					action,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					action == GTK_FILE_CHOOSER_ACTION_OPEN ?  GTK_STOCK_OPEN : GTK_STOCK_SAVE,
					GTK_RESPONSE_ACCEPT,NULL);

		gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER (fc),TRUE);
		response = gtk_dialog_run (GTK_DIALOG (fc));
		if (response == GTK_RESPONSE_ACCEPT)
		{
			gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
			if(filename)
			{
				/* Free any previous filename */
				if(img->project_filename)
					g_free(img->project_filename);
				
				/* Store new filename */
				img->project_filename = filename;
				gtk_widget_destroy(fc);
			}
		}
		else if (response == GTK_RESPONSE_CANCEL || GTK_RESPONSE_DELETE_EVENT)
		{
			gtk_widget_destroy(fc);
			return;
		}
	}
	if (action == GTK_FILE_CHOOSER_ACTION_OPEN)
		img_load_slideshow(img);
	else
		img_save_slideshow(img);
}

void img_close_slideshow(GtkWidget *widget, img_window_struct *img)
{
	if (img->project_is_modified)
	{
		if (GTK_RESPONSE_OK != img_ask_user_confirmation(img))

			return;
	}
	img_free_allocated_memory(img);
	img_set_buttons_state(img, FALSE);
	img_set_window_title(img,NULL);
	img_set_statusbar_message(img,0);
	gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area),NULL);
	gtk_widget_hide(img->thumb_scrolledwindow);
}

void img_start_stop_export(GtkWidget *widget, img_window_struct *img)
{
	GtkTreeIter   iter;
	slide_struct *entry;
	GtkTreeModel *model;

	/* If we are displaying preview, abort or bad things will happen. */
	if(img->preview_is_running)
		return;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	if(!gtk_tree_model_get_iter_first(model, &iter))
			return;

	if(img->export_is_running)
	{
		/* Remove idle function from main loop */
		g_source_remove(img->source_id);

		/* Clean resources used by export. */
		img_clean_after_export(img);
	}
	else
	{
		GtkWidget *dialog;
		GtkWidget *vbox, *hbox;
		GtkWidget *label;
		GtkWidget *progress;
		GtkWidget *button;
		gchar     *string;

		if(!img_run_encoder(img))
			return;

		/* Create progress window with cancel and pause buttons, calculate
		 * the total number of frames to display. */
		dialog = gtk_window_new( GTK_WINDOW_TOPLEVEL );
		img->export_dialog = dialog;
		gtk_window_set_title (GTK_WINDOW(img->export_dialog),_("Exporting the slideshow"));
		gtk_container_set_border_width( GTK_CONTAINER( dialog ), 10 );
		gtk_window_set_default_size( GTK_WINDOW( dialog ), 400, -1 );
		gtk_window_set_type_hint( GTK_WINDOW( dialog ),
								  GDK_WINDOW_TYPE_HINT_DIALOG );
		gtk_window_set_modal( GTK_WINDOW( dialog ), TRUE );
		gtk_window_set_transient_for( GTK_WINDOW( dialog ),
									  GTK_WINDOW( img->imagination_window ) );

		vbox = gtk_vbox_new( FALSE, 6 );
		gtk_container_add( GTK_CONTAINER( dialog ), vbox );

		label = gtk_label_new( _("Preparing for export ...") );
		gtk_misc_set_alignment( GTK_MISC( label ), 0, 0.5 );
		img->export_label = label;
		gtk_box_pack_start( GTK_BOX( vbox ), label, FALSE, FALSE, 0 );

		progress = gtk_progress_bar_new();
		img->export_pbar1 = progress;
		string = g_strdup_printf( "%.2f", .0 );
		gtk_progress_bar_set_text( GTK_PROGRESS_BAR( progress ), string );
		gtk_box_pack_start( GTK_BOX( vbox ), progress, FALSE, FALSE, 0 );

		label = gtk_label_new( _("Overall progress:") );
		gtk_misc_set_alignment( GTK_MISC( label ), 0, 0.5 );
		gtk_box_pack_start( GTK_BOX( vbox ), label, FALSE, FALSE, 0 );

		progress = gtk_progress_bar_new();
		img->export_pbar2 = progress;
		gtk_progress_bar_set_text( GTK_PROGRESS_BAR( progress ), string );
		gtk_box_pack_start( GTK_BOX( vbox ), progress, FALSE, FALSE, 0 );
		g_free( string );

		hbox = gtk_hbox_new( TRUE, 6 );
		gtk_box_pack_start( GTK_BOX( vbox ), hbox, FALSE, FALSE, 0 );

		button = gtk_button_new_from_stock( GTK_STOCK_CANCEL );
		g_signal_connect( G_OBJECT( button ), "clicked",
						  G_CALLBACK( img_start_stop_export ), img );
		gtk_box_pack_end( GTK_BOX( hbox ), button, FALSE, FALSE, 0 );

		button = gtk_toggle_button_new_with_label( GTK_STOCK_MEDIA_PAUSE );
		gtk_button_set_use_stock( GTK_BUTTON( button ), TRUE );
		g_signal_connect( G_OBJECT( button ), "toggled",
						  G_CALLBACK( img_export_pause_unpause ), img );
		gtk_box_pack_end( GTK_BOX( hbox ), button, FALSE, FALSE, 0 );

		gtk_widget_show_all( dialog );

		/* Display some visual feedback */
		while( gtk_events_pending() )
			gtk_main_iteration();

		img->slide_pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(img->image_area));
		if(img->slide_pixbuf)
			g_object_ref(G_OBJECT(img->slide_pixbuf));
		gtk_image_clear(GTK_IMAGE(img->image_area));
		gtk_widget_set_app_paintable(img->image_area, TRUE);
		g_signal_connect(G_OBJECT(img->image_area), "expose-event", G_CALLBACK(img_on_expose_event), img);

		/* Create an empty pixbuf for starting image. */
		img->pixbuf1 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, img->image_area->allocation.width, img->image_area->allocation.height);
		gdk_pixbuf_fill(img->pixbuf1, img->background_color);

		/* Load first image from model */
		gtk_tree_model_get(model, &iter, 1, &entry, -1);
		img->pixbuf2 = img_scale_pixbuf(img, entry->filename);

		/* Add export idle function and set initial values */
		img->export_is_running = TRUE;
		img->current_slide = entry;
		img->progress = 0;
		img->export_frame_nr = img->total_secs * EXPORT_FPS;
		img->export_frame_cur = 0;
		/* Fix for the wrong progress bar indicators. */
		img_export_calc_slide_frames( img );

		img->export_slide = 1;
		img->export_idle_func = (GSourceFunc)img_export_transition;
		img->source_id = g_idle_add((GSourceFunc)img_export_transition, img);

		string = g_strdup_printf( _("Slide %d export progress:"), 1 );
		/* I did this for the translators. ^^ */
		gtk_label_set_label( GTK_LABEL( img->export_label ), string );
		g_free( string );
	}
}

static gboolean img_export_transition(img_window_struct *img)
{
	gchar           string[10];
	gdouble         export_progress;

	/* If no transition effect is set, just connect still export
	 * idle function and remove itself from main loop. */
	if(img->current_slide->render == NULL)
	{
		img->source_id = g_idle_add((GSourceFunc)img_export_still, img);
		return(FALSE);
	}

	/* Switch to still export phase if progress reached 1. */
	img->progress += (gdouble)1 / ( img->current_slide->speed * EXPORT_FPS );
	if(img->progress > 1.00000005)
	{
		img->progress = 0;
		img->export_idle_func = (GSourceFunc)img_export_still;
		img->source_id = g_idle_add((GSourceFunc)img_export_still, img);

		return(FALSE);
	}

	/* Draw one frame of transition animation */
	img->current_slide->render(img->image_area->window, img->pixbuf1, img->pixbuf2, img->progress, img->file_desc);

	/* Increment global frame counters and update progress bars */
	img->export_frame_cur++;
	img->export_slide_cur++;

	export_progress = (gdouble)img->export_slide_cur / img->export_slide_nr;
	snprintf( string, 10, "%.2f%%", export_progress * 100 );
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( img->export_pbar1 ),
								   export_progress );
	gtk_progress_bar_set_text( GTK_PROGRESS_BAR( img->export_pbar1 ), string );

	export_progress = (gdouble)img->export_frame_cur / img->export_frame_nr;
	snprintf( string, 10, "%.2f%%", export_progress * 100 );
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( img->export_pbar2 ),
								   export_progress );
	gtk_progress_bar_set_text( GTK_PROGRESS_BAR( img->export_pbar2 ), string );

	/* Draw every 10th frame of animation on screen */
	if(img->export_frame_cur % 10 == 0)
		gtk_widget_queue_draw(img->image_area);

	return(TRUE);
}

static gboolean img_export_still(img_window_struct *img)
{
	static guint   lenght;
	gdouble        export_progress;
	gchar          string[10];

	/* Initialize pixbuf data buffer */
	if(img->pixbuf_data == NULL)
	{
		gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area), img->pixbuf2);
		img_export_pixbuf_to_ppm(img->pixbuf2, &img->pixbuf_data, &lenght);
	}

	 /* Draw frames until we have enough of them to fill slide duration gap. */
	if( img->export_slide_cur > img->export_slide_nr )
	{
		/* Exit still rendering and continue with next transition. */

		/* Clear image area for next renderer */
		gtk_image_clear(GTK_IMAGE(img->image_area));

		/* Load next image from store. */
		if(img_prepare_pixbufs(img))
		{
			gchar *string;

			/* Update progress counters */
			img->export_slide++;
			string = g_strdup_printf( _("Slide %d export progress:"), img->export_slide );
			gtk_label_set_label( GTK_LABEL( img->export_label ), string );
			g_free( string );
			/* Progress bug fix */
			img_export_calc_slide_frames( img );

			g_free(img->pixbuf_data);
			img->pixbuf_data = NULL;
			img->export_idle_func = (GSourceFunc)img_export_transition;
			img->source_id = g_idle_add((GSourceFunc)img_export_transition, img);
		}
		else
			img_clean_after_export(img);

		return(FALSE);
	}
	write(img->file_desc, img->pixbuf_data, lenght);

	/* Increment global frame counter and update progress bar */
	img->export_frame_cur++;
	img->export_slide_cur++;

	/* CLAMPS are needed here because of the loosy conversion when switching
	 * from floating point to integer arithmetics. */
	export_progress = CLAMP( (gdouble)img->export_slide_cur / img->export_slide_nr, 0, 1 );
	snprintf( string, 10, "%.2f%%", export_progress * 100 );
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( img->export_pbar1 ),
								   export_progress );
	gtk_progress_bar_set_text( GTK_PROGRESS_BAR( img->export_pbar1 ), string );

	export_progress = CLAMP( (gdouble)img->export_frame_cur / img->export_frame_nr, 0, 1 );
	snprintf( string, 10, "%.2f%%", export_progress * 100 );
	gtk_progress_bar_set_fraction( GTK_PROGRESS_BAR( img->export_pbar2 ),
								   export_progress );
	gtk_progress_bar_set_text( GTK_PROGRESS_BAR( img->export_pbar2 ), string );

	return(TRUE);
}

static void img_clean_after_export(img_window_struct *img)
{
	/* Disconnect expose event */
	g_signal_handlers_block_by_func(img->image_area, img_on_expose_event, img);
	gtk_widget_set_app_paintable(img->image_area, FALSE);

	/* Restore image that was used before export */
	gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area), img->slide_pixbuf);
	if(img->slide_pixbuf)
		g_object_unref(G_OBJECT(img->slide_pixbuf));

	/* Indicate that export is not running any more */
	img->export_is_running = FALSE;

	/* Clean other resources */
	g_slice_free(GtkTreeIter, img->cur_ss_iter);
	img->cur_ss_iter = NULL;
	g_free(img->pixbuf_data);
	img->pixbuf_data = NULL;
	gtk_widget_destroy( img->export_dialog );

	close(img->file_desc);
}

/* Move one step forward in model and set img->pixbuf1 and img->pixbuf2
 * to appropriate values.
 * Return FALSE if we reached the end of the model, else TRUE. */
static gboolean img_prepare_pixbufs(img_window_struct *img)
{
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));

	if(!img->cur_ss_iter)
	{
		img->cur_ss_iter = g_slice_new(GtkTreeIter);
		gtk_tree_model_get_iter_first(model, img->cur_ss_iter);
	}

	if(gtk_tree_model_iter_next(model, img->cur_ss_iter))
	{
		g_object_unref(G_OBJECT(img->pixbuf1));
		img->pixbuf1 = img->pixbuf2;
		gtk_tree_model_get(model, img->cur_ss_iter, 1, &img->current_slide, -1);
		img->pixbuf2 = img_scale_pixbuf(img, img->current_slide->filename);

		return(TRUE);
	}
	else
		return(FALSE);


	if(gtk_tree_model_iter_next(model, img->cur_ss_iter))
	{
		g_object_unref(G_OBJECT(img->pixbuf1));
		img->pixbuf1 = img->pixbuf2;
		gtk_tree_model_get(model, img->cur_ss_iter, 1, &img->current_slide, -1);
		img->pixbuf2 = img_scale_pixbuf(img, img->current_slide->filename);

		return(TRUE);
	}
	else
		return(FALSE);
}

static void img_export_pixbuf_to_ppm(GdkPixbuf *pixbuf, guchar **data, guint *lenght)
{
	gint      width, height, stride, channels;
	guchar   *pixels, *tmp;
	gint      col, row;
	gchar    *header;
	gint      header_lenght;

	width    = gdk_pixbuf_get_width(pixbuf);
	height   = gdk_pixbuf_get_height(pixbuf);
	stride   = gdk_pixbuf_get_rowstride(pixbuf);
	channels = gdk_pixbuf_get_n_channels(pixbuf);
	pixels   = gdk_pixbuf_get_pixels(pixbuf);

	header = g_strdup_printf("P6\n%d %d\n255\n", width, height);
	header_lenght = strlen(header) * sizeof(gchar);

	*lenght = sizeof(guchar) * width * height * channels + header_lenght;
	*data = g_slice_alloc(sizeof(guchar) * *lenght);

	memcpy(*data, header, header_lenght);
	tmp = *data + header_lenght;
	for(row = 0; row < height; row++)
	{
		for(col = 0; col < width; col++)
		{
			tmp[0] = pixels[0];
			tmp[1] = pixels[1];
			tmp[2] = pixels[2];

			tmp    += 3;
			pixels += channels;
		}
	}
}

/*
// In GdkPixbuf 2.12 or above, this returns the EXIF orientation value.
const char* exif_orientation = gdk_pixbuf_get_option(thumb->pixbuf, "orientation");
if (exif_orientation != NULL) {
switch (exif_orientation[0]) {
case '3':
thumb->rotation = 180;
          break;
       case '6':
         thumb->rotation = 270;
          break;
       case '8':
         thumb->rotation = 90;
          break;
       // '1' means no rotation.  The other four values are all various
          transpositions, which are rare in real photos so we don't
          implement them. 
      }
    }
  }

  // Rotate if necessary 
  if (thumb->rotation != 0)
  {
      GdkPixbuf *new_pixbuf = gdk_pixbuf_rotate_simple (thumb->pixbuf, thumb->rotation);
      g_object_unref (thumb->pixbuf);
      thumb->pixbuf = new_pixbuf;
      // Clean up 
      thumb->rotation = 0;
 }
 */

/* Changed return value to gboolean. This way we can abort export in
 * img_start_stop_export function. */
static gboolean img_run_encoder(img_window_struct *img)
{
	GtkWidget  *message;
	GError     *error = NULL;
	gchar     **argv;
	gchar      *cmd_line;
	gboolean    ret;

	if (img->slideshow_format_index == 0)
		cmd_line = g_strdup_printf(
				"ffmpeg -f image2pipe -vcodec ppm -r %s -i pipe: "
				"-target %s-dvd -r %s -an -aspect %s -s %dx%d -y -bf 2 %s",
				EXPORT_FPS_STRING,
				img->image_area->allocation.height == 576 ? "pal" : "ntsc",
				EXPORT_FPS_STRING,
				img->aspect_ratio, img->image_area->allocation.width,
				img->image_area->allocation.height, img->slideshow_filename );
	else
		cmd_line = g_strconcat(
				"ffmpeg -f image2pipe -vcodec ppm -r " EXPORT_FPS_STRING
				" -i pipe: -an -b 512k -s 320x240 -f flv -y ",
				img->slideshow_filename,NULL);
	argv = g_strsplit( cmd_line, " ", 0 );
	g_print( "%s\n", cmd_line );
	g_free( cmd_line );

	ret = g_spawn_async_with_pipes( NULL, argv, NULL,
									G_SPAWN_SEARCH_PATH/* |
									G_SPAWN_STDOUT_TO_DEV_NULL |
									G_SPAWN_STDERR_TO_DEV_NULL*/,
									NULL, NULL, NULL, &img->file_desc,
									NULL, NULL, &error );
	if( ! ret )
	{
		message = gtk_message_dialog_new (GTK_WINDOW (img->imagination_window),
										GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
										GTK_MESSAGE_ERROR,
										GTK_BUTTONS_CLOSE,
										_("Failed to launch the encoder!"));
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (message),"%s.",error->message);
		gtk_dialog_run (GTK_DIALOG (message));
		gtk_widget_destroy (message);
		g_error_free (error);
	}
	g_strfreev( argv );

	return( ret );
}

void img_set_buttons_state(img_window_struct *img, gboolean state)
{
	gtk_widget_set_sensitive(img->import_button,state);
	gtk_widget_set_sensitive(img->import_menu,	state);
	gtk_widget_set_sensitive(img->save_menu,	state);
	gtk_widget_set_sensitive(img->save_as_menu,	state);
	gtk_widget_set_sensitive(img->save_button,	state);
	gtk_widget_set_sensitive(img->close_menu,	state);
	gtk_widget_set_sensitive(img->properties_menu,	state);
	gtk_widget_set_sensitive(img->preview_menu,	state);
	gtk_widget_set_sensitive(img->preview_button,state);
	gtk_widget_set_sensitive(img->export_menu,	state);
	gtk_widget_set_sensitive(img->export_button,state);
}

static void img_export_calc_slide_frames(img_window_struct *img)
{

	/* This is fix for the progress bars. I forgot that slides with no
	 * transition effect shouldn't have transition time added.
	 * (I haven't caught that bug when testing because I created slideshow
	 * where each slide had transition effect set.) */
	if( img->current_slide->render )
		/* Duration + transition time */
		img->export_slide_nr = ( img->current_slide->duration + img->current_slide->speed ) * EXPORT_FPS;
	else
		/* Duration only */
		img->export_slide_nr = img->current_slide->duration * EXPORT_FPS;

	img->export_slide_cur = 0;
}

static void img_export_pause_unpause(GtkToggleButton *button, img_window_struct *img)
{
	if( gtk_toggle_button_get_active( button ) )
		/* Pause export */
		g_source_remove( img->source_id );
	else
		img->source_id = g_idle_add(img->export_idle_func, img);
}
