/*
 *  Copyright (C) 2009 Giuseppe Torelli - <colossus73@gmail.com>
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

void img_save_slideshow(img_window_struct *img, gchar *filename)
{
	GKeyFile *img_key_file;
	GError *error;
	gchar *conf;
	gint len,count = 0;
	FILE *fp;
	size_t bytes_written;
	GtkTreeIter iter;
	slide_struct *entry;
	GtkTreeModel *model;

	img_key_file = g_key_file_new();
	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	if (!gtk_tree_model_get_iter_first (model,&iter))
		return;

g_print("Salvo %d - %s\n",img->slides_nr,filename);

	/* Slideshow settings */
	g_key_file_set_comment(img_key_file,NULL,NULL,"Imagination 1.0 Slideshow Project - http://imagination.sf.net",NULL);

	g_key_file_set_string(img_key_file,"slideshow settings","name",img->slideshow_filename);
	g_key_file_set_integer(img_key_file,"slideshow settings","export format",img->slideshow_format_index);
	if ((img->viewport)->allocation.height == 480)
		g_key_file_set_string(img_key_file,"slideshow settings","video format","NTSC");
	else
		g_key_file_set_string(img_key_file,"slideshow settings","video format","PAL");
	g_key_file_set_string(img_key_file,"slideshow settings","aspect ratio",img->aspect_ratio);

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

	conf = g_key_file_to_data(img_key_file, NULL, NULL);
	len = strlen(conf);

	fp = fopen(filename, "w");
	if (fp != NULL)
	{
		bytes_written = fwrite(conf, sizeof (gchar), len, fp);
		fclose(fp);
	}
	g_free (conf);
	g_key_file_free(img_key_file);
}

void img_load_slideshow(img_window_struct *img, gchar *filename)
{
	GKeyFile *img_key_file;
	GError *error;

	img_key_file = g_key_file_new();
	g_key_file_load_from_file(img_key_file,filename,G_KEY_FILE_KEEP_COMMENTS,NULL);

	/*gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_prefered_format),g_key_file_get_integer(xa_key_file,PACKAGE,"preferred_format",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->confirm_deletion),g_key_file_get_boolean(xa_key_file,PACKAGE,"confirm_deletion",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->store_output),g_key_file_get_boolean(xa_key_file,PACKAGE,"store_output",NULL));

	gtk_combo_box_set_active (GTK_COMBO_BOX(prefs_data->combo_icon_size),g_key_file_get_integer(xa_key_file,PACKAGE,"icon_size",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->check_show_comment),g_key_file_get_boolean(xa_key_file,PACKAGE,"show_archive_comment",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->check_sort_filename_column),g_key_file_get_boolean(xa_key_file,PACKAGE,"sort_filename_content",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->show_sidebar),g_key_file_get_boolean(xa_key_file,PACKAGE,"show_sidebar",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(prefs_data->show_location_bar),g_key_file_get_boolean(xa_key_file,PACKAGE,"show_location_bar",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (prefs_data->allow_sub_dir),g_key_file_get_boolean(xa_key_file,PACKAGE,"allow_sub_dir",NULL));
	value = g_key_file_get_string(xa_key_file,PACKAGE,"preferred_editor",NULL);
	coords = g_key_file_get_integer_list(xa_key_file, PACKAGE, "mainwindow", &coords_len, &error);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(extract_window->overwrite_check),g_key_file_get_boolean(xa_key_file,PACKAGE,"overwrite",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(extract_window->extract_full),g_key_file_get_boolean(xa_key_file,PACKAGE,"full_path",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(extract_window->touch),g_key_file_get_boolean(xa_key_file,PACKAGE,"touch",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(extract_window->fresh),g_key_file_get_boolean(xa_key_file,PACKAGE,"fresh",NULL));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(extract_window->update),g_key_file_get_boolean(xa_key_file,PACKAGE,"update",NULL));
	Load the options in the add dialog */
	g_key_file_free (img_key_file);
}
