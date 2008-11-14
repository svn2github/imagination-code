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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "main-window.h"
#include "support.h"

img_window_struct *img_create_window (void)
{
	img_window_struct *img_struct = NULL;
	GtkWidget *vbox1;
	GtkWidget *menubar;
	GtkWidget *menuitem1;
	GtkWidget *menu1;
	GtkWidget *imagemenuitem1;
	GtkWidget *imagemenuitem2;
	GtkWidget *imagemenuitem3;
	GtkWidget *imagemenuitem4;
	GtkWidget *separatormenuitem1;
	GtkWidget *imagemenuitem5;
	GtkWidget *menuitem2;
	GtkWidget *slide_menu;
	GtkWidget *separator_slide_menu;
	GtkWidget *imagemenuitem6;
	GtkWidget *generate_menu;
	GtkWidget *imagemenuitem7;
	GtkWidget *imagemenuitem8;
	GtkWidget *import_menu;
	GtkWidget *remove_menu;
	GtkWidget *move_left_menu;
	GtkWidget *move_right_menu;
	GtkWidget *menuitem3;
	GtkWidget *menuitem4;
	GtkWidget *tmp_image;
	GtkWidget *menu3;
	GtkWidget *imagemenuitem10;
	GtkWidget *imagemenuitem11;
	GtkWidget *toolbar;
	GtkWidget *viewport;
	GtkIconSize tmp_toolbar_icon_size;
	GtkWidget *hbox;
	GtkWidget *thumb_scrolledwindow;
	GtkWidget *scrolledwindow;
	GtkWidget *picture_win;
	GtkWidget *image1;
	GtkWidget *transition_label;
	GtkWidget *vbox_info_slide;
	GtkWidget *hbox_duration;
	GtkWidget *duration_label;
	GtkObject *spinbutton1_adj;
	GtkAccelGroup *accel_group;
	
	GtkWidget *new_button;
	GdkColor background_color = {0, 65535, 65535, 65535};

	img_struct = g_new0(img_window_struct,1);
	
	accel_group = gtk_accel_group_new ();

	img_struct->imagination_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size( (GtkWindow*)img_struct->imagination_window, 840, 580 );
	g_signal_connect (G_OBJECT (img_struct->imagination_window),"delete-event",G_CALLBACK (img_quit_application),img_struct);

	vbox1 = gtk_vbox_new (FALSE,2);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (img_struct->imagination_window), vbox1);

	/* Create the menu items */
	menubar = gtk_menu_bar_new ();
	gtk_box_pack_start ((GtkBox *)vbox1, menubar, FALSE, TRUE, 0);

	menuitem1 = gtk_menu_item_new_with_mnemonic (_("_Slideshow"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem1);

	menu1 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menu1);

	imagemenuitem1 = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem1);

	imagemenuitem2 = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem2);

	imagemenuitem3 = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem3);

	imagemenuitem4 = gtk_image_menu_item_new_from_stock ("gtk-save-as", accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem4);
	  
	generate_menu = gtk_image_menu_item_new_with_mnemonic (_("Generate"));
	gtk_widget_add_accelerator (generate_menu,"activate",accel_group,GDK_g,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (menu1), generate_menu);
	  
	tmp_image = gtk_image_new_from_stock ("gtk-execute",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (generate_menu),tmp_image);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	imagemenuitem5 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem5);
	g_signal_connect ((gpointer) imagemenuitem5,"activate",G_CALLBACK (img_quit_application),img_struct);

	menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Slide"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem2);

	slide_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), slide_menu);

	imagemenuitem6 = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem6);

	imagemenuitem7 = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem7);

	imagemenuitem8 = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem8);

	separator_slide_menu = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (slide_menu),separator_slide_menu);
	gtk_widget_set_sensitive (separator_slide_menu,FALSE);

	import_menu = gtk_image_menu_item_new_with_mnemonic (_("_Import"));
	gtk_container_add (GTK_CONTAINER (slide_menu),import_menu);
	gtk_widget_add_accelerator (import_menu,"activate",accel_group,GDK_i,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect ((gpointer) import_menu,"activate",G_CALLBACK (img_add_slides_thumbnails),img_struct);

	tmp_image = gtk_image_new_from_stock ("gtk-add",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (import_menu),tmp_image);

	remove_menu = gtk_image_menu_item_new_with_mnemonic (_("_Delete"));
	gtk_widget_add_accelerator (remove_menu,"activate",accel_group,GDK_d,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (slide_menu), remove_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-remove",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (remove_menu),tmp_image);

	move_left_menu = gtk_image_menu_item_new_with_mnemonic (_("Move to _left"));
	gtk_widget_add_accelerator (move_left_menu,"activate",accel_group,GDK_l,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (slide_menu), move_left_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-go-back",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (move_left_menu),tmp_image);

	move_right_menu = gtk_image_menu_item_new_with_mnemonic (_("Move to _right"));
	gtk_widget_add_accelerator (move_right_menu,"activate",accel_group,GDK_r,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (slide_menu), move_right_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-go-forward",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (move_right_menu),tmp_image);

	menuitem3 = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem3);

	menu3 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem3), menu3);

	imagemenuitem11 = gtk_image_menu_item_new_from_stock ("gtk-about", accel_group);
	gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem11);
	gtk_widget_show_all (menubar);
	g_signal_connect ((gpointer) imagemenuitem11,"activate",G_CALLBACK (img_show_about_dialog),img_struct);

	/* Create the toolbar */
	toolbar = gtk_toolbar_new ();
	gtk_box_pack_start ((GtkBox *)vbox1, toolbar, FALSE, FALSE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

	tmp_image = gtk_image_new_from_stock ("gtk-new",tmp_toolbar_icon_size);
	new_button = (GtkWidget*) gtk_tool_button_new (tmp_image,_("New"));
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (new_button),FALSE);
	gtk_container_add (GTK_CONTAINER (toolbar),new_button);
	gtk_widget_set_tooltip_text(new_button, "Create a new slideshow"); 
	gtk_widget_show_all (toolbar);

	/* Create the image area and the other widgets */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start ((GtkBox*)vbox1, hbox, TRUE, TRUE, 0);

	/* Code from gpicview with some modifications by me */
	img_struct->event_box = gtk_event_box_new();
	GTK_WIDGET_SET_FLAGS(img_struct->event_box,GTK_CAN_FOCUS);
	gtk_widget_add_events(img_struct->event_box,GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK);
	scrolledwindow = gtk_scrolled_window_new(NULL,NULL);
	gtk_scrolled_window_set_shadow_type( (GtkScrolledWindow*)scrolledwindow, GTK_SHADOW_NONE );
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)scrolledwindow,GTK_POLICY_NEVER, GTK_POLICY_NEVER);
    gtk_widget_modify_bg(img_struct->event_box,GTK_STATE_NORMAL,&background_color);
    
	gtk_scrolled_window_add_with_viewport( (GtkScrolledWindow*)scrolledwindow,img_struct->event_box);
	viewport = gtk_bin_get_child( (GtkBin*)scrolledwindow);
	gtk_viewport_set_shadow_type( (GtkViewport*)viewport, GTK_SHADOW_IN);
	gtk_container_set_border_width( (GtkContainer*)viewport,10);
	gtk_box_pack_start( (GtkBox*)hbox,scrolledwindow,TRUE,TRUE,0);
	/* End code from gpicview */

	vbox_info_slide = gtk_vbox_new (FALSE,1);
	gtk_box_pack_start ((GtkBox *)hbox,vbox_info_slide,FALSE,FALSE,0);

	/* Create the combo box and the spinbutton */
	transition_label = gtk_label_new(_("Transition type:"));
	gtk_misc_set_alignment((GtkMisc *) transition_label,0,-1);
	gtk_box_pack_start ((GtkBox *)vbox_info_slide,transition_label,FALSE,FALSE,0);
	img_struct->transition_type = gtk_combo_box_entry_new_text ();
	gtk_box_pack_start ((GtkBox *)vbox_info_slide, img_struct->transition_type,FALSE,TRUE,0);

	hbox_duration = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start ((GtkBox *)vbox_info_slide, hbox_duration, FALSE, FALSE, 0);

	duration_label = gtk_label_new (_("Slide duration in sec:"));
	gtk_box_pack_start ((GtkBox *)hbox_duration,duration_label, FALSE, FALSE, 0);

	spinbutton1_adj = gtk_adjustment_new (1, 1, 120, 1, 10, 10);
	img_struct->duration = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
	gtk_box_pack_start ((GtkBox *)hbox_duration, img_struct->duration, FALSE, FALSE, 0);

	/* Create the model */
	img_struct->thumbnail_model = gtk_list_store_new (2,GDK_TYPE_PIXBUF,G_TYPE_POINTER);

	/* Create the thumbnail viewer */
	thumb_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show_all(thumb_scrolledwindow);
	gtk_box_pack_start ((GtkBox *)vbox1, thumb_scrolledwindow, FALSE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (thumb_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (thumb_scrolledwindow), GTK_SHADOW_IN);

	img_struct->thumbnail_iconview = gtk_icon_view_new_with_model((GtkTreeModel *)img_struct->thumbnail_model);
	gtk_widget_show (img_struct->thumbnail_iconview);
	gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_SELECTION_MULTIPLE);
	gtk_icon_view_set_orientation (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_ORIENTATION_HORIZONTAL);
	gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW (img_struct->thumbnail_iconview), 0);
	gtk_container_add (GTK_CONTAINER (thumb_scrolledwindow), img_struct->thumbnail_iconview);

	/* Create the status bar */
	img_struct->statusbar = gtk_statusbar_new ();
	gtk_widget_show (img_struct->statusbar);
	gtk_box_pack_start ((GtkBox *)vbox1, img_struct->statusbar, FALSE, TRUE, 0);
	img_struct->message_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (img_struct->statusbar), "statusbar");
	
	gtk_widget_show_all(hbox);
	gtk_window_add_accel_group (GTK_WINDOW (img_struct->imagination_window), accel_group);

	return img_struct;
}

