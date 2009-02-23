/*
 *  Copyright (C) 2009 Giuseppe Torelli - <colossus73@gmail.com>
 *  Copyright (c) 2009 Tadej Borovšak 	<tadeboro@gmail.com>
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */

#include "slideshow_project.h"

void img_save_slideshow(img_window_struct *img)
{
	GKeyFile *img_key_file;
	gchar *conf,*string;
	gint count = 0;
	gsize len;
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	if (!gtk_tree_model_get_iter_first (model,&iter))
		return;

	img_key_file = g_key_file_new();

	/* Slideshow settings */
	g_key_file_set_comment(img_key_file, NULL, NULL, comment_string, NULL);

	g_key_file_set_string(img_key_file,"slideshow settings","name",img->slideshow_filename);
	g_key_file_set_integer(img_key_file,"slideshow settings","export format",img->slideshow_format_index);

	if ((img->image_area)->allocation.height == 480)
		g_key_file_set_integer(img_key_file,"slideshow settings","video format",480);
	else
		g_key_file_set_integer(img_key_file,"slideshow settings","video format",576);
	g_key_file_set_string(img_key_file,"slideshow settings","aspect ratio",img->aspect_ratio);
	conf = g_strdup_printf( "%lx", (gulong)img->background_color );
	g_key_file_set_string(img_key_file,"slideshow settings", "background color", conf);
	g_free( conf );
	g_key_file_set_boolean(img_key_file,"slideshow settings", "distort images", img->distort_images);

	/* Slide individual settings */
	g_key_file_set_integer(img_key_file, "images", "number", img->slides_nr);
	do
	{
		count++;
		gtk_tree_model_get(model, &iter,1,&entry,-1);
		conf = g_strdup_printf("image_%d",count);

		g_key_file_set_string(img_key_file, "images",			conf, entry->filename);
		g_key_file_set_double(img_key_file, "transition speed", conf, entry->speed);
		g_key_file_set_integer(img_key_file,"slide duration",	conf, entry->duration);
		g_key_file_set_integer(img_key_file,"transition type",	conf, entry->combo_transition_type_index);
		g_free(conf);
	}
	while (gtk_tree_model_iter_next (model,&iter));

	/* Write the project file */
	conf = g_key_file_to_data(img_key_file, &len, NULL);
	g_file_set_contents(img->project_filename, conf, len, NULL);
	g_free (conf);

	string = g_path_get_basename(img->project_filename);
	img_set_window_title(img,string);
	g_free(string);
	g_key_file_free(img_key_file);
}

void img_load_slideshow(img_window_struct *img)
{
	GdkPixbuf *thumb;
	slide_struct *slide_info;
	GtkTreeIter iter;
	GtkTreePath *path;
	GKeyFile *img_key_file;
	gchar *dummy,*slide_filename;
	GtkWidget *dialog;
	gint number,i,duration,combo_transition_type_index, height;
	gdouble speed;
	GtkTreeModel *model;
	void (*render);

	img_key_file = g_key_file_new();
	if(!g_key_file_load_from_file(img_key_file,img->project_filename,G_KEY_FILE_KEEP_COMMENTS,NULL))
	{
		g_key_file_free( img_key_file );
		return;
	}

	dummy = g_key_file_get_comment(img_key_file,NULL,NULL,NULL);

	if (strncmp(dummy,comment_string,strlen(comment_string)) != 0)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(img->imagination_window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,_("This is not an Imagination project file!"));
		gtk_window_set_title(GTK_WINDOW(dialog),"Imagination");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (GTK_WIDGET (dialog));
		g_free(dummy);
		return;
	}
	g_free(dummy);

	if (img->slideshow_filename)
	{
		g_free(img->slideshow_filename);
		img->slideshow_filename = NULL;
	}
	/* Set the slideshow options */
	img->slideshow_filename 	= g_key_file_get_string(img_key_file,"slideshow settings","name",NULL);
	img->slideshow_format_index = g_key_file_get_integer(img_key_file,"slideshow settings","export format",NULL);
	img->aspect_ratio			= g_key_file_get_string(img_key_file,"slideshow settings","aspect ratio",NULL);
	height = g_key_file_get_integer(img_key_file,"slideshow settings","video format",NULL);
	gtk_widget_set_size_request( img->image_area, 720, height );
	dummy = g_key_file_get_string(img_key_file, "slideshow settings", "background color", NULL );
	img->background_color = (guint32)strtol( dummy, NULL, 16 );
	g_free(dummy);
	img->distort_images = g_key_file_get_boolean(img_key_file, "slideshow settings", "disort images", NULL );

	/* Loads the thumbnails and set the slides info */
	number = g_key_file_get_integer(img_key_file,"images","number",NULL);
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(img->transition_type));
	for (i = 1; i <= number; i++)
	{
		dummy = g_strdup_printf("image_%d",i);
		slide_filename = g_key_file_get_string(img_key_file,"images",dummy,NULL);

		thumb = img_load_pixbuf_from_file(slide_filename);
		if (thumb)
		{
			speed 	=	g_key_file_get_double(img_key_file,"transition speed"	,dummy,NULL);
			duration= 	g_key_file_get_integer (img_key_file,"slide duration"	,dummy,NULL);
			combo_transition_type_index = g_key_file_get_integer(img_key_file,"transition type",dummy,NULL);

			/* Get the mem address of the transition according to the index */
			path = gtk_tree_path_new_from_indices(combo_transition_type_index,-1);
			gtk_tree_model_get_iter(model,&iter,path);
			gtk_tree_model_get(model,&iter,1,&render,-1);
			slide_info = img_set_slide_info(duration, speed, render, combo_transition_type_index, slide_filename);
			if (slide_info)
			{
				gtk_list_store_append (img->thumbnail_model,&iter);
				gtk_list_store_set (img->thumbnail_model, &iter, 0, thumb, 1, slide_info, -1);
				g_object_unref (thumb);
				img->slides_nr++;
			}
		}
		g_free(slide_filename);
		g_free(dummy);
	}
	g_key_file_free (img_key_file);
	img_set_total_slideshow_duration(img);
	img_set_statusbar_message(img,0);
	gtk_widget_show(img->thumb_scrolledwindow);

	dummy = g_path_get_basename(img->project_filename);
	img_set_window_title(img, dummy);
	img_set_buttons_state(img, TRUE);
	g_free(dummy);
}
