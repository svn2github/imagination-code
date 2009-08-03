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

#include <gtk/gtk.h>
#include "support.h"
#include "imagination.h"
#include "imgtablebutton.h"

enum
{
	IMG_MASK_ANIM       = 1 << 0,
	IMG_MASK_ANIM_DUR   = 1 << 1,
	IMG_MASK_POS        = 1 << 2,
	IMG_MASK_PLACING    = 1 << 3,
	IMG_MASK_FONT_DESC  = 1 << 4,
	IMG_MASK_FONT_COLOR = 1 << 5
};

#define IMG_MASK_ALL \
	IMG_MASK_ANIM | IMG_MASK_ANIM_DUR | IMG_MASK_POS | \
	IMG_MASK_PLACING | IMG_MASK_FONT_DESC | IMG_MASK_FONT_COLOR

img_window_struct *img_create_window(void);
void img_iconview_selection_changed (GtkIconView *, img_window_struct *);

void
img_ken_burns_update_sensitivity( img_window_struct *img,
								  gboolean           slide_selected,
								  gint               no_points );

void
img_subtitle_update_sensitivity( img_window_struct *img,
								 gint               mode );

void
img_update_sub_properties( img_window_struct    *img,
						   TextAnimationFunc     anim,
						   gint                  anim_id,
						   gint                  anim_duration,
						   ImgSubPos             position,
						   ImgRelPlacing         placing,
						   PangoFontDescription *desc,
						   gdouble              *color,
						   guint                 mask );
#endif
