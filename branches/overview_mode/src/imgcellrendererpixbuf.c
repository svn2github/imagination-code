/*
** Copyright (C) 2009 Tadej Borovšak <tadeboro@gmail.com>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "imgcellrendererpixbuf.h"

#define IMG_PARAM_DEFAULTS \
	G_PARAM_STATIC_NICK | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB
#define IMG_PARAM_READWRITE \
	G_PARAM_WRITABLE | G_PARAM_READABLE | IMG_PARAM_DEFAULTS

#define HORIZ_F 0.3
#define VERT_F  0.5
#define BORDER  2

/* Properties */
enum
{
	P_0,
	P_HAS_TEXT,   /* Does selected slide contains text? */
	P_TEXT_ICO,   /* Icon describing text */
	P_TRANSITION  /* Transition pixbuf */
};

#define IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( obj ) \
	( G_TYPE_INSTANCE_GET_PRIVATE( ( obj ), \
								   IMG_TYPE_CELL_RENDERER_PIXBUF, \
								   ImgCellRendererPixbufPrivate ) )

typedef struct _ImgCellRendererPixbufPrivate ImgCellRendererPixbufPrivate;
struct _ImgCellRendererPixbufPrivate
{
	gboolean   has_text;
	GdkPixbuf *text_ico;
	GdkPixbuf *transition;
};

/* ****************************************************************************
 * Local declarations
 * ************************************************************************* */
static void
img_cell_renderer_pixbuf_set_property( GObject      *object,
									   guint         prop_id,
									   const GValue *value,
									   GParamSpec   *pspec );

static void
img_cell_renderer_pixbuf_get_property( GObject    *object,
									   guint       prop_id,
									   GValue     *value,
									   GParamSpec *pspec );

static void
img_cell_renderer_pixbuf_render( GtkCellRenderer      *cell,
								 GdkDrawable          *window,
								 GtkWidget            *widget,
								 GdkRectangle         *background_a,
								 GdkRectangle         *cell_a,
								 GdkRectangle         *expose_a,
								 GtkCellRendererState  state );

static void
img_cell_renderer_pixbuf_finalize( GObject *object );

/* ****************************************************************************
 * Initialization
 * ************************************************************************* */
G_DEFINE_TYPE( ImgCellRendererPixbuf,
			   img_cell_renderer_pixbuf,
			   GTK_TYPE_CELL_RENDERER_PIXBUF );

static void
img_cell_renderer_pixbuf_class_init( ImgCellRendererPixbufClass *klass )
{
	GParamSpec           *spec;
	GObjectClass         *gobject_class = G_OBJECT_CLASS( klass );
	GtkCellRendererClass *renderer_class = GTK_CELL_RENDERER_CLASS( klass );

	gobject_class->set_property = img_cell_renderer_pixbuf_set_property;
	gobject_class->get_property = img_cell_renderer_pixbuf_get_property;
	gobject_class->finalize = img_cell_renderer_pixbuf_finalize;

	renderer_class->render = img_cell_renderer_pixbuf_render;

	spec = g_param_spec_boolean( "has-text",
								 "Has text",
								 "Indicates whether text indicator should be "
								 "drawn or not.",
								 FALSE,
								 IMG_PARAM_READWRITE );
	g_object_class_install_property( gobject_class, P_HAS_TEXT, spec );

	spec = g_param_spec_object( "text-ico",
								"Text icon",
								"Icon for text indicator.",
								GDK_TYPE_PIXBUF,
								IMG_PARAM_READWRITE );
	g_object_class_install_property( gobject_class, P_TEXT_ICO, spec );

	spec = g_param_spec_object( "transition",
								"Transition",
								"Indicates what ind of transition is applied "
								"onto the slide.",
								GDK_TYPE_PIXBUF,
								IMG_PARAM_READWRITE );
	g_object_class_install_property( gobject_class, P_TRANSITION, spec );

	g_type_class_add_private( gobject_class,
							  sizeof( ImgCellRendererPixbufPrivate ) );
}

static void
img_cell_renderer_pixbuf_init( ImgCellRendererPixbuf *cell )
{
	ImgCellRendererPixbufPrivate *priv;

	priv = IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( cell );
	priv->has_text = FALSE;
	priv->text_ico = NULL;
	priv->transition = NULL;
}

/* ****************************************************************************
 * Local Declarations
 * ************************************************************************* */
static void
img_cell_renderer_pixbuf_set_property( GObject      *object,
									   guint         prop_id,
									   const GValue *value,
									   GParamSpec   *pspec )
{
	ImgCellRendererPixbufPrivate *priv;

	g_return_if_fail( IMG_IS_CELL_RENDERER_PIXBUF( object ) );
	priv = IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( object );

	switch( prop_id )
	{
		case P_HAS_TEXT:
			priv->has_text = g_value_get_boolean( value );
			break;

		case P_TEXT_ICO:
			if( priv->text_ico )
				g_object_unref( G_OBJECT( priv->text_ico ) );
			priv->text_ico = g_value_dup_object( value );
			break;

		case P_TRANSITION:
			if( priv->transition )
				g_object_unref( G_OBJECT( priv->transition ) );
			priv->transition = g_value_dup_object( value );
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
			break;
	}
}

static void
img_cell_renderer_pixbuf_get_property( GObject    *object,
									   guint       prop_id,
									   GValue     *value,
									   GParamSpec *pspec )
{
	ImgCellRendererPixbufPrivate *priv;

	g_return_if_fail( IMG_IS_CELL_RENDERER_PIXBUF( object ) );
	priv = IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( object );

	switch( prop_id )
	{
		case P_HAS_TEXT:
			g_value_set_boolean( value, priv->has_text );
			break;

		case P_TEXT_ICO:
			g_value_set_object( value, priv->text_ico );
			break;

		case P_TRANSITION:
			g_value_set_object( value, priv->transition );
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID( object, prop_id, pspec );
			break;
	}
}

static void
img_cell_renderer_pixbuf_render( GtkCellRenderer      *cell,
								 GdkDrawable          *window,
								 GtkWidget            *widget,
								 GdkRectangle         *background_a,
								 GdkRectangle         *cell_a,
								 GdkRectangle         *expose_a,
								 GtkCellRendererState  state )
{
	GtkCellRendererClass         *pix_class;
	ImgCellRendererPixbufPrivate *priv;

	/* Drawing context */
	cairo_t *cr;

	/* Image rectangle */
	GdkRectangle rect;

	priv = IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( cell );

	/* Draw image first */
	pix_class =
		GTK_CELL_RENDERER_CLASS( img_cell_renderer_pixbuf_parent_class );
	pix_class->render( cell, window, widget,
					   background_a, cell_a, expose_a, state );

	if( ( ! priv->has_text ) && ( ! priv->transition ) )
		return;

	/* Get image size */
	pix_class->get_size( cell, widget, cell_a, &rect.x,
						 &rect.y, &rect.width, &rect.height );
	rect.x += cell_a->x + cell->xpad;
	rect.y += cell_a->y + cell->ypad;
	rect.width  -= 2 * cell->xpad;
	rect.height -= 2 * cell->ypad;

	/* Draw indicators */
	cr = gdk_cairo_create( window );

	if( priv->has_text && priv->text_ico )
	{
		gint    w,
				h;
		gdouble wf,
				hf,
				cf;

		w = gdk_pixbuf_get_width( priv->text_ico );
		h = gdk_pixbuf_get_height( priv->text_ico );
		wf = (gdouble)w / ( rect.width - 2 * BORDER );
		hf = (gdouble)h / ( rect.height - 4 * BORDER  );
		cf = MIN( MIN( 1.0, HORIZ_F / wf ), MIN( 1.0, VERT_F / hf ) );

		cairo_save( cr );
		cairo_translate( cr, rect.x + rect.width - BORDER,
							 rect.y + BORDER );
		gdk_cairo_set_source_pixbuf( cr, priv->text_ico, -w, 0 );
		cairo_scale( cr, cf, cf );
		cairo_paint( cr );
		cairo_restore( cr );
	}

	if( priv->transition )
	{
		gint    w,
				h;
		gdouble wf,
				hf,
				cf;

		w = gdk_pixbuf_get_width( priv->transition );
		h = gdk_pixbuf_get_height( priv->transition );
		wf = (gdouble)w / ( rect.width - 2 * BORDER );
		hf = (gdouble)h / ( rect.height - 4 * BORDER );
		cf = MIN( MIN( 1.0, HORIZ_F / wf ), MIN( 1.0, VERT_F / hf ) );

		cairo_translate( cr, rect.x + rect.width - BORDER, 
							 rect.y + rect.height - BORDER );
		gdk_cairo_set_source_pixbuf( cr, priv->transition, - w, - h );
		cairo_scale( cr, cf, cf );
		cairo_paint( cr );
	}

	cairo_destroy( cr );
}

static void
img_cell_renderer_pixbuf_finalize( GObject *object )
{
	ImgCellRendererPixbufPrivate *priv;

	priv = IMG_CELL_RENDERER_PIXBUF_GET_PRIVATE( object );

	if( priv->text_ico )
		g_object_unref( priv->text_ico );
	if( priv->transition )
		g_object_unref( priv->transition );

	G_OBJECT_CLASS( img_cell_renderer_pixbuf_parent_class )->finalize( object );
}


/* ****************************************************************************
 * Public API
 * ************************************************************************* */
GtkCellRenderer *
img_cell_renderer_pixbuf_new( void )
{
	return( g_object_new( IMG_TYPE_CELL_RENDERER_PIXBUF, NULL ) );
}

