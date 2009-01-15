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

#ifndef __IMAGINATION_INTERFACE_H__
#define __IMAGINATION_INTERFACE_H__

#include <stdlib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include "support.h"
#include "imagination.h"
#include "eog-pixbuf-cell-renderer.h"

#define	FAST	10
#define	NORMAL	3
#define	SLOW	1

/*
typedef enum 
{
	IMG_GOTO_SLIDE = 0,
	IMG_MOVE_LEFT,
	IMG_MOVE_RIGHT
} Img_Thumbnail_Selection_Mode;
*/

void img_set_window_title(img_window_struct *,gchar *);
img_window_struct *img_create_window(void);
#endif
