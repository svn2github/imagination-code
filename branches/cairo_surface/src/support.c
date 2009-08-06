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

#include "support.h"

#define PLUGINS_INSTALLED 0

static gboolean img_plugin_is_loaded(img_window_struct *, GModule *);

GtkWidget *img_load_icon(gchar *filename, GtkIconSize size)
{
    GtkWidget *file_image;
	gchar *path;
	GdkPixbuf *file_pixbuf = NULL;

	path = g_strconcat(DATADIR, "/imagination/pixmaps/",filename,NULL);
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

gchar *img_convert_seconds_to_time(gint total_secs)
{
	gint h, m, s;

	h =  total_secs / 3600;
	m = (total_secs % 3600) / 60;
	s =  total_secs - (h * 3600) - (m * 60);
	return g_strdup_printf("%02d:%02d:%02d", h, m, s);
}

GtkWidget *_gtk_combo_box_new_text(gboolean pointer)
{
	GtkWidget *combo_box;
	GtkCellRenderer *cell;
	GtkListStore *list;
	GtkTreeStore *tree;
	GtkTreeModel *model;

	if (pointer)
	{
		tree = gtk_tree_store_new (4, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_INT);
		model = GTK_TREE_MODEL( tree );

		combo_box = gtk_combo_box_new_with_model (model);
		g_object_unref (G_OBJECT( model ));
		cell = gtk_cell_renderer_pixbuf_new ();
		gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo_box), cell, FALSE);
		gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo_box), cell, "pixbuf", 0, NULL);
	}
	else
	{
		list = gtk_list_store_new (1, G_TYPE_STRING);
		model = GTK_TREE_MODEL( list );
		
		combo_box = gtk_combo_box_new_with_model (model);
		g_object_unref (G_OBJECT( model ));
	}

	cell = gtk_cell_renderer_text_new ();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo_box), cell, TRUE);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo_box), cell, "text", pointer ? 1 : 0, NULL);
	g_object_set(cell, "ypad", (guint)0, NULL);
	return combo_box;
}

void img_set_statusbar_message(img_window_struct *img_struct, gint selected)
{
	gchar *message = NULL;
	gchar *total_slides = NULL;

	if (img_struct->slides_nr == 0)
	{
		message = g_strdup_printf(_("Welcome to Imagination - %d transitions loaded."),img_struct->nr_transitions_loaded);
		gtk_statusbar_push(GTK_STATUSBAR(img_struct->statusbar),img_struct->context_id,message);
		g_free(message);
		gtk_label_set_text(GTK_LABEL(img_struct->total_slide_number_label),NULL);
	}
	else if (selected)
	{
		message = g_strdup_printf(_("%d slides selected"),selected);
		gtk_statusbar_push(GTK_STATUSBAR(img_struct->statusbar),img_struct->context_id,message);
		g_free(message);
	}
	else
	{
		total_slides = g_strdup_printf("%d",img_struct->slides_nr);
		gtk_label_set_text(GTK_LABEL(img_struct->total_slide_number_label),total_slides);
		message = g_strdup_printf(ngettext("%d slide imported %s" ,"%d slides imported %s",img_struct->slides_nr),img_struct->slides_nr,_(" - Use the CTRL key to select/unselect or SHIFT for multiple select"));
		gtk_statusbar_push(GTK_STATUSBAR(img_struct->statusbar),img_struct->context_id,message);
		g_free(total_slides);
		g_free(message);
	}
}

void img_load_available_transitions(img_window_struct *img)
{
	GDir          *dir;
	const gchar   *transition_name;
	gchar         *fname = NULL, *name, *filename;
	gchar        **trans, **bak;
	GModule       *module;
	GdkPixbuf     *pixbuf;
	GtkTreeIter    piter, citer;
	GtkTreeStore  *model;
	gpointer       address;
	gchar         *search_paths[3], **path;
	void (*plugin_set_name)(gchar **, gchar ***);

	model = GTK_TREE_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(img->transition_type)));
	
	/* Fill the combo box with no transition */
	gtk_tree_store_append(model, &piter, NULL);
	gtk_tree_store_set(model, &piter, 0, NULL, 1, _("None"), 2, NULL, 3, -1, -1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->transition_type), 0);

	/* Create NULL terminated array of paths that we'll be looking at */
#if PLUGINS_INSTALLED
	search_paths[0] = g_strconcat(PACKAGE_LIB_DIR,"/imagination",NULL);
#else
	search_paths[0] = g_strdup("./transitions");
#endif
	search_paths[1] = g_strconcat( g_get_home_dir(), "/.imagination/plugins", NULL );
	search_paths[2] = NULL;

	/* Search all paths listed in array */
	for( path = search_paths; *path; path++ )
	{
		dir = g_dir_open( *path, 0, NULL );
		if( dir == NULL )
		{
			g_free( *path );
			continue;
		}
		
		while( TRUE )
		{
			transition_name = g_dir_read_name( dir );
			if ( transition_name == NULL )
				break;
			
			fname = g_build_filename( *path, transition_name, NULL );
			module = g_module_open( fname, G_MODULE_BIND_LOCAL );
			if( module && img_plugin_is_loaded(img, module) == FALSE )
			{
				/* Obtain the name from the plugin function */
				g_module_symbol( module, "img_get_plugin_info",
								 (void *)&plugin_set_name);
				plugin_set_name( &name, &trans );
				
				/* Add group name to the store */
				gtk_tree_store_append( model, &piter, NULL );
				gtk_tree_store_set( model, &piter, 0, NULL, 1, name, 3, 0, -1 );
				img->plugin_list = g_slist_append(img->plugin_list, module);
				
				/* Add transitions */
				for( bak = trans; *trans; trans += 3 )
				{
#if PLUGINS_INSTALLED
					filename =
						g_strdup_printf( "%s/imagination/pixmaps/imagination-%d.png",
										 DATADIR, GPOINTER_TO_INT( trans[2] ) );
#else /* PLUGINS_INSTALLED */
					filename =
						g_strdup_printf( "./pixmaps/imagination-%d.png",
										 GPOINTER_TO_INT( trans[2] ) );
#endif /* ! PLUGINS_INSTALLED */

					pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

					/* Local plugins will fail to load images from system
					 * folder, so we'll try to load the from home folder. */
					if( ! pixbuf )
					{
						g_free( filename );
						filename =
							g_strdup_printf( "%s/.imagination/pixmaps/imagination-%d.png",
											 g_get_home_dir(),
											 GPOINTER_TO_INT( trans[2] ) );
						pixbuf = gdk_pixbuf_new_from_file( filename, NULL );
					}
					g_free( filename );
					g_module_symbol( module, trans[1], &address );
					gtk_tree_store_append( model, &citer, &piter );
					gtk_tree_store_set( model, &citer, 0, pixbuf,
													   1, trans[0],
													   2, address,
													   3, GPOINTER_TO_INT( trans[2] ),
													   -1 );
					img->nr_transitions_loaded++;
				}
				g_free( bak );
			}
			g_free( fname );
		}
		g_free( *path );
		g_dir_close( dir );
	}
}

static gboolean img_plugin_is_loaded(img_window_struct *img, GModule *module)
{
	return (g_slist_find(img->plugin_list,module) != NULL);
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

slide_struct *
img_create_new_slide( const gchar *filename )
{
	slide_struct    *slide = NULL;
	GdkPixbufFormat *format;
	gint             width,
					 height;

	slide = g_slice_new( slide_struct );
	format = gdk_pixbuf_get_file_info( filename, &width, &height );
	if( slide && format )
	{
		/* Common data */
		slide->filename = g_strdup( filename );
		slide->original_filename = NULL;
		slide->resolution = g_strdup_printf( "%d x %d", width, height );
		slide->type = gdk_pixbuf_format_get_name( format );

		/* Still part */
		slide->duration = 1;

		/* Transition */
		slide->path = g_strdup( "0" );
		slide->transition_id = -1;
		slide->render = NULL;
		slide->speed = NORMAL;

		/* Ken Burns */
		slide->points = NULL;
		slide->no_points = 0;
		slide->cur_point = -1;

		/* Subtitles */
		slide->subtitle = NULL;
		slide->anim = NULL;
		slide->anim_id = 0;
		slide->anim_duration = 2; /* FIXME: Consult about this value */
		slide->position = IMG_SUB_POS_MIDDLE_CENTER;
		slide->placing = IMG_REL_PLACING_EXPORTED_VIDEO;
		slide->font_desc = pango_font_description_from_string( "Sans 12" );
		slide->font_color[0] = 0; /* R */
		slide->font_color[1] = 0; /* G */
		slide->font_color[2] = 0; /* B */
		slide->font_color[3] = 1; /* A */
	}

	return( slide );
}

void
img_set_slide_still_info( slide_struct      *slide,
						  gint               duration,
						  img_window_struct *img )
{
	if( slide->duration != duration )
	{
		slide->duration = duration;

		if( ! img->total_dur_id )
			img->total_dur_id =
				g_idle_add( (GSourceFunc)img_set_total_slideshow_duration, img );
	}
}

void
img_set_slide_transition_info( slide_struct      *slide,
							   GtkListStore      *store,
							   GtkTreeIter       *iter,
							   GdkPixbuf         *pix,
							   const gchar       *path,
							   gint               transition_id,
							   ImgRender          render,
							   guint              speed,
							   img_window_struct *img )
{
	/* Set transition render. */
	if( path && ( slide->transition_id != transition_id ) )
	{
		if( slide->path )
			g_free( slide->path );

		slide->path = g_strdup( path );
		slide->transition_id = transition_id;
		slide->render = render;

		gtk_list_store_set( store, iter, 2, pix, -1 );
	}

	if( speed && ( slide->speed != speed ) )
	{
		slide->speed = speed;

		if( ! img->total_dur_id )
			img->total_dur_id =
				g_idle_add( (GSourceFunc)img_set_total_slideshow_duration, img );
	}
}

void
img_set_slide_ken_burns_info( slide_struct *slide,
							  gint          cur_point,
							  gsize         length,
							  gdouble      *points )
{
	ImgStopPoint *point;
	gint          i,
				  full;

	if( slide->no_points )
	{
		g_list_free( slide->points );
		slide->no_points = 0;
	}

	for( i = 0; i < length; i += 4 )
	{
		/* Create new point */
		/* FIXME: This might be problematic because of the rounding. */
		point = g_slice_new0( ImgStopPoint );
		point->time = (gint)points[0 + i];
		point->offx = (gint)points[1 + i];
		point->offy = (gint)points[2 + i];
		point->zoom = 		points[3 + i];
		
		/* Append it to the list */
		slide->points = g_list_append( slide->points, point );
		slide->no_points++;
	}

	slide->cur_point = CLAMP( cur_point, -1, slide->no_points - 1 );

	full = img_calc_slide_duration_points( slide->points,
										   slide->no_points );
	if( full )
		slide->duration = full;
}

void
img_set_slide_text_info( slide_struct      *slide,
						 GtkListStore      *store,
						 GtkTreeIter       *iter,
						 const gchar       *subtitle,
						 gint	            anim_id,
						 gint               anim_duration,
						 gint               position,
						 gint               placing,
						 const gchar       *font_desc,
						 gdouble           *font_color,
						 img_window_struct *img )
{
	/* Set the slide text info parameters */
	if( store && iter )
	{
		gboolean flag;

		if( slide->subtitle )
			g_free( slide->subtitle );
		slide->subtitle = g_strdup( subtitle );

		flag = ( subtitle ? TRUE : FALSE );
		gtk_list_store_set( store, iter, 3, flag, -1 );
	}

	if( ( anim_id > -1 ) && ( anim_id != slide->anim_id ) )
	{
		GtkTreeModel *model;
		gchar        *path;
		GtkTreeIter   iter;

		path = g_strdup_printf( "%d", anim_id );
		model = gtk_combo_box_get_model( GTK_COMBO_BOX( img->sub_anim ) );
		gtk_tree_model_get_iter_from_string( model, &iter, path );
		g_free( path );

		slide->anim_id = anim_id;
		gtk_tree_model_get( model, &iter, 1, &slide->anim, -1 );
	}

	if( ( anim_duration > 0 ) && ( anim_duration != slide->anim_duration ) )
		slide->anim_duration = anim_duration;

	if( ( position > -1 ) && ( position != slide->position ) )
		slide->position = position;

	if( ( placing > -1 ) && ( placing != slide->placing ) )
		slide->placing = placing;

	if( font_desc )
	{
		if( slide->font_desc )
			pango_font_description_free( slide->font_desc );
		slide->font_desc = pango_font_description_from_string( font_desc );
	}

	if( font_color )
	{
		slide->font_color[0] = font_color[0];
		slide->font_color[1] = font_color[1];
		slide->font_color[2] = font_color[2];
		slide->font_color[3] = font_color[3];
	}
}								

void
img_free_slide_struct( slide_struct *entry )
{
	GList *tmp;

	if (entry->original_filename)
		g_free(entry->original_filename);
	g_free(entry->filename);
	g_free(entry->resolution);
	g_free(entry->type);
	
	/* Free stop point list */
	for( tmp = entry->points; tmp; tmp = g_list_next( tmp ) )
		g_slice_free( ImgStopPoint, tmp->data );
	g_list_free( entry->points );

	g_slice_free( slide_struct, entry );
}

gboolean
img_set_total_slideshow_duration( img_window_struct *img )
{
	gchar        *time;
	GtkTreeIter   iter;
	slide_struct *entry;
	GtkTreeModel *model;

	img->total_secs = 0;

	model = GTK_TREE_MODEL( img->thumbnail_model );
	if( gtk_tree_model_get_iter_first( model, &iter ) )
	{
		do
		{
			gtk_tree_model_get( model, &iter, 1, &entry, -1 );
			img->total_secs += entry->duration;
			
			if(entry->render)
				img->total_secs += entry->speed;
		}
		while( gtk_tree_model_iter_next( model, &iter ) );

		/* Add time of last pseudo slide */
		if( img->final_transition.render )
			img->total_secs += img->final_transition.speed;
	}
		
	time = img_convert_seconds_to_time(img->total_secs);
	gtk_label_set_text(GTK_LABEL (img->total_time_data),time);
	g_free(time);

	/* This is here only to be able to add this to idle source. */
	img->total_dur_id = 0;
	return( FALSE );
}

gint
img_calc_slide_duration_points( GList *list,
								gint   length )
{
	GList        *tmp;
	gint          i, duration = 0;
	ImgStopPoint *point;

	/* If we have no points, return 0 */
	if( length == 0 )
		return( 0 );

	/* Calculate length */
	for( tmp = list, i = 0; i < length; tmp = g_list_next( tmp ), i++ )
	{
		point = (ImgStopPoint *)tmp->data;
		duration += point->time;
	}

	return( duration );
}

