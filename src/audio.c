/*
 *  Copyright (c) 2009 Giuseppe Torelli <colossus73@gmail.com>
 *  Copyright (C) 2000-2006 Cedric Tefft <cedric@phreaker.net>
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
 * This file is based in part on:

	* MP3Info 0.5 by Ricardo Cerqueira <rmc@rccn.net>
	* MP3Stat 0.9 by Ed Sweetman <safemode@voicenet.com> and 
			 Johannes Overmann <overmann@iname.com>
 */

#include "audio.h"

unsigned getbits(unsigned x, int, int );
static gint sameConstant(mp3header *, mp3header *);
static gint header_frequency(mp3header *);
static gint frame_length(mp3header *);
static gint header_bitrate(mp3header *);
static gint get_next_header(mp3info *);
static gint get_header(FILE *file,mp3header *);
static gint get_first_header(mp3info *, long );
static gint get_mp3_info(mp3info *, gchar *);

gint frequencies[3][4] = {
   {22050,24000,16000,50000},  /* MPEG 2.0 */
   {44100,48000,32000,50000},  /* MPEG 1.0 */
   {11025,12000,8000,50000}    /* MPEG 2.5 */
};

gint bitrate[2][3][15] = { 
  { /* MPEG 2.0 */
    {0,32,48,56,64,80,96,112,128,144,160,176,192,224,256},  /* layer 1 */
    {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160},       /* layer 2 */
    {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160}        /* layer 3 */

  },

  { /* MPEG 1.0 */
    {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448}, /* layer 1 */
    {0,32,48,56,64,80,96,112,128,160,192,224,256,320,384},    /* layer 2 */
    {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320}      /* layer 3 */
  }
};

	/*
		dialog = gtk_message_dialog_new(GTK_WINDOW(img->imagination_window),GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,_("Can't read file header!"));
		gtk_message_dialog_format_secondary_text (GTK_MESSAGE_DIALOG (dialog),"%s.",strerror(errno));
		gtk_window_set_title(GTK_WINDOW(dialog),"Imagination");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (GTK_WIDGET (dialog));
		return NULL;
	}*/

gint frame_size_index[] = {24000, 72000, 72000};

gchar *img_get_audio_length(img_window_struct *img, gchar *filename, gint *secs)
{
	FILE *fp;
	gint seconds;

	if (g_str_has_suffix(filename, ".mp3") || g_str_has_suffix(filename, ".MP3"))
	{
		mp3info *mp3;
		fp = fopen(filename,"rb");

		mp3 = g_new0(mp3info, 1);
		mp3->file = fp;
		get_mp3_info(mp3, filename);
		fclose(fp);
		g_free(mp3);
		seconds = mp3->seconds;
	}

	*secs = seconds;
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

static gint get_mp3_info(mp3info *mp3, gchar *filename)
{
  int had_error = 0;
  int frame_type[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float seconds = 0,total_rate = 0;
  int frames = 0,frame_types = 0,frames_so_far = 0;
  int vbr_median = -1;
  int bitrate;
  int counter = 0;
  mp3header header;
  struct stat filestat;
  off_t data_start = 0;
	
  stat(filename, &filestat);
  mp3->datasize=filestat.st_size;

  	if(get_first_header(mp3,0L)) {
		data_start=ftell(mp3->file);
		while((bitrate=get_next_header(mp3))) {
			frame_type[15-bitrate]++;
			frames++;
		}
		memcpy(&header,&(mp3->header),sizeof(mp3header));
		for(counter=0;counter<15;counter++) {
			if(frame_type[counter]) {
				frame_types++;
				header.bitrate=counter;
				frames_so_far += frame_type[counter];
				seconds += (float)(frame_length(&header)*frame_type[counter])/
				           (float)(header_bitrate(&header)*125);
				total_rate += (float)((header_bitrate(&header))*frame_type[counter]);
				if((vbr_median == -1) && (frames_so_far >= frames/2))
					vbr_median=counter;
			}
		}
		mp3->seconds=(int)(seconds+0.5);
		mp3->header.bitrate=vbr_median;
		mp3->vbr_average=total_rate/(float)frames;
		mp3->frames=frames;
		if(frame_types > 1) {
			mp3->vbr=1;
		}
	}
  return had_error;
}

static gint get_first_header(mp3info *mp3, long startpos) 
{
  int k, l=0,c;
  mp3header h, h2;
  long valid_start=0;
  
  fseek(mp3->file,startpos,SEEK_SET);
  while (1) {
     while((c=fgetc(mp3->file)) != 255 && (c != EOF));
     if(c == 255) {
        ungetc(c,mp3->file);
        valid_start=ftell(mp3->file);
        if((l=get_header(mp3->file,&h))) {
          fseek(mp3->file,l-FRAME_HEADER_SIZE,SEEK_CUR);
	  for(k=1; (k < MIN_CONSEC_GOOD_FRAMES) && (mp3->datasize-ftell(mp3->file) >= FRAME_HEADER_SIZE); k++) {
	    if(!(l=get_header(mp3->file,&h2))) break;
	    if(!sameConstant(&h,&h2)) break;
	    fseek(mp3->file,l-FRAME_HEADER_SIZE,SEEK_CUR);
	  }
	  if(k == MIN_CONSEC_GOOD_FRAMES) {
		fseek(mp3->file,valid_start,SEEK_SET);
		memcpy(&(mp3->header),&h2,sizeof(mp3header));
		mp3->header_isvalid=1;
		return 1;
	  } 
        }
     } else {
	return 0;
     }
   }

  return 0;  
}

static gint get_header(FILE *file,mp3header *header)
{
    unsigned char buffer[FRAME_HEADER_SIZE];
    int fl;

    if(fread(&buffer,FRAME_HEADER_SIZE,1,file)<1) {
	header->sync=0;
	return 0;
    }
    header->sync=(((int)buffer[0]<<4) | ((int)(buffer[1]&0xE0)>>4));
    if(buffer[1] & 0x10) header->version=(buffer[1] >> 3) & 1;
                    else header->version=2;
    header->layer=(buffer[1] >> 1) & 3;
    header->bitrate=(buffer[2] >> 4) & 0x0F;
    if((header->sync != 0xFFE) || (header->layer != 1) || (header->bitrate == 0xF)) {
	header->sync=0;
	return 0;
    }
    header->crc=buffer[1] & 1;
    header->freq=(buffer[2] >> 2) & 0x3;
    header->padding=(buffer[2] >>1) & 0x1;
    header->extension=(buffer[2]) & 0x1;
    header->mode=(buffer[3] >> 6) & 0x3;
    header->mode_extension=(buffer[3] >> 4) & 0x3;
    header->copyright=(buffer[3] >> 3) & 0x1;
    header->original=(buffer[3] >> 2) & 0x1;
    header->emphasis=(buffer[3]) & 0x3;

    /* Final sanity checks: bitrate 1111b and frequency 11b are reserved (invalid) */
    if (header->bitrate == 0x0F || header->freq == 0x3) {
	return 0;
    }
    
    return ((fl=frame_length(header)) >= MIN_FRAME_SIZE ? fl : 0); 
}

static gint get_next_header(mp3info *mp3) 
{
  int l=0,c,skip_bytes=0;
  mp3header h;
  
   while(1) {
     while((c=fgetc(mp3->file)) != 255 && (ftell(mp3->file) < mp3->datasize)) skip_bytes++;
     if(c == 255) {
        ungetc(c,mp3->file);
        if((l=get_header(mp3->file,&h))) {
	  if(skip_bytes) mp3->badframes++;
          fseek(mp3->file,l-FRAME_HEADER_SIZE,SEEK_CUR);
          return 15-h.bitrate;
	} else {
		skip_bytes += FRAME_HEADER_SIZE;
	}
     } else {
	  if(skip_bytes) mp3->badframes++;
      	  return 0;
     }
  }
}

static gint header_bitrate(mp3header *h)
{
	return bitrate[h->version & 1][3-h->layer][h->bitrate];
}

static gint frame_length(mp3header *header)
{
	return header->sync == 0xFFE ? 
		    (frame_size_index[3-header->layer]*((header->version&1)+1)*
		    header_bitrate(header)/header_frequency(header))+
		    header->padding : 1;
}

static gint header_frequency(mp3header *h)
{
	return frequencies[h->version][h->freq];
}

static gint sameConstant(mp3header *h1, mp3header *h2)
{
    if((*(uint*)h1) == (*(uint*)h2)) return 1;

    if((h1->version       == h2->version         ) &&
       (h1->layer         == h2->layer           ) &&
       (h1->crc           == h2->crc             ) &&
       (h1->freq          == h2->freq            ) &&
       (h1->mode          == h2->mode            ) &&
       (h1->copyright     == h2->copyright       ) &&
       (h1->original      == h2->original        ) &&
       (h1->emphasis      == h2->emphasis        )) 
		return 1;
    else return 0;
}
