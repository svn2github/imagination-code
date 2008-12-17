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

#include "new_slideshow.h"

void img_new_slideshow_settings_dialog(img_window_struct *img)
{
	GtkWidget *dialog1;
	GtkWidget *dialog_vbox1;
	GtkWidget *vbox1;
	GtkWidget *main_frame;
	GtkWidget *alignment_main_frame;
	GtkWidget *vbox_frame1;
	GtkWidget *hbox_slideshow_name;
	GtkWidget *label2;
	GtkWidget *slideshow_title_entry;
	GtkWidget *frame2;
	GtkWidget *alignment_frame2;
	GtkWidget *vbox_video_format;
	GtkWidget *radiobutton1;
	GtkWidget *radiobutton2;
	GtkWidget *label_frame2;
	GtkWidget *label1;
	GtkWidget *dialog_action_area1;
	GtkWidget *button1;
	GtkWidget *button2;
	GSList *radiobutton1_group = NULL;
	gint response;

	dialog1 = gtk_dialog_new ();
	gtk_container_set_border_width (GTK_CONTAINER (dialog1), 5);
	gtk_widget_set_size_request(dialog1,520,248);
	gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_type_hint (GTK_WINDOW (dialog1), GDK_WINDOW_TYPE_HINT_DIALOG);
	gtk_dialog_set_has_separator (GTK_DIALOG (dialog1), FALSE);

	dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;
	gtk_widget_show (dialog_vbox1);

	vbox1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox1);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

	main_frame = gtk_frame_new (NULL);
	gtk_widget_show (main_frame);
	gtk_box_pack_start (GTK_BOX (vbox1), main_frame, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (main_frame), GTK_SHADOW_IN);

	alignment_main_frame = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment_main_frame);
	gtk_container_add (GTK_CONTAINER (main_frame), alignment_main_frame);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_main_frame), 5, 15, 10, 10);

	vbox_frame1 = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_frame1);
	gtk_container_add (GTK_CONTAINER (alignment_main_frame), vbox_frame1);

	hbox_slideshow_name = gtk_hbox_new (FALSE, 50);
	gtk_widget_show (hbox_slideshow_name);
	gtk_box_pack_start (GTK_BOX (vbox_frame1), hbox_slideshow_name, TRUE, TRUE, 0);

	label2 = gtk_label_new (_("Title of the slideshow:"));
	gtk_widget_show (label2);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_name), label2, FALSE, TRUE, 0);

	slideshow_title_entry = gtk_entry_new ();
	gtk_widget_show (slideshow_title_entry);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_name), slideshow_title_entry, TRUE, TRUE, 0);
	gtk_entry_set_invisible_char (GTK_ENTRY (slideshow_title_entry), 9679);

	frame2 = gtk_frame_new (NULL);
	gtk_widget_show (frame2);
	gtk_box_pack_start (GTK_BOX (vbox_frame1), frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);

	alignment_frame2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_widget_show (alignment_frame2);
	gtk_container_add (GTK_CONTAINER (frame2), alignment_frame2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_frame2), 5, 5, 5, 5);

	vbox_video_format = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox_video_format);
	gtk_container_add (GTK_CONTAINER (alignment_frame2), vbox_video_format);

	radiobutton1 = gtk_radio_button_new_with_mnemonic (NULL, _("PAL 720x576"));
	gtk_widget_show (radiobutton1);
	gtk_box_pack_start (GTK_BOX (vbox_video_format), radiobutton1, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton1), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton1));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton1), TRUE);

	radiobutton2 = gtk_radio_button_new_with_mnemonic (NULL, _("NTSC 720x480"));
	gtk_widget_show (radiobutton2);
	gtk_box_pack_start (GTK_BOX (vbox_video_format), radiobutton2, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton2), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton2));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton2), TRUE);

	label_frame2 = gtk_label_new (_("<b>Video Format</b>"));
	gtk_widget_show (label_frame2);
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label_frame2);
	gtk_label_set_use_markup (GTK_LABEL (label_frame2), TRUE);

	label1 = gtk_label_new (_("<b>Slideshow Settings</b>"));
	gtk_widget_show (label1);
	gtk_frame_set_label_widget (GTK_FRAME (main_frame), label1);
	gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

	dialog_action_area1 = GTK_DIALOG (dialog1)->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_SPREAD);

	button1 = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (button1);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), button1, 0);

	button2 = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (button2);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog1), button2, 0);

	response = gtk_dialog_run(GTK_DIALOG(dialog1));
	if (response == GTK_RESPONSE_OK || response == GTK_RESPONSE_ACCEPT)
	{
		img->slideshow_title = g_strdup(gtk_entry_get_text(GTK_ENTRY(slideshow_title_entry)));
		//img->slideshoow_height = ;
	}
	gtk_widget_destroy(dialog1);
}
