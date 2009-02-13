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

#include "callbacks.h"
#include <unistd.h>

static void img_file_chooser_add_preview(img_window_struct *);
static void img_update_preview_file_chooser(GtkFileChooser *,img_window_struct *);
static gboolean img_on_expose_event(GtkWidget *,GdkEventExpose *,img_window_struct *);
static gboolean img_transition_timeout(img_window_struct *);
static gboolean img_sleep_timeout(img_window_struct *);
static gboolean img_prepare_pixbufs(img_window_struct *);
static void img_swap_toolbar_images( img_window_struct *, gboolean);
static void img_clean_after_preview(img_window_struct *img);

/* Export related functions */
static gboolean img_export_transition(img_window_struct *);
static gboolean img_export_still(img_window_struct *);
static void img_clean_after_export(img_window_struct *);
static void img_export_pixbuf_to_ppm(GdkPixbuf *, guchar **, guint *);

void img_set_window_title(img_window_struct *img, gchar *text)
{
	gchar *title = NULL;

	if (text == NULL)
	{
		title = g_strconcat("Imagination " VERSION,NULL);
		gtk_window_set_title (GTK_WINDOW (img->imagination_window), title);
		g_free(title);
	}
	else
		gtk_window_set_title (GTK_WINDOW (img->imagination_window), text);
}

void img_new_slideshow(GtkMenuItem *item,img_window_struct *img_struct)
{
	img_new_slideshow_settings_dialog(img_struct);
}

void img_add_slides_thumbnails(GtkMenuItem *item,img_window_struct *img)
{
	GSList	*slides = NULL;
	GdkPixbuf *thumb;
	GtkTreeIter iter;
	slide_struct *slide_info;

	slides = img_import_slides_file_chooser(img);
	if (slides == NULL)
		return;

	/*img->progress_window = img_create_progress_window(img);*/
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
				img->slides_nr++;
			}
			g_free(slides->data);
		}
		slides = slides->next;
	}
	g_slist_free(slides);
	img_set_total_slideshow_duration(img);
	img_set_statusbar_message(img,0);
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
						"gtk-open",
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
	gtk_file_chooser_add_filter((GtkFileChooser*)img->import_slide_chooser,all_images_filter);

	/* All files filter */
	all_files_filter = gtk_file_filter_new ();
	gtk_file_filter_set_name(all_files_filter,_("All files"));
	gtk_file_filter_add_pattern(all_files_filter,"*");
	gtk_file_chooser_add_filter((GtkFileChooser*)img->import_slide_chooser,all_files_filter);

	gtk_file_chooser_set_select_multiple((GtkFileChooser *)img->import_slide_chooser,TRUE);
	if (img->current_dir)
		gtk_file_chooser_set_current_folder((GtkFileChooser*)img->import_slide_chooser,img->current_dir);
	response = gtk_dialog_run ((GtkDialog *)img->import_slide_chooser);
	if (response == GTK_RESPONSE_ACCEPT)
	{
		slides = gtk_file_chooser_get_filenames((GtkFileChooser *)img->import_slide_chooser);
		if (img->current_dir)
			g_free(img->current_dir);
		img->current_dir = gtk_file_chooser_get_current_folder((GtkFileChooser*)img->import_slide_chooser);
	}
	gtk_widget_destroy (img->import_slide_chooser);
	return slides;
}

gboolean img_quit_application(GtkWidget *widget, GdkEvent *event, img_window_struct *img_struct)
{
	GtkTreeModel *model;
	GtkTreePath  *path = NULL;
	GtkTreeIter iter;
	slide_struct *entry;

	if (img_struct->slides_nr == 0)
		goto quit;

	model = gtk_icon_view_get_model ((GtkIconView*) img_struct->thumbnail_iconview);
	path = gtk_tree_path_new_first();

	/* Free the slide struct for each slide */
	if (gtk_tree_model_get_iter(model,&iter,path) == FALSE)
		goto quit;
	do
	{
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		g_free(entry->filename);
		g_free(entry->resolution);
		g_free(entry->type);
		g_free(entry);
		
	}
	while (gtk_tree_model_iter_next (model,&iter));

quit:
	gtk_tree_path_free(path);
	if (img_struct->slideshow_filename)
		g_free(img_struct->slideshow_filename);
	if (img_struct->current_dir)
		g_free(img_struct->current_dir);

	/* Unloads the plugins */
	g_slist_foreach(img_struct->plugin_list,(GFunc)g_module_close,NULL);
	g_slist_free(img_struct->plugin_list);

	gtk_main_quit();
	return FALSE;
}

static void img_file_chooser_add_preview(img_window_struct *img_struct)
{
	GtkWidget *vbox;

	vbox = gtk_vbox_new (FALSE, 5);
	gtk_container_set_border_width ((GtkContainer *)vbox, 10);

	img_struct->preview_image = gtk_image_new ();

	img_struct->dim_label  = gtk_label_new (NULL);
	img_struct->size_label = gtk_label_new (NULL);

	gtk_box_pack_start (GTK_BOX (vbox), img_struct->preview_image, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), img_struct->dim_label, FALSE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox), img_struct->size_label, FALSE, TRUE, 0);
	gtk_widget_show_all (vbox);

	gtk_file_chooser_set_preview_widget ((GtkFileChooser*)img_struct->import_slide_chooser, vbox);
	gtk_file_chooser_set_preview_widget_active ((GtkFileChooser*)img_struct->import_slide_chooser, FALSE);

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
		gtk_image_set_from_pixbuf ((GtkImage*)img_struct->preview_image, pixbuf);
		g_object_unref (pixbuf);

		size = g_strdup_printf("%d x %d pixels",width,height);
		gtk_label_set_text((GtkLabel*)img_struct->dim_label,size);
		g_free(size);
	}
	g_free(filename);
	gtk_file_chooser_set_preview_widget_active (file_chooser, has_preview);
}

void img_delete_selected_slides(GtkMenuItem *item,img_window_struct *img_struct)
{
	GList *selected;
	GtkTreeIter iter;
	GtkTreeModel *model;
	slide_struct *entry;

	model = gtk_icon_view_get_model((GtkIconView *)img_struct->thumbnail_iconview);
	selected = gtk_icon_view_get_selected_items ((GtkIconView *)img_struct->thumbnail_iconview);
	if (selected == NULL)
		return;
	
	/* Free the slide struct for each slide and remove it from the iconview */
	while (selected)
  	{
  		gtk_tree_model_get_iter(model, &iter,selected->data);
  		gtk_tree_model_get(model, &iter,1,&entry,-1);
  		g_free(entry->filename);
  		g_free(entry->resolution);
  		g_free(entry->type);
  		g_free(entry);
  		gtk_list_store_remove((GtkListStore*) img_struct->thumbnail_model,&iter);
  		img_struct->slides_nr--;
  		selected = selected->next;
  	}
	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(selected);
	img_set_statusbar_message(img_struct,0);
	gtk_image_set_from_pixbuf((GtkImage*)img_struct->image_area,NULL);
}

void img_show_about_dialog (GtkMenuItem *item,img_window_struct *img_struct)
{
	static GtkWidget *about = NULL;
    const char *authors[] = {"\nMain developer:\nGiuseppe Torelli <colossus73@gmail.com>\n\nCode improvements and patches:\nTadej Borovšak",NULL};
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
			"version",PACKAGE_VERSION,
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
		/* With new fixed-time transitions we can calculate duration
		 * more precisely. */
		/*if (entry->speed == FAST && entry->render)
			img->total_secs += 1;
		else if (entry->speed == NORMAL && entry->render)
			img->total_secs += 3;
		else if (entry->speed == SLOW && entry->render)
			img->total_secs += 13;*/
		if(entry->render)
			img->total_secs += (1 / entry->speed) / 25;
	}
	while (gtk_tree_model_iter_next (model,&iter));

	/* Fix secs -> hour, mins and secs conversion */
	h =  img->total_secs / 3600;
	m = (img->total_secs % 3600) / 60;
	s =  img->total_secs - (h * 3600) - (m * 60);
	time = g_strdup_printf("%02d:%02d:%02d", h, m, s);
	gtk_label_set_text((GtkLabel*)img->total_time_data,time);
	g_free(time);
}

void img_start_stop_preview(GtkWidget *button, img_window_struct *img)
{
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	if( ! gtk_tree_model_get_iter_first (model,&iter))
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

		/* Create an empty pixbuf - starting white image */
		img->pixbuf1 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,img->image_area->allocation.width,img->image_area->allocation.height);
		gdk_pixbuf_fill(img->pixbuf1,0xffffffff);

		/* Load the first image in the pixbuf */
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		img->pixbuf2 = img_scale_pixbuf(img,entry->filename);

		/* Add transition timeout function */
		img->preview_is_running = TRUE;
		img->current_slide = entry;
		img->progress = 0;
		img->source_id = g_timeout_add(TRANSITION_TIMEOUT,(GSourceFunc)img_transition_timeout,img);
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

GdkPixbuf *img_scale_pixbuf (img_window_struct *img, gchar *filename)
{
	GdkPixbuf *pixbuf;
	GdkPixbuf *compose;
	GdkPixbufFormat *format;
	gint image_width, image_height;
	gint width, height;
	gint offset_x, offset_y;

	width  = img->image_area->allocation.width;
	height = img->image_area->allocation.height;

	format = gdk_pixbuf_get_file_info(filename, &image_width, &image_height);
	if(image_width < width && image_height < height)
		pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
	else
		pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, width, height, TRUE, NULL);

	image_width  = gdk_pixbuf_get_width(pixbuf);
	image_height = gdk_pixbuf_get_height(pixbuf);
	offset_x = (width - image_width) / 2;
	offset_y = (height - image_height) / 2;

	compose = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
	gdk_pixbuf_fill(compose, 0xffffffff);
	gdk_pixbuf_composite(pixbuf, compose, offset_x, offset_y, image_width, image_height, offset_x, offset_y, 1, 1, GDK_INTERP_BILINEAR, 255 );
	g_object_unref(G_OBJECT(pixbuf));

	return compose;
}

static gboolean img_transition_timeout(img_window_struct *img)
{
	/* Increment progress variable (this is being passed as a parameter
	 * to plug-in provided transition function). */
	img->progress += (img->current_slide)->speed;
	
	/* if the transition render is NULL (no transition is set for this
	 * slide), we stop this timeout function, but still invalidate
	 * preview area so expose event gets called. */
	if( img->current_slide->render == NULL )
	{
		img->progress = 0;
		img->source_id = g_timeout_add( img->current_slide->duration * 1000, (GSourceFunc)img_sleep_timeout, img );
		gtk_widget_queue_draw( img->image_area );
		return( FALSE );
	}

	/* If the progress reached 1, the transition should be finished and
	 * it's time to stop this timeout function and add sleep timeout
	 * function to the main loop. */
	if( img->progress > (1 + img->current_slide->speed / 2) )
	{
		img->progress = 0;
		img->source_id = g_timeout_add( img->current_slide->duration * 1000,(GSourceFunc)img_sleep_timeout, img );
		return( FALSE );
	}

	/* Schedule our image redraw */
	gtk_widget_queue_draw( img->image_area );

	return( TRUE );
}

static gboolean img_sleep_timeout(img_window_struct *img)
{
	if(img_prepare_pixbufs(img))
	{
		img->source_id = g_timeout_add( TRANSITION_TIMEOUT,(GSourceFunc)img_transition_timeout,img );
	}
	else
	{
		/* Clean resources used in preview and prepare application for
		 * next preview. */
		img_clean_after_preview(img);
	}

	return( FALSE );
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
	gchar *filename = NULL;
	GtkWidget *fc;
	GtkFileChooserAction action = 0;
	gint response;

	if (widget == img->open_menu_item || widget == img->open_button)
		action = GTK_FILE_CHOOSER_ACTION_OPEN;
	else if (widget == img->save_as_menu_item || widget == img->save_menu_item || widget == img->save_button)
		action = GTK_FILE_CHOOSER_ACTION_SAVE;

	if (img->slideshow_filename == NULL || widget == img->save_as_menu_item || action == GTK_FILE_CHOOSER_ACTION_OPEN)
	{
		fc = gtk_file_chooser_dialog_new (action == GTK_FILE_CHOOSER_ACTION_OPEN ? _("Load an Imagination slideshow project") : 
					_("Save an Imagination slideshow project"),
					GTK_WINDOW (img->imagination_window),
					action,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					action == GTK_FILE_CHOOSER_ACTION_OPEN ?  "gtk-open" : "gtk-save",
					GTK_RESPONSE_ACCEPT,NULL);

		gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER (fc),TRUE);
		response = gtk_dialog_run ((GtkDialog *)fc);
		if (response == GTK_RESPONSE_ACCEPT)
		{
			filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fc));
			gtk_widget_destroy(fc);
		}
		else if (response == GTK_RESPONSE_CANCEL || GTK_RESPONSE_DELETE_EVENT)
		{
			gtk_widget_destroy(fc);
			return;
		}
	}
	else
		filename = g_strdup(img->slideshow_filename);

	if (action == GTK_FILE_CHOOSER_ACTION_OPEN)
		img_load_slideshow(img,filename);
	else
		img_save_slideshow(img,filename);
	
	if (filename)
		g_free(filename);
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
		img->file_desc = g_open("/tmp/img.fifo",O_WRONLY,0);

		/* Create progress window with cancel and pause buttons, calculate the total number of frames to display */
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

		/* Add export idle function */
		img->export_is_running = TRUE;
		img->current_slide = entry;
		img->progress = 0;
		img->source_id = g_idle_add((GSourceFunc)img_export_transition, img);
	}
}

static gboolean img_export_transition(img_window_struct *img)
{
	/* Counter, used to draw every 10th frame. */
	static guint counter = 0;

	/* If no transition effect is set, just connect still export
	 * idle function and remove itself from main loop. */
	if(img->current_slide->render == NULL)
	{
		img->source_id = g_idle_add((GSourceFunc)img_export_still, img);
		return(FALSE);
	}

	img->progress += img->current_slide->speed * (25 / 29.97);
	if(img->progress > 1 + 0.00000005)
	{
		img->progress = 0;
		counter = 0;
		img->source_id = g_idle_add((GSourceFunc)img_export_still, img);

		return(FALSE);
	}

	/* Draw one frame of transition animation */
	img->current_slide->render(img->image_area->window, img->pixbuf1, img->pixbuf2, img->progress, img->file_desc);
	counter++;

	/* Draw every 10th frame of animation on screen */
	if(counter % 10 == 0)
		gtk_widget_queue_draw(img->image_area);

	return(TRUE);
}

static gboolean img_export_still(img_window_struct *img)
{
	static guint   frame_counter;
	static guint   lenght;

	/* Initialize pixbuf data buffer */
	if(img->pixbuf_data == NULL)
	{
		frame_counter = 0;
		gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area), img->pixbuf2);
		img_export_pixbuf_to_ppm(img->pixbuf2, &img->pixbuf_data, &lenght);
	}

	/* FIXED RATE!!!
	 * Draw frames until we have enough of them to fill slide duration gap.
	 * Again, output rate is fixed at 29.97 fps. */
	if(frame_counter > img->current_slide->duration * 29.97)
	{
		/* Exit still rendering and continue with next transition. */

		/* Clear image area for next renderer */
		gtk_image_clear(GTK_IMAGE(img->image_area));

		/* Load next image from store. */
		if(img_prepare_pixbufs(img))
		{
			g_free(img->pixbuf_data);
			img->pixbuf_data = NULL;
			img->source_id = g_idle_add((GSourceFunc)img_export_transition, img);
		}
		else
			img_clean_after_export(img);

		return(FALSE);
	}
	write(img->file_desc, img->pixbuf_data, lenght);
	frame_counter++;

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

	close(img->file_desc);
	//g_unlink("/tmp/img.fifo");
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
