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

#include "new_slideshow.h"

static void img_bg_color_changed( GtkColorButton *, img_window_struct *);
static void img_distort_toggled( GtkToggleButton *, img_window_struct *);

void img_new_slideshow_settings_dialog(img_window_struct *img, gboolean flag)
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
	GtkWidget *ex_vbox;
	GtkWidget *ex_hbox;
	GtkWidget *frame3;
	GtkWidget *label_frame3;
	GtkWidget *alignment_frame3;
	GtkWidget *distort_button;
	GtkWidget *bg_button;
	GtkWidget *bg_label;
	GdkColor   color;
	GtkWidget *alignment_frame2;
	GtkWidget *vbox_video_format, *vbox_aspect_ratio;
	GtkWidget *pal,*ntsc,*tv,*wide;
	GtkWidget *label1;
	gint       response;
	gchar     *string;

	/* Display propert title depending on the callback that is calling this function. */
	string = ( flag ? _("Project properties") : _("Create a new slideshow") );
	dialog1 = gtk_dialog_new_with_buttons( string,
										GTK_WINDOW(img->imagination_window),
										GTK_DIALOG_DESTROY_WITH_PARENT,
										GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);

	gtk_button_box_set_layout (GTK_BUTTON_BOX (GTK_DIALOG (dialog1)->action_area), GTK_BUTTONBOX_SPREAD);
	gtk_window_set_default_size(GTK_WINDOW(dialog1),520,-1);
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

	vbox_frame1 = gtk_vbox_new (FALSE, 5);
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

	hbox_slideshow_options = gtk_hbox_new(TRUE, 10);
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
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);

	ntsc = gtk_radio_button_new_with_mnemonic_from_widget (GTK_RADIO_BUTTON (pal), "NTSC 720 x 480");
	gtk_box_pack_start (GTK_BOX (vbox_video_format), ntsc, TRUE, TRUE, 0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);

	label_frame1 = gtk_label_new (_("<b>Video Format</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame1), label_frame1);
	gtk_label_set_use_markup (GTK_LABEL (label_frame1), TRUE);

	frame2 = gtk_frame_new (NULL);
	gtk_box_pack_start (GTK_BOX (hbox_slideshow_options), frame2, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_IN);

	label_frame2 = gtk_label_new (_("<b>Television Format</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame2), label_frame2);
	gtk_label_set_use_markup (GTK_LABEL (label_frame2), TRUE);

	alignment_frame2 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame2), alignment_frame2);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_frame2), 5, 5, 5, 5);

	vbox_aspect_ratio = gtk_vbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (alignment_frame2), vbox_aspect_ratio);

	tv = gtk_radio_button_new_with_mnemonic (NULL, _("Normal 4:3"));
	gtk_box_pack_start (GTK_BOX (vbox_aspect_ratio), tv, TRUE, TRUE, 0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);

	wide = gtk_radio_button_new_with_mnemonic_from_widget (GTK_RADIO_BUTTON (tv), _("Widescreen 16:9"));
	gtk_box_pack_start (GTK_BOX (vbox_aspect_ratio), wide, TRUE, TRUE, 0);
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);

	frame3 = gtk_frame_new( NULL );
	gtk_box_pack_start (GTK_BOX (vbox_frame1), frame3, TRUE, TRUE, 0);
	gtk_frame_set_shadow_type (GTK_FRAME (frame3), GTK_SHADOW_IN);

	alignment_frame3 = gtk_alignment_new (0.5, 0.5, 1, 1);
	gtk_container_add (GTK_CONTAINER (frame3), alignment_frame3);
	gtk_alignment_set_padding (GTK_ALIGNMENT (alignment_frame3), 5, 5, 5, 5);

	label_frame3 = gtk_label_new (_("<b>Advanced Settings</b>"));
	gtk_frame_set_label_widget (GTK_FRAME (frame3), label_frame3);
	gtk_label_set_use_markup (GTK_LABEL (label_frame3), TRUE);

	ex_vbox = gtk_vbox_new( FALSE, 5 );
	gtk_container_add( GTK_CONTAINER( alignment_frame3 ), ex_vbox );

	distort_button = gtk_check_button_new_with_label( _("Rescale images to fit desired aspect ratio") );
	gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( distort_button ), img->distort_images );
	g_signal_connect( G_OBJECT( distort_button ), "toggled", G_CALLBACK( img_distort_toggled ), img );
	gtk_box_pack_start( GTK_BOX( ex_vbox ), distort_button, FALSE, FALSE, 0 );

	ex_hbox = gtk_hbox_new( FALSE, 5 );
	gtk_box_pack_start( GTK_BOX( ex_vbox ), ex_hbox, FALSE, FALSE, 0 );

	bg_label = gtk_label_new( _("Select background color:") );
	gtk_box_pack_start( GTK_BOX( ex_hbox ), bg_label, FALSE, FALSE, 0 );

	color.red   = ( ( img->background_color >> 24 ) & 0xff ) / 0xff * 0xffff;
	color.green = ( ( img->background_color >> 16 ) & 0xff ) / 0xff * 0xffff;
	color.blue  = ( ( img->background_color >>  8 ) & 0xff ) / 0xff * 0xffff;
	bg_button = gtk_color_button_new_with_color( &color );
	g_signal_connect( G_OBJECT( bg_button ), "color-set", G_CALLBACK( img_bg_color_changed ), img );
	gtk_box_pack_start( GTK_BOX( ex_hbox ), bg_button, FALSE, FALSE, 0 );

	gtk_widget_show_all(dialog_vbox1);

	/* Set parameters if some project file is already loaded and user clicked
	 * selected Project properties menu entry. */
	if (img->slideshow_filename != NULL && flag)
	{
		gtk_entry_set_text(GTK_ENTRY(slideshow_title_entry),img->slideshow_filename);
		gtk_combo_box_set_active(GTK_COMBO_BOX(slideshow_fmt_combo),img->slideshow_format_index);
		if (strcmp(img->aspect_ratio,"4:3") == 0)
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (tv), TRUE);
		else
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (wide), TRUE);

		if (img->image_area->allocation.height == 480)
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (ntsc), TRUE);
		else
			gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (pal), TRUE);
	}

	response = gtk_dialog_run(GTK_DIALOG(dialog1));

	if (response == GTK_RESPONSE_ACCEPT)
	{
		img->slideshow_filename = g_strdup(gtk_entry_get_text(GTK_ENTRY(slideshow_title_entry)));

		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (pal)))
			gtk_widget_set_size_request(img->image_area,720,576);
		else
			gtk_widget_set_size_request(img->image_area,720,480);

		if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON (tv)))
			img->aspect_ratio = "4:3";
		else
			img->aspect_ratio = "16:9";
		img->slideshow_format_index = gtk_combo_box_get_active(GTK_COMBO_BOX(slideshow_fmt_combo));
	
		img_set_buttons_state(img, TRUE);

		string = g_path_get_basename(img->project_filename);
		img_set_window_title(img,string);
		g_free(string);

		img->project_is_modified = TRUE;
	}
	gtk_widget_destroy(dialog1);
}

static void img_bg_color_changed( GtkColorButton *button, img_window_struct *img )
{
	GdkColor color;
	gint     r, g, b;

	gtk_color_button_get_color( button, &color );
	r = ( color.red   / 0xffff * 0xff );
	g = ( color.green / 0xffff * 0xff );
	b = ( color.blue  / 0xffff * 0xff );
	img->background_color = r  << 24 | g << 16 | b <<  8 | 0xff;
	img->project_is_modified = TRUE;
}

static void img_distort_toggled( GtkToggleButton *button, img_window_struct *img )
{
	img->distort_images = gtk_toggle_button_get_active( button );
	img->project_is_modified = TRUE;
}
