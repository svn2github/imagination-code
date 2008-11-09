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
	GtkIconSize tmp_toolbar_icon_size;
	GtkWidget *hbox;
	GtkWidget *hpaned_left;
	GtkWidget *scrolledwindow1;
	GtkWidget *treeview1;
	GtkWidget *scrolledwindow;
	GtkWidget *picture_win;
	GtkWidget *image1;
	GtkWidget *hpaned_right;
	GtkWidget *vbox_info_picture;
	GtkWidget *hbox_duration;
	GtkWidget *duration_label;
	GtkObject *spinbutton1_adj;
	GtkAccelGroup *accel_group;

	img_struct = g_new0(img_window_struct,1);
	
	accel_group = gtk_accel_group_new ();

	img_struct->imagination_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (G_OBJECT (img_struct->imagination_window),"delete-event",G_CALLBACK (img_quit_application),img_struct);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (img_struct->imagination_window), vbox1);

	menubar = gtk_menu_bar_new ();
	gtk_widget_show (menubar);
	gtk_box_pack_start (GTK_BOX (vbox1), menubar, FALSE, TRUE, 0);

	menuitem1 = gtk_menu_item_new_with_mnemonic (_("_Slideshow"));
	gtk_widget_show (menuitem1);
	gtk_container_add (GTK_CONTAINER (menubar), menuitem1);

	menu1 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menu1);

	imagemenuitem1 = gtk_image_menu_item_new_from_stock ("gtk-new", accel_group);
	gtk_widget_show (imagemenuitem1);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem1);

	imagemenuitem2 = gtk_image_menu_item_new_from_stock ("gtk-open", accel_group);
	gtk_widget_show (imagemenuitem2);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem2);

	imagemenuitem3 = gtk_image_menu_item_new_from_stock ("gtk-save", accel_group);
	gtk_widget_show (imagemenuitem3);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem3);

	imagemenuitem4 = gtk_image_menu_item_new_from_stock ("gtk-save-as", accel_group);
	gtk_widget_show (imagemenuitem4);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem4);
	  
	generate_menu = gtk_image_menu_item_new_with_mnemonic (_("Generate"));
	gtk_widget_show (generate_menu);
	gtk_container_add (GTK_CONTAINER (menu1), generate_menu);
	  
	tmp_image = gtk_image_new_from_stock ("gtk-execute",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (generate_menu),tmp_image);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_widget_show (separatormenuitem1);
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	imagemenuitem5 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
	gtk_widget_show (imagemenuitem5);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem5);
	g_signal_connect ((gpointer) imagemenuitem5,"activate",G_CALLBACK (img_quit_application),NULL);

	menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Slide"));
	gtk_widget_set_sensitive (menuitem2,FALSE);
	gtk_widget_show (menuitem2);
	gtk_container_add (GTK_CONTAINER (menubar), menuitem2);

	slide_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), slide_menu);

	imagemenuitem6 = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
	gtk_widget_show (imagemenuitem6);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem6);

	imagemenuitem7 = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
	gtk_widget_show (imagemenuitem7);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem7);

	imagemenuitem8 = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
	gtk_widget_show (imagemenuitem8);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem8);

	separator_slide_menu = gtk_separator_menu_item_new ();
	gtk_widget_show (separator_slide_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu),separator_slide_menu);
	gtk_widget_set_sensitive (separator_slide_menu,FALSE);

	import_menu = gtk_image_menu_item_new_with_mnemonic (_("Import"));
	gtk_widget_show (import_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu),import_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-add",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (import_menu),tmp_image);

	remove_menu = gtk_image_menu_item_new_with_mnemonic (_("Remove"));
	gtk_widget_show (remove_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu), remove_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-remove",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (remove_menu),tmp_image);

	move_left_menu = gtk_image_menu_item_new_with_mnemonic (_("Move to left"));
	gtk_widget_show (move_left_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu), move_left_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-go-back",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (move_left_menu),tmp_image);

	move_right_menu = gtk_image_menu_item_new_with_mnemonic (_("Move to right"));
	gtk_widget_show (move_right_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu), move_right_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-go-forward",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (move_right_menu),tmp_image);

	menuitem3 = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_widget_show (menuitem3);
	gtk_container_add (GTK_CONTAINER (menubar), menuitem3);

	menu3 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem3), menu3);

	imagemenuitem11 = gtk_image_menu_item_new_from_stock ("gtk-about", accel_group);
	gtk_widget_show (imagemenuitem11);
	gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem11);
	g_signal_connect ((gpointer) imagemenuitem11,"activate",G_CALLBACK (img_show_about_dialog),img_struct);

	toolbar = gtk_toolbar_new ();
	gtk_widget_show (toolbar);
	gtk_box_pack_start (GTK_BOX (vbox1), toolbar, FALSE, TRUE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

	hbox = gtk_hbox_new (FALSE, 0);
	gtk_widget_show (hbox);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox, TRUE, TRUE, 0);

	hpaned_left = gtk_hpaned_new ();
	gtk_widget_show (hpaned_left);
	gtk_box_pack_start (GTK_BOX (hbox), hpaned_left, TRUE, TRUE, 0);
	gtk_paned_set_position (GTK_PANED (hpaned_left), 0);

	scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow1);
	gtk_paned_pack1 (GTK_PANED (hpaned_left), scrolledwindow1, FALSE, TRUE);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_SHADOW_IN);

	treeview1 = gtk_tree_view_new ();
	gtk_widget_show (treeview1);
	gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);

   mw->scroll = gtk_scrolled_window_new( NULL, NULL );
    gtk_scrolled_window_set_shadow_type( (GtkScrolledWindow*)mw->scroll, GTK_SHADOW_NONE );
    gtk_scrolled_window_set_policy((GtkScrolledWindow*)mw->scroll,
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    GtkAdjustment *hadj, *vadj;
    hadj = gtk_scrolled_window_get_hadjustment((GtkScrolledWindow*)mw->scroll);
    hadj->page_increment = 10;
    gtk_adjustment_changed(hadj);
    vadj = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow*)mw->scroll);
    vadj->page_increment = 10;
    gtk_adjustment_changed(vadj);

    image_view_set_adjustments( IMAGE_VIEW(mw->img_view), hadj, vadj );    // dirty hack :-(
    gtk_scrolled_window_add_with_viewport( (GtkScrolledWindow*)mw->scroll, mw->evt_box );
    GtkWidget* viewport = gtk_bin_get_child( (GtkBin*)mw->scroll );
    gtk_viewport_set_shadow_type( (GtkViewport*)viewport, GTK_SHADOW_NONE );
    gtk_container_set_border_width( (GtkContainer*)viewport, 20 );

    gtk_box_pack_start( (GtkBox*)box, mw->scroll, TRUE, TRUE, 0 );

	/*scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow);
	gtk_box_pack_start (GTK_BOX (hbox), scrolledwindow, TRUE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	picture_win = gtk_viewport_new (NULL, NULL);
	gtk_widget_show (picture_win);
	gtk_container_add (GTK_CONTAINER (scrolledwindow), picture_win);
	gtk_viewport_set_shadow_type (GTK_VIEWPORT (picture_win), GTK_SHADOW_NONE);*/

	image1 = create_pixmap (img_struct->imagination_window, NULL);
	gtk_widget_show (image1);
	gtk_container_add (GTK_CONTAINER (picture_win), image1);

	hpaned_right = gtk_hpaned_new ();
	gtk_widget_show (hpaned_right);
	gtk_box_pack_start (GTK_BOX (hbox), hpaned_right, TRUE, TRUE, 0);
	gtk_paned_set_position (GTK_PANED (hpaned_right), 0);
	vbox_info_picture = gtk_vbox_new (FALSE, 5);
	gtk_widget_show (vbox_info_picture);
	gtk_paned_pack2 (GTK_PANED (hpaned_right), vbox_info_picture, TRUE, TRUE);

	img_struct->transition_type = gtk_combo_box_entry_new_text ();
	gtk_widget_show (img_struct->transition_type);
	gtk_box_pack_start (GTK_BOX (vbox_info_picture), img_struct->transition_type, TRUE, TRUE, 0);

	hbox_duration = gtk_hbox_new (TRUE, 0);
	gtk_widget_show (hbox_duration);
	gtk_box_pack_start (GTK_BOX (vbox_info_picture), hbox_duration, FALSE, FALSE, 0);

	duration_label = gtk_label_new (_("Slide duration:"));
	gtk_widget_show (duration_label);
	gtk_box_pack_start (GTK_BOX (hbox_duration), duration_label, FALSE, FALSE, 0);

	spinbutton1_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
	img_struct->duration = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
	gtk_widget_show (img_struct->duration);
	gtk_box_pack_start (GTK_BOX (hbox_duration), img_struct->duration, TRUE, TRUE, 0);

	img_struct->thumbnail_iconview = gtk_icon_view_new ();
	gtk_widget_show (img_struct->thumbnail_iconview);
	gtk_box_pack_start (GTK_BOX (vbox1), img_struct->thumbnail_iconview, FALSE, FALSE, 0);
	gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_SELECTION_MULTIPLE);
	gtk_icon_view_set_orientation (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_ORIENTATION_HORIZONTAL);

	img_struct->statusbar = gtk_statusbar_new ();
	gtk_widget_show (img_struct->statusbar);
	gtk_box_pack_start (GTK_BOX (vbox1), img_struct->statusbar, FALSE, TRUE, 0);

	gtk_window_add_accel_group (GTK_WINDOW (img_struct->imagination_window), accel_group);

	return img_struct;
}

