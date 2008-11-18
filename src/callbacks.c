/*
 *  Copyright (c) 2008 Giuseppe Torelli <colossus73@gmail.com>
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

#include <gtk/gtk.h>

#include "main-window.h"
#include "callbacks.h"
#include "support.h"

/* pixbuf = img.get_pixbuf()
    pixbuf.save(filename, "jpeg", {"quality":"100"})
*/

static void img_file_chooser_add_preview(img_window_struct *);
static void	img_update_preview_file_chooser(GtkFileChooser *,img_window_struct *);

void img_add_slides_thumbnails(GtkMenuItem *item,img_window_struct *img)
{
	GSList *slides = NULL;
	GdkPixbuf *thumb;
	GtkTreeIter iter;

	slides = img_import_slides_file_chooser(img);
	if (slides == NULL)
		return;

	//img->progress_window = img_create_progress_window(img);
	while (slides)
	{
		thumb = gdk_pixbuf_new_from_file_at_scale(slides->data, 93, 70, TRUE, NULL);
		if (thumb)
		{
			gtk_list_store_append (img->thumbnail_model,&iter);
			gtk_list_store_set (img->thumbnail_model, &iter, 0, thumb, 1, NULL,-1);
			g_object_unref (thumb);
			img->slides_nr++;
		}
		slides = slides->next;
	}
	g_slist_foreach(slides,(GFunc) g_free,NULL);
	g_slist_free(slides);
}

GSList *img_import_slides_file_chooser(img_window_struct *img)
{
	GtkFileFilter *all_images_filter, *all_files_filter;
	gchar **mime_types;
	GSList *slides = NULL, *formats = NULL, *_formats = NULL;
	int response,i;

	img->import_slide_chooser = gtk_file_chooser_dialog_new (_("Import pictures, use SHIFT key for multiple select"),
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

void img_quit_application(GtkMenuItem *menuitem,img_window_struct *img_struct)
{
	gtk_main_quit();
}

void img_thumb_view_select_slide(img_window_struct *img_struct, Img_Thumbnail_Selection_Mode mode)
{
	GtkTreePath *path = NULL;
	GtkTreeModel *model;
	GList *list;
	gint n_slides;

	model = gtk_icon_view_get_model ((GtkIconView*) img_struct->thumbnail_iconview);
	n_slides = gtk_tree_model_iter_n_children ((GtkTreeModel*) model, NULL);

	if (n_slides == 0)
		return;

	switch (mode)
	{
		case IMG_CURRENT_SLIDE:
			path = gtk_tree_path_new_from_indices (n_slides - 1, -1);
			//path = gtk_icon_view_get_path_at_pos((GtkIconView*) img_struct->thumbnail_iconview,gdk_pointer_grab(,y);
		break;
		
		case IMG_CURRENT_LEFT:
		break;
		
		case IMG_CURRENT_RIGHT:
		break;
	}
	gtk_icon_view_select_path ((GtkIconView*) img_struct->thumbnail_iconview, path);
	gtk_icon_view_set_cursor ((GtkIconView*) img_struct->thumbnail_iconview, path, NULL, FALSE);
	gtk_icon_view_scroll_to_path ((GtkIconView*) img_struct->thumbnail_iconview, path, FALSE, 0, 0);
	gtk_tree_path_free (path);
}

static void img_file_chooser_add_preview(img_window_struct *img_struct)
{
	GtkWidget *vbox;

	vbox = gtk_vbox_new (FALSE, 5);
	gtk_container_set_border_width ((GtkContainer *)vbox, 10);

	img_struct->preview_image = gtk_image_new ();
	gtk_widget_set_size_request (img_struct->preview_image, 128, 128);

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
	GdkPixbuf *pixbuf,*pixbuf_scaled;

	filename = gtk_file_chooser_get_filename(file_chooser);
	if (filename == NULL)
	{
		gtk_file_chooser_set_preview_widget_active (file_chooser, has_preview);
		return;
	}
	pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
	g_free(filename);	
	has_preview = (pixbuf != NULL);
	if (has_preview)
	{
		width	=	gdk_pixbuf_get_width(pixbuf);
		height	=	gdk_pixbuf_get_height(pixbuf);
		pixbuf_scaled = gdk_pixbuf_scale_simple(pixbuf, 128, 96, GDK_INTERP_BILINEAR);
		g_object_unref (pixbuf);

		gtk_image_set_from_pixbuf ((GtkImage*)img_struct->preview_image, pixbuf_scaled);
		g_object_unref (pixbuf_scaled);

		size = g_strdup_printf("%d x %d",width,height);
		gtk_label_set_text((GtkLabel*)img_struct->dim_label,size);
		g_free(size);
	}
	gtk_file_chooser_set_preview_widget_active (file_chooser, has_preview);
}

void img_show_about_dialog (GtkMenuItem *item,img_window_struct *img_struct)
{
	static GtkWidget *about = NULL;
    const char *authors[] = {"\nMain developer:\nGiuseppe Torelli <colossus73@gmail.com>\n",NULL};
    const char *documenters[] = {NULL};

	if (about == NULL)
	{
		about = gtk_about_dialog_new ();
		/*gtk_about_dialog_set_email_hook (xa_activate_link,NULL,NULL);
		gtk_about_dialog_set_url_hook (xa_activate_link,NULL,NULL);*/
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
