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

void img_add_slides_thumbnails(GtkMenuItem *item,img_window_struct *img)
{
	GSList *slides = NULL;
	GdkPixbuf *thumb;
	GtkTreeIter iter;

	slides = img_import_slides_file_chooser(img);
	if (slides == NULL)
		return;

	//img->progress_window = img_create_progress_window(img);
	gtk_tree_model_get_iter_first((GtkTreeModel *)img->thumbnail_model,&iter);
	while (slides)
	{
		thumb = gdk_pixbuf_new_from_file_at_scale(slides->data, 93, 70, TRUE, NULL);
		//g_print ("%d %d\n",gdk_pixbuf_get_width(thumb),gdk_pixbuf_get_height(thumb));
		gtk_list_store_set (img->thumbnail_model, &iter, 0, thumb, 1, NULL,-1);
		g_object_unref (thumb);
		slides = slides->next;
	}
	g_slist_foreach(slides,(GFunc) g_free,NULL);
	g_slist_free(slides);
}

GSList *img_import_slides_file_chooser(img_window_struct *img)
{
	GtkWidget *import_slide;
	GSList *slides = NULL;
	gchar *filename;
	int response;

	import_slide = gtk_file_chooser_dialog_new (_("Import pictures, use SHIFT key for multiple select"),
						GTK_WINDOW (img->imagination_window),
						GTK_FILE_CHOOSER_ACTION_OPEN,
						GTK_STOCK_CANCEL,
						GTK_RESPONSE_CANCEL,
						"gtk-open",
						GTK_RESPONSE_ACCEPT,
						NULL);
	gtk_file_chooser_set_select_multiple((GtkFileChooser *)import_slide,TRUE);
	response = gtk_dialog_run ((GtkDialog *)import_slide);
	if (response == GTK_RESPONSE_ACCEPT)
		slides = gtk_file_chooser_get_filenames((GtkFileChooser *)import_slide);

	gtk_widget_destroy (import_slide);
	return slides;
}

void img_quit_application(GtkMenuItem *menuitem,gpointer user_data)
{
	gtk_main_quit();
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
