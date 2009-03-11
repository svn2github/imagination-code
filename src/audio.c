
#include <stdio.h>
#include <stdlib.h>

unsigned getbits(unsigned x, int p, int n);
int img_get_audio_length(char *filename)
{
	/* Bitrate values for MP3 files */
	const int bitrates[14][6] ={	{ 1, 32,  32 , 32 ,  32, 8  },
									{ 2, 64,  48 , 40 ,  48, 16 },
									{ 3, 96,  56 , 48 ,  56, 24 },
									{ 4, 128, 64 , 56 ,  64, 32 },
									{ 5, 160, 80 , 64 ,  80, 40 },
									{ 6, 192, 96 , 80 ,  96, 48 },
									{ 7, 224, 112, 96 , 112, 56 },
									{ 8, 256, 128, 112, 128, 64 },
									{ 9, 288, 160, 128, 144, 80 },
									{10, 320, 192, 160, 160, 96 },
									{11, 352, 224, 192, 176, 112},
									{12, 384, 256, 224,	192, 128},
									{13, 416, 320, 256, 224, 144},
									{14, 448, 384, 320, 256, 160}
									
								};
	int bitrate;

	unsigned char header[4];
	int i;

	FILE *f;

	f = fopen(filename,"r");
	if (f == NULL)
		printf ("NULL\n");
	if (fread (header, 1, 4, f) == 0)
		printf ("Error !\n");

	printf ("Version: ");
	for (i=4; i >= 3; i--)
		printf(((header[1] >> i) & 1) ? "1" : "0");

	printf (" %d\n", getbits(header[1], 4, 2));
	
	printf ("\nLayer: ");
	for (i=2; i >= 1; i--)
		printf(((header[1] >> i) & 1) ? "1" : "0");

	printf (" %d\n", getbits(header[1], 2, 2));
	
	printf ("\nBitrate: ");

	for (i=7; i >= 4; i--)
		printf(((header[2] >> i) & 1) ? "1" : "0");
	
	bitrate = getbits(header[2], 7, 4);
	printf(" %d\n",bitrate);
	printf ("\n%x\n",header[2]);
	//printf ("\n%x%x%x%x\n",header[0],header[1],header[2],header[3]);
	fclose(f);
	printf ("\n");

	printf("%d\n",bitrates[bitrate-1][1]);
	return 0;

}

unsigned getbits(unsigned x, int p, int n)
{
	return ( x >> (p+1-n)) & ~ (~0 << n);
}
