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

#ifndef __IMAGINATION_AUDIO_H
#define __IMAGINATION_AUDIO_H

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <signal.h>
#include <errno.h>
#include "support.h"

#define MIN_CONSEC_GOOD_FRAMES 4
#define FRAME_HEADER_SIZE 4
#define MIN_FRAME_SIZE 21
#define NUM_SAMPLES 4

typedef struct {
	unsigned long	sync;
	unsigned int	version;
	unsigned int	layer;
	unsigned int	crc;
	unsigned int	bitrate;
	unsigned int	freq;
	unsigned int	padding;
	unsigned int	extension;
	unsigned int	mode;
	unsigned int	mode_extension;
	unsigned int	copyright;
	unsigned int	original;
	unsigned int	emphasis;
} mp3header;

typedef struct {
	char *filename;
	FILE *file;
	off_t datasize;
	int header_isvalid;
	mp3header header;
	int id3_isvalid;
	int vbr;
	float vbr_average;
	int seconds;
	int frames;
	int badframes;
} mp3info;

gchar *img_get_audio_length(img_window_struct *, gchar *, gint *);
void img_play_stop_selected_file(GtkButton *, img_window_struct *);

#endif
