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

#include "main-window.h"
#include "callbacks.h"

static void img_iconview_selection_changed (GtkIconView *, img_window_struct *);
static void img_combo_box_transition_type_changed (GtkComboBox *, img_window_struct *);
static void img_combo_box_speed_changed (GtkComboBox *,  img_window_struct *);
static void img_spinbutton_value_changed (GtkSpinButton *, img_window_struct *);
static void img_quit_menu(GtkMenuItem *, img_window_struct *);
static void img_select_all_thumbnails(GtkMenuItem *, img_window_struct *);
static void img_unselect_all_thumbnails(GtkMenuItem *, img_window_struct *);
static void img_goto_slide(GtkMenuItem *, img_window_struct *);
static void img_goto_line_entry_activate(GtkEntry *, img_window_struct *);

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
	GtkWidget *imagemenuitem5;
	GtkWidget *close_menu;
	GtkWidget *separatormenuitem1;
	GtkWidget *menuitem2;
	GtkWidget *slide_menu;
	GtkWidget *separator_slide_menu;
	GtkWidget *imagemenuitem6;
	GtkWidget *generate_menu;
	GtkWidget *imagemenuitem7;
	GtkWidget *imagemenuitem8;
	GtkWidget *import_menu;
	GtkWidget *image_menu;
	GtkWidget *select_all_menu;
	GtkWidget *deselect_all_menu;
	GtkWidget *goto_menu;
	GtkWidget *menuitem3;
	GtkWidget *tmp_image;
	GtkWidget *menu3;
	GtkWidget *imagemenuitem11;
	GtkWidget *toolbar;
	GtkWidget *new_button;
	GtkWidget *open_button;
	GtkWidget *save_button;
	GtkWidget *generate_button;
	GtkWidget *import_button;
	GtkWidget *separatortoolitem;
	GtkWidget *goto_button;
	GtkWidget *hbox;
	GtkWidget *fixed;
	GtkWidget *valign;
	GtkWidget *halign;
	GtkWidget *frame1_alignment;
	GtkWidget *thumb_scrolledwindow;
	GtkWidget *transition_label;
	GtkWidget *vbox_info_slide;
	GtkWidget *hbox_effect_duration;
	GtkWidget *hbox_duration;
	GtkWidget *duration_label;
	GtkWidget *trans_duration_label;
	GtkObject *spinbutton1_adj;
	GtkWidget *frame1;
	GtkWidget *frame_label;
	GtkWidget *hbox_slide_selected;
	GtkWidget *selected_slide;
	GtkWidget *hbox_resolution;
	GtkWidget *resolution;
	GtkWidget *hbox_type;
	GtkWidget *hbox_total;
	GtkWidget *total_time;
	GtkWidget *type;
	GtkAccelGroup *accel_group;
	GdkColor background_color = {0, 65535, 65535, 65535};
	GtkCellRenderer *pixbuf_cell;
	GtkIconSize tmp_toolbar_icon_size;

	accel_group = gtk_accel_group_new ();

	img_struct = g_new0(img_window_struct,1);
	img_struct->imagination_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (img_struct->imagination_window), 840, 580 );
	img_set_window_title(img_struct,NULL);
	g_signal_connect (G_OBJECT (img_struct->imagination_window),"delete-event",G_CALLBACK (img_quit_application),img_struct);

	vbox1 = gtk_vbox_new (FALSE,2);
	gtk_widget_show (vbox1);
	gtk_container_add (GTK_CONTAINER (img_struct->imagination_window), vbox1);

	/* Create the menu items */
	menubar = gtk_menu_bar_new ();
	gtk_box_pack_start (GTK_BOX (vbox1), menubar, FALSE, TRUE, 0);

	menuitem1 = gtk_menu_item_new_with_mnemonic (_("_Slideshow"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem1);

	menu1 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem1), menu1);

	imagemenuitem1 = gtk_image_menu_item_new_from_stock (GTK_STOCK_NEW, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem1);
	g_signal_connect (G_OBJECT (imagemenuitem1),"activate",G_CALLBACK (img_new_slideshow),img_struct);

	imagemenuitem2 = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem2);

	imagemenuitem3 = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem3);

	imagemenuitem4 = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE_AS, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem4);

	close_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_CLOSE, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), close_menu);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	img_struct->preview_menu = gtk_image_menu_item_new_with_mnemonic (_("_Preview"));
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->preview_menu);
	gtk_widget_add_accelerator (img_struct->preview_menu,"activate",accel_group,GDK_p,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (img_struct->preview_menu),"activate",G_CALLBACK (img_start_stop_preview),img_struct);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->preview_menu),tmp_image);

	generate_menu = gtk_image_menu_item_new_with_mnemonic (_("Generate"));
	gtk_widget_add_accelerator (generate_menu,"activate",accel_group,GDK_g,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (menu1), generate_menu);
	  
	image_menu = img_load_icon ("imagination-generate.png",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (generate_menu),image_menu);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (GTK_WIDGET (separatormenuitem1), FALSE);

	imagemenuitem5 = gtk_image_menu_item_new_from_stock (GTK_STOCK_QUIT, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), imagemenuitem5);
	g_signal_connect (G_OBJECT (imagemenuitem5),"activate",G_CALLBACK (img_quit_menu),img_struct);

	menuitem2 = gtk_menu_item_new_with_mnemonic (_("_Slide"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem2);

	slide_menu = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem2), slide_menu);

	imagemenuitem6 = gtk_image_menu_item_new_from_stock (GTK_STOCK_CUT, accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem6);

	imagemenuitem7 = gtk_image_menu_item_new_from_stock (GTK_STOCK_COPY, accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem7);

	imagemenuitem8 = gtk_image_menu_item_new_from_stock (GTK_STOCK_PASTE, accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu), imagemenuitem8);

	separator_slide_menu = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (slide_menu),separator_slide_menu);

	select_all_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_SELECT_ALL, accel_group);
	gtk_container_add (GTK_CONTAINER (slide_menu),select_all_menu);
	gtk_widget_add_accelerator (select_all_menu,"activate",accel_group,GDK_a,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (select_all_menu),"activate",G_CALLBACK (img_select_all_thumbnails),img_struct);

	deselect_all_menu = gtk_image_menu_item_new_with_mnemonic (_("Un_select all"));
	gtk_container_add (GTK_CONTAINER (slide_menu),deselect_all_menu);
	gtk_widget_add_accelerator (deselect_all_menu,"activate",accel_group,GDK_e,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (deselect_all_menu),"activate",G_CALLBACK (img_unselect_all_thumbnails),img_struct);

	separator_slide_menu = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (slide_menu),separator_slide_menu);

	import_menu = gtk_image_menu_item_new_with_mnemonic (_("_Import"));
	gtk_container_add (GTK_CONTAINER (slide_menu),import_menu);
	gtk_widget_add_accelerator (import_menu,"activate",accel_group,GDK_i,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (import_menu),"activate",G_CALLBACK (img_add_slides_thumbnails),img_struct);

	image_menu = img_load_icon ("imagination-import.png",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (import_menu),image_menu);

	img_struct->remove_menu = gtk_image_menu_item_new_with_mnemonic (_("_Delete"));
	gtk_widget_set_sensitive(img_struct->remove_menu,FALSE);
	gtk_container_add (GTK_CONTAINER (slide_menu), img_struct->remove_menu);
	gtk_widget_add_accelerator (img_struct->remove_menu,"activate",accel_group, GDK_Delete,GDK_MODE_DISABLED,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (img_struct->remove_menu),"activate",G_CALLBACK (img_delete_selected_slides),img_struct);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_DELETE,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->remove_menu),tmp_image);

	separator_slide_menu = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (slide_menu),separator_slide_menu);

	goto_menu = gtk_image_menu_item_new_with_mnemonic (_("Go to s_lide"));
	gtk_widget_add_accelerator (goto_menu,"activate",accel_group,GDK_l,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (slide_menu), goto_menu);
	g_signal_connect (G_OBJECT (goto_menu),"activate",G_CALLBACK (img_goto_slide),img_struct);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_JUMP_TO,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (goto_menu),tmp_image);

	menuitem3 = gtk_menu_item_new_with_mnemonic (_("_Help"));
	gtk_container_add (GTK_CONTAINER (menubar), menuitem3);

	menu3 = gtk_menu_new ();
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem3), menu3);

	imagemenuitem11 = gtk_image_menu_item_new_from_stock (GTK_STOCK_ABOUT, accel_group);
	gtk_container_add (GTK_CONTAINER (menu3), imagemenuitem11);
	gtk_widget_show_all (menubar);
	g_signal_connect (G_OBJECT (imagemenuitem11),"activate",G_CALLBACK (img_show_about_dialog),img_struct);

	/* Create the toolbar */
	toolbar = gtk_toolbar_new ();
	gtk_box_pack_start (GTK_BOX (vbox1), toolbar, FALSE, FALSE, 0);
	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_ICONS);
	tmp_toolbar_icon_size = gtk_toolbar_get_icon_size (GTK_TOOLBAR (toolbar));

	new_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_NEW));
	gtk_container_add (GTK_CONTAINER (toolbar),new_button);
	gtk_widget_set_tooltip_text(new_button, _("Create a new slideshow"));
	g_signal_connect (G_OBJECT (new_button),"clicked",G_CALLBACK (img_new_slideshow),img_struct);

	open_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_OPEN));
	gtk_container_add (GTK_CONTAINER (toolbar),open_button);
	gtk_widget_set_tooltip_text(open_button, _("Open a slideshow"));

	save_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_SAVE));
	gtk_container_add (GTK_CONTAINER (toolbar),save_button);
	gtk_widget_set_tooltip_text(save_button, _("Save the slideshow"));

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	img_struct->preview_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY));
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->preview_button);
	gtk_widget_set_tooltip_text(img_struct->preview_button, _("Preview the slideshow"));
	g_signal_connect (G_OBJECT (img_struct->preview_button),"clicked",G_CALLBACK (img_start_stop_preview),img_struct);

	tmp_image = img_load_icon("imagination-generate.png",GTK_ICON_SIZE_LARGE_TOOLBAR);
	generate_button = GTK_WIDGET (gtk_tool_button_new (tmp_image,""));
	gtk_container_add (GTK_CONTAINER (toolbar),generate_button);
	gtk_widget_set_tooltip_text(generate_button, _("Generate the DVD slideshow"));

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	tmp_image = img_load_icon("imagination-import.png",GTK_ICON_SIZE_LARGE_TOOLBAR);
	import_button = GTK_WIDGET (gtk_tool_button_new (tmp_image,""));
	gtk_container_add (GTK_CONTAINER (toolbar),import_button);
	gtk_widget_set_tooltip_text(import_button, _("Import the slides"));
	g_signal_connect ((gpointer) import_button,"clicked",G_CALLBACK (img_add_slides_thumbnails),img_struct);

	img_struct->remove_button = GTK_WIDGET (gtk_tool_button_new_from_stock ("gtk-delete"));
	gtk_widget_set_sensitive(img_struct->remove_button,FALSE);
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->remove_button);
	gtk_widget_set_tooltip_text(img_struct->remove_button, _("Delete the selected slides"));
	g_signal_connect (G_OBJECT (img_struct->remove_button),"clicked",G_CALLBACK (img_delete_selected_slides),img_struct);

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	goto_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_JUMP_TO));
	gtk_container_add (GTK_CONTAINER (toolbar),goto_button);
	gtk_widget_set_tooltip_text(goto_button, _("Jump to the entered slide number"));
	g_signal_connect (G_OBJECT (goto_button),"clicked",G_CALLBACK (img_goto_slide),img_struct);

	gtk_widget_show_all (toolbar);

	/* Create the image area and the other widgets */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox, TRUE, TRUE, 0);

	fixed = gtk_alignment_new (0.5, 0.5, 0, 0);
	gtk_box_pack_start (GTK_BOX (hbox), fixed, TRUE, TRUE, 0);

	img_struct->viewport = gtk_viewport_new(NULL,NULL);
	gtk_container_add( GTK_CONTAINER(fixed), img_struct->viewport ); 
	gtk_widget_set_size_request (img_struct->viewport, 720, 576);
	gtk_widget_modify_bg(img_struct->viewport,GTK_STATE_NORMAL,&background_color);
	gtk_viewport_set_shadow_type(GTK_VIEWPORT (img_struct->viewport), GTK_SHADOW_IN);
	gtk_container_set_border_width(GTK_CONTAINER (img_struct->viewport),10);

	img_struct->image_area = gtk_image_new();
	gtk_container_add (GTK_CONTAINER (img_struct->viewport), img_struct->image_area);

	valign = gtk_alignment_new (1, 0, 0, 0);
	gtk_box_pack_start (GTK_BOX (hbox), valign, FALSE, FALSE, 0);

	halign = gtk_alignment_new (0, 0, 0, 0);
	gtk_container_add (GTK_CONTAINER (valign), halign);
	gtk_alignment_set_padding (GTK_ALIGNMENT (halign), 10, 10, 10, 10);

	frame1 = gtk_frame_new (NULL);
	gtk_container_add (GTK_CONTAINER (halign), frame1);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_OUT);

	frame1_alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame1), frame1_alignment);
	gtk_alignment_set_padding (GTK_ALIGNMENT (frame1_alignment), 2, 2, 5, 5);

	frame_label = gtk_label_new (_("<b>Slide settings</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame1), frame_label);
	gtk_label_set_use_markup (GTK_LABEL (frame_label), TRUE);
	gtk_misc_set_padding (GTK_MISC (frame_label), 2, 2);

	vbox_info_slide = gtk_vbox_new (FALSE, 3);
	gtk_container_add (GTK_CONTAINER (frame1_alignment), vbox_info_slide);
	gtk_container_set_border_width (GTK_CONTAINER (vbox_info_slide), 2);

	/* Transition types */
	transition_label = gtk_label_new (_("Transition Type:"));
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), transition_label, FALSE, FALSE, 0);
	gtk_misc_set_alignment (GTK_MISC (transition_label), 0, -1);

	img_struct->transition_type = _gtk_combo_box_new_text(TRUE);
	gtk_widget_set_sensitive(img_struct->transition_type,FALSE);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), img_struct->transition_type, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (img_struct->transition_type),"changed",G_CALLBACK (img_combo_box_transition_type_changed),img_struct);

	/* Transition duration */
	hbox_effect_duration = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_effect_duration, FALSE, TRUE, 0);
	trans_duration_label = gtk_label_new (_("Transition Speed:"));
	gtk_box_pack_start (GTK_BOX (hbox_effect_duration), trans_duration_label, FALSE, FALSE, 0);

	img_struct->trans_duration = _gtk_combo_box_new_text(FALSE);
	gtk_combo_box_append_text(GTK_COMBO_BOX(img_struct->trans_duration),_("Fast"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(img_struct->trans_duration),_("Normal"));
	gtk_combo_box_append_text(GTK_COMBO_BOX(img_struct->trans_duration),_("Slow"));
	gtk_combo_box_set_active(GTK_COMBO_BOX(img_struct->trans_duration),1);
	gtk_widget_set_sensitive(img_struct->trans_duration,FALSE);
	gtk_box_pack_end (GTK_BOX (hbox_effect_duration), img_struct->trans_duration, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (img_struct->trans_duration),"changed",G_CALLBACK (img_combo_box_speed_changed),img_struct);

	/* Slide duration */
	hbox_duration = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_duration, FALSE, TRUE, 0);
	duration_label = gtk_label_new (_("Slide Duration in sec:"));
	gtk_box_pack_start (GTK_BOX (hbox_duration), duration_label, FALSE, FALSE, 0);
	spinbutton1_adj = gtk_adjustment_new (1, 1, 300, 1, 10, 0);
	img_struct->duration = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton1_adj), 1, 0);
	gtk_widget_set_sensitive(img_struct->duration,FALSE);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON (img_struct->duration),TRUE);
	gtk_box_pack_end (GTK_BOX (hbox_duration), img_struct->duration, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (img_struct->duration),"value-changed",G_CALLBACK (img_spinbutton_value_changed),img_struct);

	/* Slide Selected */
	hbox_slide_selected = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_slide_selected, FALSE, TRUE, 0);

	selected_slide = gtk_label_new (_("Slide number:"));
	gtk_box_pack_start (GTK_BOX (hbox_slide_selected), selected_slide, FALSE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (selected_slide), 0, 0.5);

	img_struct->slide_selected_data = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox_slide_selected), img_struct->slide_selected_data, TRUE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (img_struct->slide_selected_data), 0, 0.5);

	/* Slide Resolution */
	hbox_resolution = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_resolution, FALSE, TRUE, 0);

	resolution = gtk_label_new (_("Resolution:"));
	gtk_box_pack_start (GTK_BOX (hbox_resolution), resolution, FALSE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (resolution), 0, 0.5);

	img_struct->resolution_data = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox_resolution), img_struct->resolution_data, TRUE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (img_struct->resolution_data), 0, 0.5);

	/* Slide Type */
	hbox_type = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_type, FALSE, TRUE, 0);

	type = gtk_label_new (_("Type:"));
	gtk_box_pack_start (GTK_BOX (hbox_type), type, FALSE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (type), 0, 0.5);

	img_struct->type_data = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (hbox_type), img_struct->type_data, TRUE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (img_struct->type_data), 0, 0.5);

	/* Slide Total Duration */
	hbox_total = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_total, FALSE, TRUE, 0);

	total_time = gtk_label_new (_("Slideshow Length:"));
	gtk_box_pack_start (GTK_BOX (hbox_total), total_time, FALSE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC( total_time ), 0, 0.5);

	img_struct->total_time_data = gtk_label_new ("00:00:00");
	gtk_box_pack_start (GTK_BOX (hbox_total), img_struct->total_time_data, TRUE, TRUE, 0);
	gtk_misc_set_alignment (GTK_MISC (img_struct->total_time_data), 0, 0.5);
	
	/* Create the model */
	img_struct->thumbnail_model = gtk_list_store_new (2, GDK_TYPE_PIXBUF, G_TYPE_POINTER);

	/* Create the thumbnail viewer */
	thumb_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show(thumb_scrolledwindow);
	gtk_box_pack_start (GTK_BOX (vbox1), thumb_scrolledwindow, FALSE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (thumb_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (thumb_scrolledwindow), GTK_SHADOW_IN);
	img_struct->thumbnail_iconview = gtk_icon_view_new_with_model(GTK_TREE_MODEL (img_struct->thumbnail_model));
	gtk_widget_show (img_struct->thumbnail_iconview);

	/* Create the cell layout */
	pixbuf_cell = eog_pixbuf_cell_renderer_new();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (img_struct->thumbnail_iconview), pixbuf_cell, FALSE);
	g_object_set (G_OBJECT (pixbuf_cell), "width", 115, "ypad", 2, NULL);
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (img_struct->thumbnail_iconview), pixbuf_cell, "pixbuf", 0, NULL);

	/* Set some iconview properties */
	//gtk_icon_view_enable_model_drag_source ((GtkIconView*)img_struct->thumbnail_iconview, 0, target_table, G_N_ELEMENTS (target_table), GDK_ACTION_MOVE);
	gtk_icon_view_set_reorderable(GTK_ICON_VIEW (img_struct->thumbnail_iconview),TRUE);
	gtk_icon_view_set_selection_mode (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_SELECTION_MULTIPLE);
	gtk_icon_view_set_orientation (GTK_ICON_VIEW (img_struct->thumbnail_iconview), GTK_ORIENTATION_HORIZONTAL);
	gtk_icon_view_set_column_spacing (GTK_ICON_VIEW (img_struct->thumbnail_iconview),0);
	gtk_icon_view_set_row_spacing (GTK_ICON_VIEW (img_struct->thumbnail_iconview),0);
	gtk_icon_view_set_columns (GTK_ICON_VIEW (img_struct->thumbnail_iconview), G_MAXINT);
	gtk_container_add (GTK_CONTAINER (thumb_scrolledwindow), img_struct->thumbnail_iconview);
	g_signal_connect (G_OBJECT (img_struct->thumbnail_iconview),"selection-changed",G_CALLBACK (img_iconview_selection_changed),img_struct);

	/* Create the status bar */
	img_struct->statusbar = gtk_statusbar_new ();
	gtk_widget_show (img_struct->statusbar);
	gtk_box_pack_start (GTK_BOX (vbox1), img_struct->statusbar, FALSE, TRUE, 0);
	img_struct->context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (img_struct->statusbar), "statusbar");

	gtk_widget_show_all(hbox);
	gtk_window_add_accel_group (GTK_WINDOW (img_struct->imagination_window), accel_group);

	return img_struct;
}

static void img_quit_menu(GtkMenuItem *menuitem, img_window_struct *img)
{
	gboolean value;

	g_signal_emit_by_name(img->imagination_window,"delete-event", img, &value);
}

static void img_iconview_selection_changed(GtkIconView *iconview, img_window_struct *img)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkTreePath *path = NULL;
	gint dummy, nr_selected = 0;
	gchar *selected_slide = NULL;
	GList *selected = NULL;
	slide_struct *info_slide;

	model = gtk_icon_view_get_model(iconview);
	gtk_icon_view_get_cursor(iconview,&path,NULL);

	selected = gtk_icon_view_get_selected_items(iconview);
	nr_selected = g_list_length(selected);
	if (path == NULL || gtk_icon_view_path_is_selected(iconview,path) == FALSE)
	{
		if (selected == NULL)
		{
			img_set_statusbar_message(img,nr_selected);
			gtk_image_set_from_pixbuf(GTK_IMAGE(img->image_area),NULL);
			gtk_widget_set_sensitive(img->remove_menu,		FALSE);
			gtk_widget_set_sensitive(img->remove_button,	FALSE);
			gtk_widget_set_sensitive(img->trans_duration,	FALSE);
			gtk_widget_set_sensitive(img->duration,			FALSE);
			gtk_widget_set_sensitive(img->transition_type,	FALSE);
			gtk_label_set_text(GTK_LABEL (img->type_data),"");
			gtk_label_set_text(GTK_LABEL (img->resolution_data),"");
			gtk_label_set_text(GTK_LABEL (img->slide_selected_data),"");
			return;
		}
		if (nr_selected >= 1)
			img_set_statusbar_message(img,nr_selected);
		else
			gtk_statusbar_pop(GTK_STATUSBAR(img->statusbar),img->context_id);
		g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
		g_list_free (selected);
		return;
	}
	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (selected);
	gtk_widget_set_sensitive(img->remove_menu,		TRUE);
	gtk_widget_set_sensitive(img->remove_button,	TRUE);
	gtk_widget_set_sensitive(img->trans_duration,	TRUE);
	gtk_widget_set_sensitive(img->duration,			TRUE);
	gtk_widget_set_sensitive(img->transition_type,	TRUE);

	dummy = gtk_tree_path_get_indices(path)[0]+1;
	selected_slide = g_strdup_printf("%d",dummy);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_path_free(path);
	gtk_tree_model_get(model,&iter,1,&info_slide,-1);

	/* Set the transition type */
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(img->transition_type));

	if (gtk_combo_box_get_active(GTK_COMBO_BOX(img->transition_type)) == 0)
		gtk_widget_set_sensitive(img->trans_duration,FALSE);
	else
		gtk_widget_set_sensitive(img->trans_duration,TRUE);

	/* Block "changed" signal from model to avoid rewriting the same value back into current slide. */
	g_signal_handlers_block_by_func((gpointer)model, (gpointer)img_combo_box_transition_type_changed, img);
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->transition_type), info_slide->combo_transition_type_index );
	g_signal_handlers_unblock_by_func((gpointer)model, (gpointer)img_combo_box_transition_type_changed, img);

	/* Set the transition speed */
	if (info_slide->speed == FAST)
		dummy = 0;
	else if (info_slide->speed == NORMAL)
		dummy = 1;
	else
		dummy = 2;
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->trans_duration),dummy);

	/* Set the transition duration */
	gtk_spin_button_set_value((GtkSpinButton*)img->duration, info_slide->duration);

	gtk_label_set_text(GTK_LABEL (img->slide_selected_data),selected_slide);
	g_free(selected_slide);
	gtk_label_set_text(GTK_LABEL (img->type_data),info_slide->type);
	gtk_label_set_text(GTK_LABEL (img->resolution_data),info_slide->resolution);
	if (nr_selected > 1)
		img_set_statusbar_message(img,nr_selected);
	else
		gtk_statusbar_push(GTK_STATUSBAR (img->statusbar),img->context_id,info_slide->filename);

	img->slide_pixbuf = img_scale_pixbuf(img,info_slide->filename);
	gtk_image_set_from_pixbuf(GTK_IMAGE (img->image_area),img->slide_pixbuf);
	g_object_unref(img->slide_pixbuf);
}

static void img_combo_box_transition_type_changed (GtkComboBox *combo, img_window_struct *img)
{
	GList *selected;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gpointer address;
	slide_struct *info_slide;
	gint render_index;

	/* Get the address of the transition function stored in the model of the combo box*/
	model = gtk_combo_box_get_model(combo);
	gtk_combo_box_get_active_iter(combo,&iter);
	gtk_tree_model_get(model,&iter,1,&address,-1);

	/* Get index of currently selected item */
	render_index = gtk_combo_box_get_active(combo);
	if (render_index == 0)
		gtk_widget_set_sensitive(img->trans_duration,FALSE);
	else
		gtk_widget_set_sensitive(img->trans_duration,TRUE);

	model = gtk_icon_view_get_model(GTK_ICON_VIEW (img->thumbnail_iconview));
	selected = gtk_icon_view_get_selected_items(GTK_ICON_VIEW (img->thumbnail_iconview));
	if (selected == NULL)
		return;

	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->render = address;
		info_slide->combo_transition_type_index = render_index;
		selected = selected->next;
	}
	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(selected);
}

static void img_combo_box_speed_changed (GtkComboBox *combo, img_window_struct *img)
{
	gint speed;
	gdouble duration;
	GList *selected;
	GtkTreeIter iter;
	GtkTreeModel *model;
	slide_struct *info_slide;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW (img->thumbnail_iconview));
	selected = gtk_icon_view_get_selected_items(GTK_ICON_VIEW (img->thumbnail_iconview));
	if (selected == NULL)
		return;

	speed = gtk_combo_box_get_active(combo);

	if (speed == 0)
		duration = FAST;
	else if (speed == 1)
		duration = NORMAL;
	else 
		duration = SLOW;

	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->speed = duration;
		selected = selected->next;
	}
	img_set_total_slideshow_duration(img);

	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(selected);
}

static void img_spinbutton_value_changed (GtkSpinButton *spinbutton, img_window_struct *img)
{
	gint duration = 0;
	GList *selected;
	GtkTreeIter iter;
	GtkTreeModel *model;
	slide_struct *info_slide;

	model = gtk_icon_view_get_model((GtkIconView *)img->thumbnail_iconview);
	selected = gtk_icon_view_get_selected_items((GtkIconView *)img->thumbnail_iconview);
	if (selected == NULL)
		return;

	duration = gtk_spin_button_get_value_as_int(spinbutton);
	
	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->duration = duration;
		selected = selected->next;
	}
	img_set_total_slideshow_duration(img);

	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(selected);
}

static void img_select_all_thumbnails(GtkMenuItem *item, img_window_struct *img)
{
	gtk_icon_view_select_all(GTK_ICON_VIEW (img->thumbnail_iconview));
}

static void img_unselect_all_thumbnails(GtkMenuItem *item, img_window_struct *img)
{
	gtk_icon_view_unselect_all(GTK_ICON_VIEW (img->thumbnail_iconview));
}

static void img_goto_slide(GtkMenuItem *item, img_window_struct *img_struct)
{
	GtkWidget *vbox;
	GtkWidget *text_label;
	GtkWidget *slide_number_entry;
	gint response;

	img_struct->goto_window = gtk_dialog_new_with_buttons(_("Go to slide"),GTK_WINDOW (img_struct->imagination_window),
										GTK_DIALOG_DESTROY_WITH_PARENT,
										GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										GTK_STOCK_JUMP_TO, GTK_RESPONSE_ACCEPT, NULL);
	vbox = gtk_vbox_new(FALSE, 12);
	gtk_container_set_border_width(GTK_CONTAINER (vbox), 5);
	gtk_container_add(GTK_CONTAINER (GTK_DIALOG (img_struct->goto_window)->vbox), vbox);

	text_label = gtk_label_new_with_mnemonic(_("Enter the slide number you want to go to:"));
	gtk_misc_set_alignment(GTK_MISC (text_label), 0, 0.5);
	slide_number_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY (slide_number_entry), 4);
	gtk_entry_set_width_chars(GTK_ENTRY (slide_number_entry), 30);
	g_signal_connect(G_OBJECT (slide_number_entry), "activate", G_CALLBACK(img_goto_line_entry_activate), img_struct);

	gtk_container_add(GTK_CONTAINER (vbox), text_label);
	gtk_container_add(GTK_CONTAINER (vbox), slide_number_entry);
	gtk_widget_show_all(img_struct->goto_window);

	response = gtk_dialog_run(GTK_DIALOG (img_struct->goto_window));
	if (response == GTK_RESPONSE_CANCEL || response == GTK_RESPONSE_DELETE_EVENT)
	{
		gtk_widget_destroy(img_struct->goto_window);
		return;
	}
	img_goto_line_entry_activate(GTK_ENTRY (slide_number_entry),img_struct);
}

static void img_goto_line_entry_activate(GtkEntry *entry, img_window_struct *img)
{
	gint slide;
	GtkTreePath *path;

	slide = strtol(gtk_entry_get_text(entry), NULL, 10);
	if (slide > 0 && slide <= img->slides_nr)
	{
		gtk_icon_view_unselect_all(GTK_ICON_VIEW (img->thumbnail_iconview));
		path = gtk_tree_path_new_from_indices(slide-1,-1);
		gtk_icon_view_set_cursor (GTK_ICON_VIEW (img->thumbnail_iconview), path, NULL, FALSE);
		gtk_icon_view_select_path (GTK_ICON_VIEW (img->thumbnail_iconview), path);
		gtk_icon_view_scroll_to_path (GTK_ICON_VIEW (img->thumbnail_iconview), path, FALSE, 0, 0);
		gtk_tree_path_free (path);
	}
	gtk_widget_destroy(img->goto_window);
}

