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

GtkWidget *img_create_window (void)
{
  GtkWidget *window1;
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
  GtkWidget *transitions_combo;
  GtkWidget *hbox_duration;
  GtkWidget *duration_label;
  GtkObject *spinbutton1_adj;
  GtkWidget *spinbutton1;
  GtkWidget *thumbnail_viewer;
  GtkWidget *statusbar;
  GtkAccelGroup *accel_group;

  accel_group = gtk_accel_group_new ();

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

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

  menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Slide"));
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
	gtk_widget_set_sensitive (import_menu,FALSE);
	gtk_widget_show (import_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu),import_menu);

	tmp_image = gtk_image_new_from_stock ("gtk-add",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (import_menu),tmp_image);
	
	remove_menu = gtk_image_menu_item_new_with_mnemonic (_("Remove"));
	gtk_widget_set_sensitive (remove_menu,FALSE);
	gtk_widget_show (remove_menu);
	gtk_container_add (GTK_CONTAINER (slide_menu), remove_menu);
  
  	tmp_image = gtk_image_new_from_stock ("gtk-remove",GTK_ICON_SIZE_MENU);
	gtk_widget_show (tmp_image);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (remove_menu),tmp_image);
  
  menuitem3 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem3);
  gtk_container_add (GTK_CONTAINER (menubar), menuitem3);

  menu3 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem3), menu3);

  imagemenuitem11 = gtk_image_menu_item_new_from_stock ("gtk-about", accel_group);
  gtk_widget_show (imagemenuitem11);
  gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem11);

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

  scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow);
  gtk_box_pack_start (GTK_BOX (hbox), scrolledwindow, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  picture_win = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (picture_win);
  gtk_container_add (GTK_CONTAINER (scrolledwindow), picture_win);
  gtk_viewport_set_shadow_type (GTK_VIEWPORT (picture_win), GTK_SHADOW_NONE);

  image1 = create_pixmap (window1, NULL);
  gtk_widget_show (image1);
  gtk_container_add (GTK_CONTAINER (picture_win), image1);

  hpaned_right = gtk_hpaned_new ();
  gtk_widget_show (hpaned_right);
  gtk_box_pack_start (GTK_BOX (hbox), hpaned_right, TRUE, TRUE, 0);
  gtk_paned_set_position (GTK_PANED (hpaned_right), 0);

  vbox_info_picture = gtk_vbox_new (FALSE, 5);
  gtk_widget_show (vbox_info_picture);
  gtk_paned_pack2 (GTK_PANED (hpaned_right), vbox_info_picture, TRUE, TRUE);

  transitions_combo = gtk_combo_box_entry_new_text ();
  gtk_widget_show (transitions_combo);
  gtk_box_pack_start (GTK_BOX (vbox_info_picture), transitions_combo, TRUE, TRUE, 0);

  hbox_duration = gtk_hbox_new (TRUE, 0);
  gtk_widget_show (hbox_duration);
  gtk_box_pack_start (GTK_BOX (vbox_info_picture), hbox_duration, FALSE, FALSE, 0);

  duration_label = gtk_label_new (_("Duration:"));
  gtk_widget_show (duration_label);
  gtk_box_pack_start (GTK_BOX (hbox_duration), duration_label, FALSE, FALSE, 0);

  spinbutton1_adj = gtk_adjustment_new (1, 0, 100, 1, 10, 10);
  spinbutton1 = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
  gtk_widget_show (spinbutton1);
  gtk_box_pack_start (GTK_BOX (hbox_duration), spinbutton1, TRUE, TRUE, 0);

  thumbnail_viewer = gtk_icon_view_new ();
  gtk_widget_show (thumbnail_viewer);
  gtk_box_pack_start (GTK_BOX (vbox1), thumbnail_viewer, FALSE, FALSE, 0);
  gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (thumbnail_viewer), GTK_SELECTION_MULTIPLE);
  gtk_icon_view_set_orientation (GTK_ICON_VIEW (thumbnail_viewer), GTK_ORIENTATION_HORIZONTAL);

  statusbar = gtk_statusbar_new ();
  gtk_widget_show (statusbar);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar, FALSE, TRUE, 0);

  gtk_window_add_accel_group (GTK_WINDOW (window1), accel_group);

  return window1;
}

