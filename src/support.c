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

#include "support.h"
#include "imagination.h"

GtkWidget *img_load_icon(gchar *filename, GtkIconSize size)
{
    GtkWidget *file_image;
	gchar *path;
	GdkPixbuf *file_pixbuf = NULL;
	path = g_strconcat(DATADIR, "/pixmaps/imagination/",filename,NULL);
	file_pixbuf = gdk_pixbuf_new_from_file(path,NULL);
	g_free (path);

	if (file_pixbuf == NULL)
		file_image = gtk_image_new_from_stock(GTK_STOCK_MISSING_IMAGE, size);
    else
	{
		file_image = gtk_image_new_from_pixbuf(file_pixbuf);
	    g_object_unref (file_pixbuf);
	}
    return file_image;
}
/*
cairo_surface_t *img_get_cairo_surface_from_gdk_pixbuf(GdkPixbuf *pixbuf)
{
	cairo_surface_t *image;
	cairo_t *cr;

	image = cairo_image_surface_create (gdk_pixbuf_get_has_alpha (pixbuf) ?
					      CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24,gdk_pixbuf_get_width (pixbuf),gdk_pixbuf_get_height (pixbuf));

	cr = cairo_create (image);
	gdk_cairo_set_source_pixbuf (cr, pixbuf, 0, 0);
	cairo_paint (cr);
	cairo_destroy (cr);
	//g_object_unref(pixbuf);
	return image;
}
*/

GtkWidget *_gtk_combo_box_new_text(gboolean pointer)
{
	GtkWidget *combo_box;
	GtkCellRenderer *cell;
	GtkListStore *store;

	if (pointer)
		store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
	else
		store = gtk_list_store_new (1, G_TYPE_STRING);
	combo_box = gtk_combo_box_new_with_model (GTK_TREE_MODEL (store));
	g_object_unref (store);

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo_box), cell, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo_box), cell,"text", 0, NULL);
	g_object_set(cell,"ypad", 0.0, NULL);
	return combo_box;
}

void img_set_statusbar_message(img_window_struct *img_struct, gint selected)
{
	gchar *message = NULL;

	if (img_struct->slides_nr == 0)
	{
		message = g_strdup_printf(_("Welcome to Imagination - %d transitions loaded."),img_struct->nr_transitions_loaded);
		gtk_statusbar_push((GtkStatusbar*)img_struct->statusbar,img_struct->context_id,message);
		g_free(message);
	}
	else if (selected)
	{
		message = g_strdup_printf(_("%d slides selected"),selected);
		gtk_statusbar_push((GtkStatusbar*)img_struct->statusbar,img_struct->context_id,message);
		g_free(message);
	}
	else
	{
		message = g_strdup_printf(ngettext("%d slide %s" ,"%d slides %s",img_struct->slides_nr),img_struct->slides_nr,_("imported - Use the CTRL key to select/unselect or SHIFT for multiple select"));
		gtk_statusbar_push((GtkStatusbar*)img_struct->statusbar,img_struct->context_id,message);
		g_free(message);
	}
}

void img_load_available_transitions(img_window_struct *img)
{
	GDir *dir;
	const gchar *transition_name;
	gchar *path = NULL, *fname = NULL,*name;
	GError **error = NULL;
	GModule *module;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gpointer address;
	void (*plugin_set_name)(gchar**);

	model = gtk_combo_box_get_model(GTK_COMBO_BOX(img->transition_type));
	
	/* Fill the combo box with no transition */
	gtk_list_store_append(GTK_LIST_STORE(model), &iter);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter,0, _("None"), 1, NULL, -1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->transition_type), 0);

	path = g_strdup("./transitions");
	//path = g_strconcat(PACKAGE_LIB_DIR,"/imagination",NULL);
	dir = g_dir_open(path, 0, error);
	if (dir == NULL)
	{
		g_free(path);
		img->nr_transitions_loaded = 0;
		return;
	}
	while (1)
	{
		transition_name = g_dir_read_name(dir);
		if (transition_name == NULL)
			break;

		fname = g_build_filename(path,transition_name, NULL);
		module = g_module_open(fname, G_MODULE_BIND_LOCAL);
		if (module)
		{
			/* Obtain the name from the plugin function */
			g_module_symbol(module, "img_transition_set_name",(void *) &plugin_set_name);
			plugin_set_name(&name);
			/* And the memory address of the render function */
			g_module_symbol(module, "img_transition_render", &address);
			/* Fill the model of the combo box */
			gtk_list_store_append(GTK_LIST_STORE(model), &iter);
			gtk_list_store_set(GTK_LIST_STORE(model), &iter,0, name, 1, address, -1);
			img->plugin_list = g_slist_append(img->plugin_list, module);
			img->nr_transitions_loaded++;
		}
		g_free(fname);
	}
	g_free(path);
	g_dir_close(dir);
}

void img_show_file_chooser(SexyIconEntry *entry, SexyIconEntryPosition icon_pos,int button,img_window_struct *img)
{
	GtkWidget *file_selector;
	gchar *dest_dir;
	gint response;

	file_selector = gtk_file_chooser_dialog_new (_("Please choose the slideshow project filename"),
							GTK_WINDOW (img->imagination_window),
							GTK_FILE_CHOOSER_ACTION_SAVE,
							GTK_STOCK_CANCEL,
							GTK_RESPONSE_CANCEL,
							GTK_STOCK_SAVE,
							GTK_RESPONSE_ACCEPT,
							NULL);

	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER (file_selector),TRUE);
	response = gtk_dialog_run (GTK_DIALOG(file_selector));
	if (response == GTK_RESPONSE_ACCEPT)
	{
		dest_dir = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (file_selector));
		gtk_entry_set_text(GTK_ENTRY(entry),dest_dir);
		g_free(dest_dir);
	}
	gtk_widget_destroy(file_selector);
}

GdkPixbuf *img_load_pixbuf_from_file(gchar *filename)
{
	GdkPixbuf *thumb = NULL;

	thumb = gdk_pixbuf_new_from_file_at_scale(filename, 93, 70, TRUE, NULL);
	return thumb;
}

slide_struct *img_set_slide_info(gint duration, gdouble speed, void	(*render), gint combo_transition_type_index, gchar *filename)
{
	slide_struct *slide_info;
	GdkPixbufFormat *pixbuf_format;
	gint width,height;

	slide_info = g_new0(slide_struct,1);
	if (slide_info)
	{
		slide_info->duration = duration;
		slide_info->speed = speed;
		slide_info->render = render;
		slide_info->combo_transition_type_index = combo_transition_type_index;
		slide_info->filename = g_strdup(filename);
		pixbuf_format = gdk_pixbuf_get_file_info(filename,&width,&height);
		slide_info->resolution = g_strdup_printf("%d x %d",width,height);
		slide_info->type = gdk_pixbuf_format_get_name(pixbuf_format);
	}
	return slide_info;
}
