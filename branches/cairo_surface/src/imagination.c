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

#include <gtk/gtk.h>
#include <sox.h>

#include "main-window.h"
#include "support.h"
#include "callbacks.h"

extern void output_message(unsigned , const char *, const char *, va_list ap);

int main (int argc, char *argv[])
{
	img_window_struct *img_window;

	#ifdef ENABLE_NLS
  		bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  		textdomain (GETTEXT_PACKAGE);
	#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);
	
	sox_globals.output_message_handler = output_message;
	sox_format_init();

	img_window = img_create_window();

	/* Load the transitions as plugins with GModule */
	img_load_available_transitions(img_window);

	/* Set some default values */
	img_window->background_color[0] = 0;
	img_window->background_color[1] = 0;
	img_window->background_color[2] = 0;
	img_window->slides_nr = 0;
	img_window->distort_images = TRUE;

	img_window->maxoffx = 0;
	img_window->maxoffy = 0;
	img_window->current_point.offx = 0;
	img_window->current_point.offy = 0;
	img_window->current_point.zoom = 1;

	img_window->image_area_zoom = 1.0;
	img_window->overview_zoom = 1.0;
	img_window->low_quality = TRUE;
	img_window->video_size[0] = 720;
	img_window->video_size[1] = 576;
	img_window->video_ratio = (gdouble)720 / 576;

	/* Last pseudo-slide has a duration of 0 */
	img_window->final_transition.duration = 0;
	img_window->final_transition.render = NULL;
	img_window->final_transition.speed = NORMAL;

	gtk_widget_show (img_window->imagination_window);
	img_set_statusbar_message(img_window,0);

	/*read the project filename passed in argv*/
 	if (argc > 1 )
	{
		img_load_slideshow( img_window, argv[1] );
	}

	gtk_main ();

	sox_format_quit();
	g_free(img_window);
	return 0;
}

