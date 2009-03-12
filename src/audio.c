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

#include "audio.h"

unsigned getbits(unsigned x, int p, int n);
gchar *img_get_audio_length(gchar *filename, gint *secs)
{
	/* Bitrate values for MP3 files
									bits  V1 L1,  V1 L2,   V1 L3,  V2 L1,  V2  L2 & L3 */
	const gint bitrates[14][6] ={	{ 1, 	32,  	32 , 	32 ,  	32, 		8  },
									{ 2, 	64,  	48 , 	40 ,  	48, 		16 },
									{ 3, 	96,  	56 , 	48 ,  	56, 		24 },
									{ 4, 	128, 	64 , 	56 ,  	64, 		32 },
									{ 5, 	160, 	80 , 	64 ,  	80, 		40 },
									{ 6, 	192, 	96 , 	80 ,  	96, 		48 },
									{ 7, 	224, 	112, 	96 , 	112, 		56 },
									{ 8, 	256, 	128, 	112, 	128, 		64 },
									{ 9, 	288, 	160, 	128, 	144, 		80 },
									{10, 	320, 	192, 	160, 	160, 		96 },
									{11, 	352, 	224, 	192, 	176, 		112},
									{12, 	384, 	256, 	224,	192, 		128},
									{13, 	416, 	320, 	256, 	224, 		144},
									{14, 	448, 	384, 	320, 	256, 		160}
									
								};
	gint bitrate, version, layer, i, kb;
	guchar header[4];
	FILE *f;
	struct stat buf;

	f = fopen(filename,"r");
	if (f == NULL || fread (header, 1, 4, f) == 0)
	{
		fclose(f);
		return g_strdup(_("Error!"));
	}
	fclose(f);

	/*for (i=4; i >= 3; i--)
		g_print(((header[1] >> i) & 1) ? "1" : "0");*/

	version = getbits(header[1], 4, 2);
	//g_print ("\nVersion: %d",version);
	
	/*for (i=2; i >= 1; i--)
		g_print(((header[1] >> i) & 1) ? "1" : "0");*/

	layer = getbits(header[1], 2, 2);
	//g_print ("\nLayer: %d\n",layer);

	/*for (i=7; i >= 4; i--)
		g_print(((header[2] >> i) & 1) ? "1" : "0");*/

	bitrate = getbits(header[2], 7, 4);
	//g_print ("\nbitrate: %d\n",bitrate);

	if (version == 3 && layer == 3)
		i = 1;
	else if (version == 3 && layer == 2)
		i = 2;
	else if (version == 3 && layer == 1)
		i = 3;
	else if (version == 2 && layer == 3)
		i = 4;
	else if ( (version == 2 && layer == 2) || (version == 2 && layer == 1) )
		i = 5;

	kb = bitrates[bitrate-1][i];
	i = g_stat(filename, &buf);
	*secs = ((buf.st_size - 4) * 8) / (kb * 1000);

	return img_convert_seconds_to_time(*secs);
}

unsigned getbits(unsigned x, int p, int n)
{
	return ( x >> (p+1-n)) & ~ (~0 << n);
}

void img_play_stop_selected_file(GtkButton *button, img_window_struct *img)
{
	GError *error = NULL;
	gchar	*cmd_line, *path, *filename, *file, *message;
	gchar 	**argv;
	gboolean ret;
	GtkTreeIter iter;
	GtkWidget *tmp_image;

	if (img->play_child_pid)
	{
		kill (img->play_child_pid, SIGINT);
		img->play_child_pid = 0;
		
		tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_PLAY,GTK_ICON_SIZE_MENU);
		gtk_button_set_image(GTK_BUTTON(img->play_audio_button), tmp_image);
		gtk_widget_set_tooltip_text(img->play_audio_button, _("Play the selected file"));
		gtk_statusbar_pop(GTK_STATUSBAR(img->statusbar), img->context_id);
		return;
	}
	gtk_tree_model_get_iter_first(GTK_TREE_MODEL (img->music_file_liststore),&iter);
	gtk_tree_model_get(GTK_TREE_MODEL(img->music_file_liststore), &iter, 0, &path, 1, &filename, -1);

	file = g_build_filename(path,filename,NULL);
	g_free(path);
	g_free(filename);

	/* TODO get the correct file type */
	cmd_line = g_strconcat("play -t mp3 ", file, NULL);
	g_print ("%s\n",cmd_line);

	argv = g_strsplit(cmd_line," ", 0);
	g_free(cmd_line);
	ret = g_spawn_async_with_pipes( NULL, argv, NULL,
									G_SPAWN_SEARCH_PATH | G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_STDERR_TO_DEV_NULL,
									NULL, NULL, &img->play_child_pid, NULL, NULL, NULL, &error );
									
	message = g_strdup_printf(_("Playing %s..."),file);
	gtk_statusbar_push(GTK_STATUSBAR(img->statusbar), img->context_id, message);
	g_free(file);
	g_free(message);

	tmp_image = gtk_image_new_from_stock (GTK_STOCK_MEDIA_STOP,GTK_ICON_SIZE_MENU);
	gtk_button_set_image(GTK_BUTTON(img->play_audio_button), tmp_image);
	gtk_widget_set_tooltip_text(img->play_audio_button, _("Stop the playback"));
}
