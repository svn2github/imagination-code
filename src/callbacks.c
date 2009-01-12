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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "callbacks.h"

static void img_file_chooser_add_preview(img_window_struct *);
static void img_update_preview_file_chooser(GtkFileChooser *,img_window_struct *);
static gboolean img_on_expose_event(GtkWidget *,GdkEventExpose *,img_window_struct *);
static gboolean img_time_handler(img_window_struct *);
double radius;

void img_new_slideshow(GtkMenuItem *item,img_window_struct *img_struct)
{
	img_new_slideshow_settings_dialog(img_struct);
}

void img_add_slides_thumbnails(GtkMenuItem *item,img_window_struct *img)
{
	GSList	*slides = NULL;
	GdkPixbuf *thumb;
	GdkPixbufFormat *pixbuf_format;
	gint width,height;
	GtkTreeIter iter;
	slide_struct *slide_info;

	slides = img_import_slides_file_chooser(img);
	if (slides == NULL)
		return;

	/*img->progress_window = img_create_progress_window(img);*/
	while (slides)
	{
		thumb = gdk_pixbuf_new_from_file_at_scale(slides->data, 93, 70, TRUE, NULL);
		if (thumb)
		{
			slide_info = g_new0(slide_struct,1);
			if (slide_info)
			{
				/* Get some slide info */
				slide_info->duration = 1;
				img->total_secs++;
				slide_info->filename = g_strdup(slides->data);
				pixbuf_format = gdk_pixbuf_get_file_info(slides->data,&width,&height);
				slide_info->resolution = g_strdup_printf("%d x %d",width,height);
				slide_info->type = gdk_pixbuf_format_get_name(pixbuf_format);
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
	img_set_statusbar_message(img);
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
	if (gtk_tree_model_get_iter (model,&iter,path) == FALSE)
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
	if (img_struct->slideshow_title)
		g_free(img_struct->slideshow_title);
	if (img_struct->current_dir)
		g_free(img_struct->current_dir);

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

void img_set_statusbar_message(img_window_struct *img_struct)
{
	gchar *message = NULL;

	if (img_struct->slides_nr == 0)
		gtk_statusbar_push((GtkStatusbar*)img_struct->statusbar,img_struct->context_id,_("Welcome to Imagination " VERSION));
	else
	{
		message = g_strdup_printf(ngettext("%d slide %s" ,"%d slides %s",img_struct->slides_nr),img_struct->slides_nr,"imported");
		gtk_statusbar_push((GtkStatusbar*)img_struct->statusbar,img_struct->context_id,message);
		g_free(message);
	}
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
	img_set_statusbar_message(img_struct);
	gtk_image_set_from_pixbuf((GtkImage*)img_struct->image_area,NULL);
}

void img_show_about_dialog (GtkMenuItem *item,img_window_struct *img_struct)
{
	static GtkWidget *about = NULL;
    const char *authors[] = {"\nMain developer:\nGiuseppe Torelli <colossus73@gmail.com>\n",NULL};
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
			"copyright","Copyright \xC2\xA9 2008 Giuseppe Torelli",
			"comments","A simple and lightweight DVD slideshow maker",
			"authors",authors,
			"documenters",documenters,
			"translator_credits",_("translator-credits"),
			"logo_icon_name","imagination",
			"website","http://imagination.sf.net",
			"license","Copyright \xC2\xA9 2008 Giuseppe Torelli - Colossus <colossus73@gmail.com>\n\n"
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
	GtkTreePath *path;
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	path = gtk_tree_path_new_first();
	if (gtk_tree_model_get_iter (model,&iter,path) == FALSE)
		goto here;

	img->total_secs = 0;
	do
	{
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		img->total_secs += entry->duration;
	}
	while (gtk_tree_model_iter_next (model,&iter));

	time = g_strdup_printf("%02d:%02d:%02d",img->total_secs/3600,img->total_secs/60,img->total_secs);
	gtk_label_set_text((GtkLabel*)img->total_time_data,time);
	g_free(time);

here:
	gtk_tree_path_free(path);
}

void img_start_preview(GtkButton *button, img_window_struct *img)
{
	GtkTreePath *path;
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;

	gtk_widget_set_app_paintable(img->image_area, TRUE);
	g_signal_connect( G_OBJECT(img->image_area), "expose-event",G_CALLBACK(img_on_expose_event),img);

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	path = gtk_tree_path_new_first();
	if (gtk_tree_model_get_iter (model,&iter,path) == FALSE)
		goto here;

	/* Create an empty pixbuf */
	img->pixbuf1 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, (img->viewport)->allocation.width, (img->viewport)->allocation.height);

	/* Load the first image in the pixbuf */
	gtk_tree_model_get(model, &iter,1,&entry,-1);
	img->pixbuf2 = img_scale_pixbuf(img,entry->filename);

	g_timeout_add(15,(GSourceFunc)img_time_handler,img);
	img_idle_function(entry->duration);

	while (gtk_tree_model_iter_next(model,&iter))
	{
		gtk_widget_queue_draw(img->image_area);
		gtk_tree_model_get(model, &iter,1,&entry,-1);

		img->pixbuf2 = img_scale_pixbuf(img,entry->filename);
		g_object_unref(img->pixbuf1);

		img->pixbuf1 = gdk_pixbuf_copy(img->pixbuf2);

		img_idle_function(entry->duration);
		g_object_ref_sink(img->pixbuf2);
	}

here:
	gtk_tree_path_free(path);
	gtk_widget_set_app_paintable(img->image_area, FALSE);
	g_signal_handlers_disconnect_by_func(img->image_area,img_on_expose_event,NULL);
}

static gboolean img_on_expose_event(GtkWidget *widget,GdkEventExpose *event,img_window_struct *img)
{
	cairo_t *cr;
	gint offset_x,offset_y;

	offset_x = ((img->image_area)->allocation.width - gdk_pixbuf_get_width(img->pixbuf1)) / 2;
	offset_y = ((img->image_area)->allocation.height - gdk_pixbuf_get_height(img->pixbuf1)) / 2;
	cr = gdk_cairo_create(widget->window);
	gdk_cairo_set_source_pixbuf(cr,img->pixbuf1,offset_x,offset_y);
	cairo_paint(cr);

	offset_x = ((img->image_area)->allocation.width - gdk_pixbuf_get_width(img->pixbuf2)) / 2;
	offset_y = ((img->image_area)->allocation.height - gdk_pixbuf_get_height(img->pixbuf2)) / 2;
	gdk_cairo_set_source_pixbuf(cr,img->pixbuf2,offset_x,offset_y);
	cairo_arc(cr, gdk_pixbuf_get_width(img->pixbuf2)/2, gdk_pixbuf_get_height(img->pixbuf2)/2, radius, 0, 2 * G_PI );
	cairo_clip(cr);
	cairo_paint(cr);

	cairo_destroy(cr);
	return( FALSE );
}

static gboolean img_time_handler(img_window_struct *img)
{
	gboolean value;

	radius++;
	if (radius <= 0)
		value = TRUE;

	gtk_widget_queue_draw(img->image_area);

	if(radius > 450)
	{
		g_print ("FALSE\n");
		value = FALSE;
	}

	return value;
}

GdkPixbuf *img_scale_pixbuf (img_window_struct *img, gchar *filename)
{
	GdkPixbuf *pixbuf;
	GdkPixbufFormat *image_format;
	gint width,height;

	image_format = gdk_pixbuf_get_file_info(filename,&width,&height);
	if (width > (img->viewport)->allocation.width || height > (img->viewport)->allocation.height)
		pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, (img->viewport)->allocation.width, (img->viewport)->allocation.height, TRUE, NULL);
	else
		pixbuf = gdk_pixbuf_new_from_file(filename,NULL);

	return pixbuf;
}
