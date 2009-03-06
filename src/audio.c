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

int img_get_audio_length(char *argc, char **argv)
{
	/* Bitrate hex values for MP3 files */
	const gchar *bitrate[5][2] ={
									{ "0x10", "32"  },
									{ "0x20", "40"  },
									{ "0x30", "48"  },
									{ "0x40", "56"  },
									{ "0x50", "64"  },
									{ "0x90", "128" },
									{ "0xA0", "160" },
									{ "0xB0", "192" },
									{ "0xD0", "256" }
								};

	unsigned char header[4];
	int i;

	FILE *f;

	f = fopen(argv[1],"r");
	if (f == NULL)
		return 1;
	if (fread (header, 1, 4, f) == 0)
		printf ("Error !\n");

	/* Version and Layer */
	printf ("Version: ");
	for (i=4; i >= 3; i--)
	{
		printf(((header[1] >> i) & 1) ? "1" : "0");
	}
	printf ("\nLayer: ");
	for (i=2; i >= 1; i--)
	{
		printf(((header[1] >> i) & 1) ? "1" : "0");
	}
	printf ("\nBitrate: ");
	/* Bitrate */
	for (i=7; i >= 4; i--)
	{
		printf(((header[2] >> i) & 1) ? "1" : "0");
	}

	fclose(f);
	printf ("\n");
	return 0;
}
