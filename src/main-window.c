/*
 *  Copyright (c) 2009 Giuseppe Torelli <colossus73@gmail.com>
 *  Copyright (c) 2009 Tadej Borovšak 	<tadeboro@gmail.com>
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

static void img_combo_box_transition_type_changed (GtkComboBox *, img_window_struct *);
static void img_combo_box_speed_changed (GtkComboBox *,  img_window_struct *);
static void img_spinbutton_value_changed (GtkSpinButton *, img_window_struct *);
static void img_quit_menu(GtkMenuItem *, img_window_struct *);
static void img_select_all_thumbnails(GtkMenuItem *, img_window_struct *);
static void img_unselect_all_thumbnails(GtkMenuItem *, img_window_struct *);
static void img_goto_line_entry_activate(GtkWidget *, img_window_struct *);
static gint img_sort_none_before_other(GtkTreeModel *, GtkTreeIter *, GtkTreeIter *, gpointer);
static void img_check_numeric_entry (GtkEditable *entry, gchar *text, gint lenght, gint *position, gpointer data);

img_window_struct *img_create_window (void)
{
	img_window_struct *img_struct = NULL;
	GtkWidget *vbox1;
	GtkWidget *menubar;
	GtkWidget *menuitem1;
	GtkWidget *menu1;
	GtkWidget *imagemenuitem1;
	GtkWidget *imagemenuitem5;
	GtkWidget *separatormenuitem1;
	GtkWidget *menuitem2;
	GtkWidget *slide_menu;
	GtkWidget *separator_slide_menu;
	GtkWidget *image_menu;
	GtkWidget *select_all_menu;
	GtkWidget *deselect_all_menu;
	GtkWidget *menuitem3;
	GtkWidget *tmp_image;
	GtkWidget *menu3;
	GtkWidget *imagemenuitem11;
	GtkWidget *toolbar;
	GtkWidget *new_button;
	GtkWidget *separatortoolitem;
	GtkWidget *toolbutton_slide_goto;
	GtkWidget *goto_button;
	GtkWidget *hbox;
	GtkWidget *swindow;
	GtkWidget *viewport;
	GtkWidget *align;
	GtkWidget *image_area_frame;
	GtkWidget *valign;
	GtkWidget *halign;
	GtkWidget *frame1_alignment;
	GtkWidget *transition_label;
	GtkWidget *vbox_info_slide;
	GtkWidget *hbox_effect_duration;
	GtkWidget *hbox_duration;
	GtkWidget *duration_label;
	GtkWidget *trans_duration_label;
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
	GtkCellRenderer *pixbuf_cell;
	GtkIconSize tmp_toolbar_icon_size;
	GtkIconTheme *icon_theme;
	GdkPixbuf *icon;

	accel_group = gtk_accel_group_new ();
	icon_theme = gtk_icon_theme_get_default();
	icon = gtk_icon_theme_load_icon(icon_theme,"imagination",24,0,NULL);

	img_struct = g_new0(img_window_struct,1);
	img_struct->imagination_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_icon (GTK_WINDOW(img_struct->imagination_window),icon);
	gtk_window_set_default_size (GTK_WINDOW (img_struct->imagination_window), 1030, 800);
	img_set_window_title(img_struct,NULL);
	g_signal_connect (G_OBJECT (img_struct->imagination_window),"delete-event",G_CALLBACK (img_quit_application),img_struct);
	g_signal_connect (G_OBJECT (img_struct->imagination_window), "destroy", G_CALLBACK (gtk_main_quit), NULL );

	vbox1 = gtk_vbox_new (FALSE, 2);
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

	img_struct->open_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_OPEN, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->open_menu);
	g_signal_connect (G_OBJECT (img_struct->open_menu),"activate",G_CALLBACK (img_choose_slideshow_filename),img_struct);

	img_struct->save_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->save_menu);
	gtk_widget_set_sensitive(img_struct->save_menu, FALSE);
	g_signal_connect (G_OBJECT (img_struct->save_menu),"activate",G_CALLBACK (img_choose_slideshow_filename),img_struct);

	img_struct->save_as_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_SAVE_AS, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->save_as_menu);
	gtk_widget_set_sensitive(img_struct->save_as_menu, FALSE);
	g_signal_connect (G_OBJECT (img_struct->save_as_menu),"activate",G_CALLBACK (img_choose_slideshow_filename),img_struct);

	img_struct->close_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_CLOSE, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->close_menu);
	gtk_widget_set_sensitive(img_struct->close_menu, FALSE);
	g_signal_connect (G_OBJECT (img_struct->close_menu),"activate",G_CALLBACK (img_close_slideshow),img_struct);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	img_struct->properties_menu = gtk_image_menu_item_new_from_stock (GTK_STOCK_PROPERTIES, accel_group);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->properties_menu);
	gtk_widget_set_sensitive(img_struct->properties_menu, FALSE);
	g_signal_connect (G_OBJECT (img_struct->properties_menu), "activate", G_CALLBACK (img_project_properties), img_struct);

	separatormenuitem1 = gtk_separator_menu_item_new ();
	gtk_container_add (GTK_CONTAINER (menu1), separatormenuitem1);
	gtk_widget_set_sensitive (separatormenuitem1, FALSE);

	img_struct->preview_menu = gtk_image_menu_item_new_with_mnemonic (_("_Preview"));
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->preview_menu);
	gtk_widget_set_sensitive(img_struct->preview_menu, FALSE);
	gtk_widget_add_accelerator (img_struct->preview_menu, "activate",accel_group,GDK_p,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (img_struct->preview_menu),"activate",G_CALLBACK (img_start_stop_preview),img_struct);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->preview_menu),tmp_image);

	img_struct->export_menu = gtk_image_menu_item_new_with_mnemonic (_("Export"));
	gtk_widget_add_accelerator (img_struct->export_menu,"activate",accel_group,GDK_g,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	gtk_container_add (GTK_CONTAINER (menu1), img_struct->export_menu);
	gtk_widget_set_sensitive(img_struct->export_menu, FALSE);
	g_signal_connect(G_OBJECT(img_struct->export_menu), "activate", G_CALLBACK(img_start_stop_export), img_struct);

	image_menu = img_load_icon ("imagination-generate.png",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->export_menu),image_menu);

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

	img_struct->import_menu = gtk_image_menu_item_new_with_mnemonic (_("_Import"));
	gtk_container_add (GTK_CONTAINER (slide_menu),img_struct->import_menu);
	gtk_widget_set_sensitive(img_struct->import_menu, FALSE);
	gtk_widget_add_accelerator (img_struct->import_menu,"activate",accel_group,GDK_i,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (img_struct->import_menu),"activate",G_CALLBACK (img_add_slides_thumbnails),img_struct);

	image_menu = img_load_icon ("imagination-import.png",GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->import_menu),image_menu);

	img_struct->remove_menu = gtk_image_menu_item_new_with_mnemonic (_("_Delete"));
	gtk_widget_set_sensitive(img_struct->remove_menu, FALSE);
	gtk_container_add (GTK_CONTAINER (slide_menu), img_struct->remove_menu);
	gtk_widget_add_accelerator (img_struct->remove_menu,"activate",accel_group, GDK_d,GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);
	g_signal_connect (G_OBJECT (img_struct->remove_menu),"activate",G_CALLBACK (img_delete_selected_slides),img_struct);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_DELETE,GTK_ICON_SIZE_MENU);
	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (img_struct->remove_menu),tmp_image);

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

	img_struct->open_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_OPEN));
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->open_button);
	gtk_widget_set_tooltip_text(img_struct->open_button, _("Open a slideshow"));
	g_signal_connect (G_OBJECT (img_struct->open_button),"clicked",G_CALLBACK (img_choose_slideshow_filename),img_struct);

	img_struct->save_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_SAVE));
	gtk_container_add (GTK_CONTAINER (toolbar), img_struct->save_button);
	gtk_widget_set_sensitive(img_struct->save_button, FALSE);
	gtk_widget_set_tooltip_text(img_struct->save_button, _("Save the slideshow"));
	g_signal_connect (G_OBJECT (img_struct->save_button),"clicked",G_CALLBACK (img_choose_slideshow_filename),img_struct);

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	tmp_image = img_load_icon("imagination-import.png",GTK_ICON_SIZE_LARGE_TOOLBAR);
	img_struct->import_button = GTK_WIDGET (gtk_tool_button_new (tmp_image,""));
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->import_button);
	gtk_widget_set_sensitive(img_struct->import_button, FALSE);
	gtk_widget_set_tooltip_text(img_struct->import_button, _("Import the slides"));
	g_signal_connect ((gpointer) img_struct->import_button,"clicked",G_CALLBACK (img_add_slides_thumbnails),img_struct);

	img_struct->remove_button = GTK_WIDGET (gtk_tool_button_new_from_stock ("gtk-delete"));
	gtk_widget_set_sensitive(img_struct->remove_button, FALSE);
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->remove_button);
	gtk_widget_set_tooltip_text(img_struct->remove_button, _("Delete the selected slides"));
	g_signal_connect (G_OBJECT (img_struct->remove_button),"clicked",G_CALLBACK (img_delete_selected_slides),img_struct);

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	img_struct->preview_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_MEDIA_PLAY));
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->preview_button);
	gtk_widget_set_sensitive(img_struct->preview_button, FALSE);
	gtk_widget_set_tooltip_text(img_struct->preview_button, _("Preview the slideshow"));
	g_signal_connect (G_OBJECT (img_struct->preview_button),"clicked",G_CALLBACK (img_start_stop_preview),img_struct);

	tmp_image = img_load_icon("imagination-generate.png",GTK_ICON_SIZE_LARGE_TOOLBAR);
	img_struct->export_button = GTK_WIDGET (gtk_tool_button_new (tmp_image,""));
	gtk_container_add (GTK_CONTAINER (toolbar),img_struct->export_button);
	gtk_widget_set_sensitive(img_struct->export_button, FALSE);
	gtk_widget_set_tooltip_text(img_struct->export_button, _("Export the DVD slideshow"));
	g_signal_connect(G_OBJECT(img_struct->export_button), "clicked", G_CALLBACK(img_start_stop_export), img_struct);

	separatortoolitem = GTK_WIDGET (gtk_separator_tool_item_new());
	gtk_widget_show (separatortoolitem);
	gtk_container_add (GTK_CONTAINER (toolbar),separatortoolitem);

	toolbutton_slide_goto = GTK_WIDGET(gtk_tool_item_new ());
	gtk_container_add (GTK_CONTAINER (toolbar), toolbutton_slide_goto);
 
	img_struct->slide_number_entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY (img_struct->slide_number_entry), 4);
	gtk_entry_set_width_chars(GTK_ENTRY (img_struct->slide_number_entry), 6);
	gtk_container_add(GTK_CONTAINER(toolbutton_slide_goto),img_struct->slide_number_entry);
	g_signal_connect(G_OBJECT (img_struct->slide_number_entry), "activate", G_CALLBACK(img_goto_line_entry_activate), img_struct);
	g_signal_connect(G_OBJECT (img_struct->slide_number_entry), "insert-text", G_CALLBACK (img_check_numeric_entry), NULL );
	
	goto_button = GTK_WIDGET (gtk_tool_button_new_from_stock (GTK_STOCK_JUMP_TO));
	gtk_container_add (GTK_CONTAINER (toolbar),goto_button);
	gtk_widget_set_tooltip_text(goto_button, _("Jump to the entered slide number"));
	g_signal_connect (G_OBJECT (goto_button),"clicked",G_CALLBACK (img_goto_line_entry_activate),img_struct);

	gtk_widget_show_all (toolbar);

	/* Create the image area and the other widgets */
	hbox = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox, TRUE, TRUE, 0);

	swindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER(swindow), 10);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox), swindow, TRUE, TRUE, 0);

	align = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), align);

	image_area_frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(image_area_frame), GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(align), image_area_frame);

	align = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(image_area_frame), align);

	img_struct->image_area = gtk_image_new();
	gtk_widget_set_size_request(img_struct->image_area, 720, 576);
	gtk_container_add(GTK_CONTAINER(align), img_struct->image_area);

	viewport = gtk_bin_get_child(GTK_BIN(swindow));
	gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);

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
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(GTK_LIST_STORE(gtk_combo_box_get_model(GTK_COMBO_BOX(img_struct->transition_type)))),
										0,GTK_SORT_ASCENDING);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(gtk_combo_box_get_model(GTK_COMBO_BOX(img_struct->transition_type))),0,img_sort_none_before_other,NULL,NULL);										
	gtk_widget_set_sensitive(img_struct->transition_type, FALSE);
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
	gtk_widget_set_sensitive(img_struct->trans_duration, FALSE);
	gtk_box_pack_end (GTK_BOX (hbox_effect_duration), img_struct->trans_duration, FALSE, TRUE, 0);
	g_signal_connect (G_OBJECT (img_struct->trans_duration),"changed",G_CALLBACK (img_combo_box_speed_changed),img_struct);

	/* Slide duration */
	hbox_duration = gtk_hbox_new (FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_info_slide), hbox_duration, FALSE, TRUE, 0);
	duration_label = gtk_label_new (_("Slide Duration in sec:"));
	gtk_box_pack_start (GTK_BOX (hbox_duration), duration_label, FALSE, FALSE, 0);

	img_struct->duration = gtk_spin_button_new_with_range (1, 300, 1);
	gtk_widget_set_sensitive(img_struct->duration, FALSE);
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
	img_struct->thumb_scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_box_pack_start (GTK_BOX (vbox1), img_struct->thumb_scrolledwindow, FALSE, TRUE, 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (img_struct->thumb_scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (img_struct->thumb_scrolledwindow), GTK_SHADOW_IN);
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
	gtk_container_add (GTK_CONTAINER (img_struct->thumb_scrolledwindow), img_struct->thumbnail_iconview);
	g_signal_connect (G_OBJECT (img_struct->thumbnail_iconview),"selection-changed",G_CALLBACK (img_iconview_selection_changed),img_struct);
	g_signal_connect (G_OBJECT (img_struct->thumbnail_iconview),"select-all",G_CALLBACK (img_iconview_selection_changed),img_struct);

	/* Create the status bar */
	img_struct->statusbar = gtk_statusbar_new ();
	gtk_widget_show (img_struct->statusbar);
	gtk_box_pack_start (GTK_BOX (vbox1), img_struct->statusbar, FALSE, TRUE, 0);
	img_struct->context_id = gtk_statusbar_get_context_id (GTK_STATUSBAR (img_struct->statusbar), "statusbar");

	/* Create the progress bar */
	img_struct->progress_bar = gtk_progress_bar_new();
	gtk_widget_set_size_request (img_struct->progress_bar, -1, 15);
	{
		GtkWidget *vbox;

		vbox = gtk_vbox_new (FALSE, 0);
		gtk_box_pack_start (GTK_BOX (img_struct->statusbar), vbox, FALSE, FALSE, 0);
		gtk_box_pack_start (GTK_BOX (vbox), img_struct->progress_bar, TRUE, FALSE, 0);
		gtk_widget_show (vbox);
	}
	gtk_widget_show_all(hbox);
	gtk_window_add_accel_group (GTK_WINDOW (img_struct->imagination_window), accel_group);

	return img_struct;
}

static void img_quit_menu(GtkMenuItem *menuitem, img_window_struct *img)
{
	gboolean value;

	g_signal_emit_by_name(img->imagination_window,"delete-event", img, &value);

	if(value)
		return;

	/* Unloads the plugins */
	g_slist_foreach(img->plugin_list,(GFunc)g_module_close,NULL);
	g_slist_free(img->plugin_list);

	gtk_main_quit();
}

void img_iconview_selection_changed(GtkIconView *iconview, img_window_struct *img)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkTreePath *path = NULL;
	gint dummy, nr_selected = 0;
	gchar *selected_slide = NULL;
	GList *selected = NULL;
	slide_struct *info_slide;

	if (img->preview_is_running || img->export_is_running)
		return;

	model = gtk_icon_view_get_model(iconview);
	gtk_icon_view_get_cursor(iconview,&path,NULL);

	selected = gtk_icon_view_get_selected_items(iconview);
	nr_selected = g_list_length(selected);

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
		gtk_label_set_text(GTK_LABEL (img->total_time_data),"");
		return;
	}
	if (nr_selected >= 1)
		img_set_statusbar_message(img,nr_selected);
	else
		gtk_statusbar_pop(GTK_STATUSBAR(img->statusbar),img->context_id);
	
	gtk_widget_set_sensitive(img->remove_menu,		TRUE);
	gtk_widget_set_sensitive(img->remove_button,	TRUE);
	gtk_widget_set_sensitive(img->trans_duration,	TRUE);
	gtk_widget_set_sensitive(img->duration,			TRUE);
	gtk_widget_set_sensitive(img->transition_type,	TRUE);

	dummy = gtk_tree_path_get_indices(selected->data)[0]+1;
	selected_slide = g_strdup_printf("%d",dummy);
	gtk_tree_model_get_iter(model,&iter,selected->data);
	g_list_foreach (selected, (GFunc)gtk_tree_path_free, NULL);
	g_list_free (selected);
	gtk_tree_model_get(model,&iter,1,&info_slide,-1);

	/* Set the transition type */
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(img->transition_type));

	/* Block "changed" signal from model to avoid rewriting the same value back into current slide. */
	g_signal_handlers_block_by_func((gpointer)img->transition_type, (gpointer)img_combo_box_transition_type_changed, img);
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->transition_type), info_slide->combo_transition_type_index );
	g_signal_handlers_unblock_by_func((gpointer)img->transition_type, (gpointer)img_combo_box_transition_type_changed, img);

	/* Moved this piece of code below the setting the transition, since we
	 * get false negatives in certain situations (eg.: if the previously
	 * selected transition doesn't have transition renderer set): */
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(img->transition_type)) == 0)
		gtk_widget_set_sensitive(img->trans_duration,FALSE);
	else
		gtk_widget_set_sensitive(img->trans_duration,TRUE);

	/* Set the transition speed */
	if (info_slide->speed == FAST)
		dummy = 0;
	else if (info_slide->speed == NORMAL)
		dummy = 1;
	else
		dummy = 2;
	g_signal_handlers_block_by_func((gpointer)img->duration, (gpointer)img_combo_box_speed_changed, img);
	gtk_combo_box_set_active(GTK_COMBO_BOX(img->trans_duration),dummy);
	g_signal_handlers_block_by_func((gpointer)img->duration, (gpointer)img_combo_box_speed_changed, img);

	/* Set the transition duration */
	g_signal_handlers_block_by_func((gpointer)img->duration, (gpointer)img_spinbutton_value_changed, img);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(img->duration), info_slide->duration);
	g_signal_handlers_unblock_by_func((gpointer)img->duration, (gpointer)img_spinbutton_value_changed, img);

	gtk_label_set_text(GTK_LABEL (img->slide_selected_data),selected_slide);
	g_free(selected_slide);
	gtk_label_set_text(GTK_LABEL (img->type_data),info_slide->type);
	gtk_label_set_text(GTK_LABEL (img->resolution_data),info_slide->resolution);
	/* Added missing total label "setter". Current method is not the most
	 * efficient one, since we're recalculating the whole duration when we
	 * only need to display it. BTW, is total duration label hidding needed?
	 * Even when there is no slide selected, duration stays the same. */
	img_set_total_slideshow_duration(img);

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
	GList *selected, *bak;
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

	/* Avoiding GList memory leak. */
	bak = selected;
	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->render = address;
		info_slide->combo_transition_type_index = render_index;
		selected = selected->next;
	}
	img->project_is_modified = TRUE;
	img_set_total_slideshow_duration(img);
	g_list_foreach (bak, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(bak);
}

static void img_combo_box_speed_changed (GtkComboBox *combo, img_window_struct *img)
{
	gint speed;
	gdouble duration;
	GList *selected, *bak;
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

	/* Avoid memory leak */
	bak = selected;
	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->speed = duration;
		selected = selected->next;
	}
	img_set_total_slideshow_duration(img);

	g_list_foreach (bak, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(bak);
}

static void img_spinbutton_value_changed (GtkSpinButton *spinbutton, img_window_struct *img)
{
	gint duration = 0;
	GList *selected, *bak;
	GtkTreeIter iter;
	GtkTreeModel *model;
	slide_struct *info_slide;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(img->thumbnail_iconview));
	selected = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(img->thumbnail_iconview));
	if (selected == NULL)
		return;

	duration = gtk_spin_button_get_value_as_int(spinbutton);
	
	bak = selected;
	while (selected)
	{
		gtk_tree_model_get_iter(model, &iter,selected->data);
		gtk_tree_model_get(model, &iter,1,&info_slide,-1);
		info_slide->duration = duration;
		selected = selected->next;
	}
	img_set_total_slideshow_duration(img);

	g_list_foreach (bak, (GFunc)gtk_tree_path_free, NULL);
	g_list_free(bak);
}

static void img_select_all_thumbnails(GtkMenuItem *item, img_window_struct *img)
{
	gtk_icon_view_select_all(GTK_ICON_VIEW (img->thumbnail_iconview));
}

static void img_unselect_all_thumbnails(GtkMenuItem *item, img_window_struct *img)
{
	gtk_icon_view_unselect_all(GTK_ICON_VIEW (img->thumbnail_iconview));
}

static void img_goto_line_entry_activate(GtkWidget *entry, img_window_struct *img)
{
	gint slide;
	GtkTreePath *path;

	slide = strtol(gtk_entry_get_text(GTK_ENTRY(img->slide_number_entry)), NULL, 10);
	if (slide > 0 && slide <= img->slides_nr)
	{
		gtk_icon_view_unselect_all(GTK_ICON_VIEW (img->thumbnail_iconview));
		path = gtk_tree_path_new_from_indices(slide-1,-1);
		gtk_icon_view_set_cursor (GTK_ICON_VIEW (img->thumbnail_iconview), path, NULL, FALSE);
		gtk_icon_view_select_path (GTK_ICON_VIEW (img->thumbnail_iconview), path);
		gtk_icon_view_scroll_to_path (GTK_ICON_VIEW (img->thumbnail_iconview), path, FALSE, 0, 0);
		gtk_tree_path_free (path);
	}
}

static gint img_sort_none_before_other(GtkTreeModel *model,GtkTreeIter *a,GtkTreeIter *b,gpointer data)
{
	gchar *name1, *name2;
	gint i;

	gtk_tree_model_get(model, a, 0, &name1, -1);
	gtk_tree_model_get(model, b, 0, &name2, -1);

	if (strcmp(name1,_("None")) == 0)
		i = -1;
	else if(strcmp(name2,_("None")) == 0)
		i = 1;
	else
		i = (g_strcmp0 (name1,name2));
	
	g_free(name1);
	g_free(name2);
	return i;	
}

static void img_check_numeric_entry (GtkEditable *entry, gchar *text, gint lenght, gint *position, gpointer data)
{
	if(*text < '0' || *text > '9')
		g_signal_stop_emission_by_name( (gpointer)entry, "insert-text" );
}

