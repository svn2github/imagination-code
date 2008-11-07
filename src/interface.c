/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
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
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_window1 (void)
{
  GtkWidget *window1;
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *menuitem1;
  GtkWidget *menu1;
  GtkWidget *imagemenuitem1;
  GtkWidget *imagemenuitem2;
  GtkWidget *imagemenuitem3;
  GtkWidget *imagemenuitem4;
  GtkWidget *separatormenuitem1;
  GtkWidget *imagemenuitem5;
  GtkWidget *menuitem2;
  GtkWidget *menu2;
  GtkWidget *imagemenuitem6;
  GtkWidget *imagemenuitem7;
  GtkWidget *imagemenuitem8;
  GtkWidget *imagemenuitem9;
  GtkWidget *menuitem3;
  GtkWidget *menuitem4;
  GtkWidget *menu3;
  GtkWidget *imagemenuitem10;
  GtkWidget *toolbar;
  GtkIconSize tmp_toolbar_icon_size;
  GtkWidget *hbox1;
  GtkWidget *iconview1;
  GtkWidget *statusbar1;
  GtkAccelGroup *accel_group;
  
  GtkWidget *tmp_image,*New_button,*Open_button;

  accel_group = gtk_accel_group_new ();

  window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (window1), vbox1);

  menubar1 = gtk_menu_bar_new ();
  gtk_widget_show (menubar1);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar1, FALSE, TRUE, 0);

  menuitem1 = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (menuitem1);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem1);

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

  separatormenuitem1 = gtk_separator_menu_item_new ();
  gtk_widget_show (separatormenuitem1);
  gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
  gtk_widget_set_sensitive (separatormenuitem1, FALSE);

  imagemenuitem5 = gtk_image_menu_item_new_from_stock ("gtk-quit", accel_group);
  gtk_widget_show (imagemenuitem5);
  gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem5);

  menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Edit"));
  gtk_widget_show (menuitem2);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem2);

  menu2 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), menu2);

  imagemenuitem6 = gtk_image_menu_item_new_from_stock ("gtk-cut", accel_group);
  gtk_widget_show (imagemenuitem6);
  gtk_container_add (GTK_CONTAINER (menu2), imagemenuitem6);

  imagemenuitem7 = gtk_image_menu_item_new_from_stock ("gtk-copy", accel_group);
  gtk_widget_show (imagemenuitem7);
  gtk_container_add (GTK_CONTAINER (menu2), imagemenuitem7);

  imagemenuitem8 = gtk_image_menu_item_new_from_stock ("gtk-paste", accel_group);
  gtk_widget_show (imagemenuitem8);
  gtk_container_add (GTK_CONTAINER (menu2), imagemenuitem8);

  imagemenuitem9 = gtk_image_menu_item_new_from_stock ("gtk-delete", accel_group);
  gtk_widget_show (imagemenuitem9);
  gtk_container_add (GTK_CONTAINER (menu2), imagemenuitem9);

  menuitem3 = gtk_menu_item_new_with_mnemonic (_("_View"));
  gtk_widget_show (menuitem3);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem3);

  menuitem4 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (menuitem4);
  gtk_container_add (GTK_CONTAINER (menubar1), menuitem4);

  menu3 = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem4), menu3);

  imagemenuitem10 = gtk_image_menu_item_new_from_stock ("gtk-about", accel_group);
  gtk_widget_show (imagemenuitem10);
  gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem10);

/* Create the toolbar */
  toolbar = gtk_toolbar_new ();
  gtk_widget_show (toolbar);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar, FALSE, FALSE, 0);
  gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
  tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

  	tmp_image = gtk_image_new_from_stock ("gtk-new",tmp_toolbar_icon_size);
	gtk_widget_show (tmp_image);
	New_button = (GtkWidget*) gtk_tool_button_new (tmp_image,_("New"));
	gtk_widget_show (New_button);
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (New_button),FALSE);
	gtk_container_add (GTK_CONTAINER (toolbar),New_button);
	//gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (New_button),tooltips,_("Create a new archive"),NULL);

	tmp_image = gtk_image_new_from_stock ("gtk-open",tmp_toolbar_icon_size);
	gtk_widget_show (tmp_image);
	Open_button = (GtkWidget*) gtk_tool_button_new (tmp_image,_("Open"));
	gtk_widget_show (Open_button);
	gtk_tool_item_set_homogeneous (GTK_TOOL_ITEM (Open_button),FALSE);
	gtk_container_add (GTK_CONTAINER (toolbar),Open_button);
	//gtk_tool_item_set_tooltip (GTK_TOOL_ITEM (Open_button),tooltips,_("Open an archive"),NULL);
	
  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

  iconview1 = gtk_icon_view_new ();
  gtk_widget_show (iconview1);
  gtk_box_pack_start (GTK_BOX (vbox1), iconview1, FALSE, FALSE, 0);
  gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (iconview1), GTK_SELECTION_MULTIPLE);
  gtk_icon_view_set_orientation (GTK_ICON_VIEW (iconview1), GTK_ORIENTATION_HORIZONTAL);

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_show (statusbar1);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar1, FALSE, TRUE, 0);

  gtk_window_add_accel_group (GTK_WINDOW (window1), accel_group);

  return window1;
}

