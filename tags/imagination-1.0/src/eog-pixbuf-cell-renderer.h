/* Eye Of Gnome - Pixbuf Cellrenderer 
 *
 * Copyright (C) 2007 The GNOME Foundation
 *
 * Author: Lucas Rocha <lucasr@gnome.org>
 *
 * Based on gnome-control-center code (capplets/appearance/wp-cellrenderer.c) by: 
 *      - Denis Washington <denisw@svn.gnome.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __EOG_PIXBUF_CELL_RENDERER_H__
#define __EOG_PIXBUF_CELL_RENDERER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _EogPixbufCellRenderer EogPixbufCellRenderer;
typedef struct _EogPixbufCellRendererClass EogPixbufCellRendererClass;

struct _EogPixbufCellRenderer
{
	GtkCellRendererPixbuf parent;
};

struct _EogPixbufCellRendererClass
{
	GtkCellRendererPixbufClass parent;
};

GType            eog_pixbuf_cell_renderer_get_type (void) G_GNUC_CONST;

GtkCellRenderer *eog_pixbuf_cell_renderer_new (void);

G_END_DECLS

#endif /* __EOG_PIXBUF_CELL_RENDERER_H__ */
