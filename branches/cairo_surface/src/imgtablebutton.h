#ifndef __IMG_TABLE_BUTTON_H__
#define __IMG_TABLE_BUTTON_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define IMG_TYPE_TABLE_BUTTON \
	( img_table_button_get_type() )
#define IMG_TABLE_BUTTON( obj ) \
	( G_TYPE_CHECK_INSTANCE_CAST( ( obj ), IMG_TYPE_TABLE_BUTTON, ImgTableButton ) )
#define IMG_TABLE_BUTTON_CLASS( klass ) \
	( G_TYPE_CHECK_CLASS_CAST( ( klass ), IMG_TYPE_TABLE_BUTTON_CLASS, ImgTableButtonClass ) )
#define IMG_IS_TABLE_BUTTON( obj ) \
	( G_TYPE_CHECK_INSTANCE_TYPE( ( obj ), IMG_TYPE_TABLE_BUTTON ) )
#define IMG_IS_TABLE_BUTTON_CLASS( klass ) \
	( G_TYPE_CHECK_CLASS_TYPE( ( klass ), IMG_TYPE_TABLE_BUTTON_CLASS ) )

typedef struct _ImgTableButton ImgTableButton;
typedef struct _ImgTableButtonClass ImgTableButtonClass;

struct _ImgTableButton
{
	GtkButton parent;
};

struct _ImgTableButtonClass
{
	GtkButtonClass parent_class;

	void (* active_item_changed)( ImgTableButton *button,
								  gint            item );
};


GType
img_table_button_get_type( void );

GtkWidget *
img_table_button_new( void );

void
img_table_button_set_pixbufs( ImgTableButton  *button,
							  gint             no_images,
							  GdkPixbuf      **pixbufs );

void
img_table_button_set_rows( ImgTableButton *button,
						   gint            rows );

void
img_table_button_set_columns( ImgTableButton *button,
							  gint            columns );

void
img_table_button_set_use_mnemonics( ImgTableButton *button,
									gboolean        use_mnemonics );

void
img_table_button_set_use_stock( ImgTableButton *button,
								gboolean        use_stock );

void
img_table_button_set_active_item( ImgTableButton *button,
								  gint            active );

gint
img_table_button_get_rows( ImgTableButton *button );

gint
img_table_button_get_columns( ImgTableButton *button );

gint
img_table_button_get_no_items( ImgTableButton *button );

GdkPixbuf **
img_table_button_get_pixbufs( ImgTableButton *button );

gchar **
img_table_button_get_labels( ImgTableButton *button );

gboolean
img_table_button_get_use_mnemonics( ImgTableButton *button );

gboolean
img_table_button_get_use_stock( ImgTableButton *button );

gint
img_table_button_get_active_item( ImgTableButton *button );

void
img_table_button_update_layout( ImgTableButton *button );

G_END_DECLS

#endif /* __IMG_TABLE_BUTTON_H__ */

