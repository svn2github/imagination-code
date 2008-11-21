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

#include "main-window.h"

void img_add_slides_thumbnails(GtkMenuItem *,img_window_struct *);
GSList *img_import_slides_file_chooser(img_window_struct *);
gboolean img_quit_application(GtkWidget *, GdkEvent *, img_window_struct *);
void img_thumb_view_select_slide(img_window_struct *, Img_Thumbnail_Selection_Mode );
void img_delete_selected_slides(GtkMenuItem *,img_window_struct *);
void img_show_about_dialog (GtkMenuItem *,img_window_struct *);

