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
	GtkWidget *hbox_slideshow_options;
	GtkWidget *hbox_slideshow_name;
	GtkWidget *hbox_slideshow_fmt;
	GtkWidget *label2;
	GtkWidget *label3;
	GtkWidget *slideshow_fmt_combo;
	GtkWidget *slideshow_title_entry;
	GtkWidget *frame1;
	GtkWidget *label_frame1;
	GtkWidget *alignment_frame1;
	GtkWidget *frame2;
	GtkWidget *label_frame2;
	GtkWidget *alignment_frame2;
	GtkWidget *vbox_video_format, *vbox_aspect_ratio;
	GtkWidget *pal,*ntsc,*tv,*wide;
	GtkWidget *label1;
	GSList *radiobutton1_group = NULL;
	GSList *radiobutton2_group = NULL;
	gint response;

	dialog1 = gtk_dialog_new_with_buttons(_("Create a new slideshow"),(GtkWindow*)img->imagination_window,
										GTK_DIALOG_DESTROY_WITH_PARENT,
										GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);

	gtk_button_box_set_layout (GTK_BUTTON_BOX (GTK_DIALOG (dialog1)->action_area), GTK_BUTTONBOX_SPREAD);
	gtk_widget_set_size_request(dialog1,520,248);
	gtk_dialog_set_has_separator (GTK_DIALOG (dialog1), FALSE);

	dialog_vbox1 = GTK_DIALOG (dialog1)->vbox;

	vbox1 = gtk_vbox_new (FALSE, 5);
	gtk_container_set_border_width (GTK_CONTAINER (vbox1), 5);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), vbox1, TRUE, TRUE, 0);

	main_frame = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (vbox1), main_frame, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (main_frame), GTK_SHADOW_IN);

	label1 = gtk_label_new (_("<b>Slideshow Settings</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (main_frame), label1);
	gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);

	alignment_main_frame = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (main_frame), alignment_main_frame);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_main_frame), 5, 15, 10, 10);

	vbox_frame1 = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment_main_frame), vbox_frame1);

	hbox_slideshow_name = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_frame1), hbox_slideshow_name, TRUE, TRUE, 0);

	label2 = gtk_label_new (_("Filename of the slideshow:"));
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_name), label2, FALSE, TRUE, 0);
	gtk_misc_set_alignment(GTK_MISC(label2), 0, 0.5);

	slideshow_title_entry = sexy_icon_entry_new();
	sexy_icon_entry_add_clear_button( SEXY_ICON_ENTRY(slideshow_title_entry), img, img_show_file_chooser);
    sexy_icon_entry_set_icon_highlight( SEXY_ICON_ENTRY(slideshow_title_entry), SEXY_ICON_ENTRY_PRIMARY, TRUE);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_name), slideshow_title_entry, TRUE, TRUE, 0);

	hbox_slideshow_fmt = gtk_hbox_new (TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox_frame1), hbox_slideshow_fmt, FALSE, TRUE, 0);

	label3 = gtk_label_new (_("Export the slideshow as:"));
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_fmt), label3, FALSE, TRUE, 0);
	gtk_misc_set_alignment(GTK_MISC(label3), 0, 0.5);

	slideshow_fmt_combo = _gtk_combo_box_new_text(FALSE);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_fmt), slideshow_fmt_combo, TRUE, TRUE, 0);
	gtk_combo_box_append_text(GTK_COMBO_BOX(slideshow_fmt_combo),"VOB (DVD VIDEO)");
	gtk_combo_box_append_text(GTK_COMBO_BOX(slideshow_fmt_combo),"FLV (FLash Video)");
	gtk_combo_box_set_active(GTK_COMBO_BOX(slideshow_fmt_combo),0);

	hbox_slideshow_options = gtk_hbox_new(TRUE,10);
	gtk_box_pack_start(GTK_BOX (vbox_frame1), hbox_slideshow_options, TRUE, TRUE, 10);
	
	frame1 = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_options), frame1, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_IN);

	alignment_frame1 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame1), alignment_frame1);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_frame1), 5, 5, 5, 5);

	vbox_video_format = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment_frame1), vbox_video_format);

	pal = gtk_radio_button_new_with_mnemonic (NULL, "PAL 720 x 576");
	gtk_box_pack_start (GTK_BOX (vbox_video_format), pal, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (pal), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (pal));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);

	ntsc = gtk_radio_button_new_with_mnemonic (NULL, "NTSC 720 x 480");
	gtk_box_pack_start (GTK_BOX (vbox_video_format), ntsc, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (ntsc), radiobutton1_group);
	radiobutton1_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (ntsc));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);

	label_frame1 = gtk_label_new (_("<b>Video Format</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame1), label_frame1);
	gtk_label_set_use_markup (GTK_LABEL (label_frame1), TRUE);

	frame2 = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_options), frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);

	alignment_frame2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame2), alignment_frame2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_frame2), 5, 5, 5, 5);

	vbox_aspect_ratio = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment_frame2), vbox_aspect_ratio);

	tv = gtk_radio_button_new_with_mnemonic (NULL, _("Normal 4:3"));
	gtk_box_pack_start (GTK_BOX (vbox_aspect_ratio), tv, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (tv), radiobutton2_group);
	radiobutton2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON(tv));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);

	wide = gtk_radio_button_new_with_mnemonic (NULL, _("Widescreen 16:9"));
	gtk_box_pack_start (GTK_BOX (vbox_aspect_ratio), wide, TRUE, TRUE, 0);
	gtk_radio_button_set_group (GTK_RADIO_BUTTON (wide), radiobutton2_group);
	radiobutton2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (wide));
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);

	label_frame2 = gtk_label_new (_("<b>Television Format</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label_frame2);
	gtk_label_set_use_markup (GTK_LABEL (label_frame2), TRUE);

	gtk_widget_show_all(dialog_vbox1);

	/* Set parameters in case the user has loaded a project file */
	if (img->slideshow_filename != NULL)
	{
		gtk_entry_set_text(GTK_ENTRY(slideshow_title_entry),img->slideshow_filename);
		gtk_combo_box_set_active(GTK_COMBO_BOX(slideshow_fmt_combo),img->slideshow_format_index);
		if (strcmp(img->aspect_ratio,"4:3") == 0)
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);
		else
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wide), TRUE);
		if (img->slideshow_height == 480)
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ntsc), TRUE);
		else
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);
	}

	response = gtk_dialog_run(GTK_DIALOG(dialog1));
	if (response == GTK_RESPONSE_OK || response == GTK_RESPONSE_ACCEPT)
	{
		img->slideshow_filename = g_strdup(gtk_entry_get_text(GTK_ENTRY(slideshow_title_entry)));
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (pal)))
		{
			gtk_widget_set_size_request(img->image_area,720,576);
			img->slideshow_height = 576;
		}
		else
		{
			gtk_widget_set_size_request(img->image_area,720,480);
			img->slideshow_height = 480;
		}
		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (tv)))
			img->aspect_ratio = "4:3";
		else
			img->aspect_ratio = "16:9";
		img->slideshow_format_index = gtk_combo_box_get_active(GTK_COMBO_BOX(slideshow_fmt_combo));
	}
	gtk_widget_destroy(dialog1);
}
