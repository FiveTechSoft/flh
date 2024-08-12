#include <hbapi.h>
#undef HB_DEPRECATED
#include <gtk/gtk.h>

int MsgInfo( char * szMsg );
gint ButtonPressEvent( GtkWidget * hWnd, GdkEventButton * event );
gint DeleteEvent( GtkWidget * hWnd, gpointer Data );
gint ConfigureEvent( GtkWidget * hWnd, GdkEventConfigure * event );
gboolean motion_notify_event( GtkWidget * hWnd, GdkEventMotion * event );
void SetWndMain( GtkWidget * hWnd );

HB_FUNC( CREATEWINDOW )
{
   GtkWidget * hWnd  = gtk_window_new( GTK_WINDOW_TOPLEVEL );
   GtkWidget * hVBox = gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 );
   GtkWidget * hFixed = gtk_fixed_new();

   SetWndMain( hWnd );

   gtk_container_add( GTK_CONTAINER ( hWnd ), hVBox );
   gtk_box_pack_start( GTK_BOX( hVBox ), hFixed, TRUE, TRUE, 0 );

   g_object_set_data( G_OBJECT( hWnd ), "vbox", ( gpointer ) hVBox );
   g_object_set_data( G_OBJECT( hWnd ), "fixed", ( gpointer ) hFixed );

   gtk_window_set_default_size( GTK_WINDOW( hWnd ), 100, 100 );

   g_signal_connect( hWnd, "delete-event",
                     G_CALLBACK( DeleteEvent ), NULL );

   g_signal_connect( hWnd, "button-press-event",
                     G_CALLBACK( ButtonPressEvent ), NULL );

   g_signal_connect( hWnd, "configure-event",
                     G_CALLBACK( ConfigureEvent ), NULL );

   // g_signal_connect( hWnd, "motion-notify-event",
   //                   G_CALLBACK( motion_notify_event ), NULL );

   gtk_widget_set_events( hWnd, GDK_CONFIGURE |
                                GDK_EXPOSURE_MASK |
                                GDK_LEAVE_NOTIFY_MASK |
                                GDK_BUTTON_PRESS_MASK |
                                GDK_POINTER_MOTION_MASK |
                                GDK_POINTER_MOTION_HINT_MASK );

   hb_retptr( hWnd );
}

HB_FUNC( WNDDESTROY )
{
   gtk_widget_destroy( ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( SETPARENT )
{
   gpointer fixed = g_object_get_data( G_OBJECT( hb_parptr( 2 ) ), "fixed" );

   if( fixed == NULL )
      fixed = ( gpointer ) hb_parptr( 2 );

   gtk_container_add( GTK_CONTAINER( fixed ), ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( SETPOPUPPARENT )
{
   gtk_container_add( GTK_CONTAINER( hb_parptr( 2 ) ),
                      ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( SETCOORS )
{
   gtk_window_move( GTK_WINDOW( hb_parptr( 1 ) ), hb_parnl( 3 ), hb_parnl( 2 ) );
}

HB_FUNC( SETFOCUS )
{
   gtk_widget_grab_focus( ( ( GtkWidget * ) hb_parptr( 1 ) ) );
}

HB_FUNC( GETFOCUS )
{
   GtkWidget * hWnd = NULL;
   
   if( gtk_window_list_toplevels() )
      hWnd = g_list_last( gtk_window_list_toplevels() )->data;

   hb_retptr( gtk_window_get_focus( GTK_WINDOW( hWnd ) ) );
}

HB_FUNC( SETPROP )
{
   g_object_set_data( G_OBJECT( hb_parptr( 1 ) ), hb_parc( 2 ), ( gpointer ) hb_parnl( 3 ) );
}

HB_FUNC( GETPROP )
{
   hb_retnl( ( HB_LONG ) g_object_get_data( G_OBJECT( hb_parptr( 1 ) ), hb_parc( 2 ) ) );
}

HB_FUNC( GETLEFT )
{
   GtkAllocation allocation;
   gtk_widget_get_allocation( GTK_WIDGET( hb_parptr( 1 ) ), &allocation );
   hb_retnl( allocation.x );
}

HB_FUNC( GETTOP )
{
   GtkAllocation allocation;
   gtk_widget_get_allocation( GTK_WIDGET( hb_parptr( 1 ) ), &allocation );
   hb_retnl( allocation.y );
}

HB_FUNC( GETWIDTH )
{
   hb_retnl( gtk_widget_get_allocated_width( ( GtkWidget * ) hb_parptr( 1 ) ) );
}

HB_FUNC( GETHEIGHT )
{
   hb_retnl( gtk_widget_get_allocated_height( ( GtkWidget * ) hb_parptr( 1 ) ) );
}

HB_FUNC( CTRLSETSIZE )
{
   GtkWidget *widget = (GtkWidget *)hb_parptr(1);
   gint width = hb_parni(2);
   gint height = hb_parni(3);

   gtk_widget_set_size_request(widget, width, height);

   // If the widget is a viewport, also set its size
   if (GTK_IS_VIEWPORT(widget)) {
       GtkWidget *child = gtk_bin_get_child(GTK_BIN(widget));
       if (child) {
           gtk_widget_set_size_request(child, width, height);
       }
   }

    // Use CSS to set the preferred size
    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gchar *css = g_strdup_printf("widget { min-width: %dpx; min-height: %dpx; }", width, height);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_free(css);
    g_object_unref(provider);
}

HB_FUNC( CTRLSETPOS ) 
{
    long margin_start = hb_parnl(3);
    long margin_top = hb_parnl(2);
    
    // Limitar los valores a G_MAXINT16
    margin_start = (margin_start > G_MAXINT16) ? G_MAXINT16 : margin_start;
    margin_top = (margin_top > G_MAXINT16) ? G_MAXINT16 : margin_top;
    
    gtk_widget_set_margin_start( (GtkWidget *) hb_parptr(1), (int)margin_start );
    gtk_widget_set_margin_top( (GtkWidget *) hb_parptr(1), (int)margin_top );
}

HB_FUNC( WNDSETSIZE )
{
   gtk_window_set_default_size( GTK_WINDOW( hb_parptr( 1 ) ), hb_parnl( 2 ), hb_parnl( 3 ) );
}

HB_FUNC( WNDSETPOS )
{
   gtk_window_move( GTK_WINDOW( hb_parptr( 1 ) ), hb_parnl( 3 ), hb_parnl( 2 ) );
}

HB_FUNC( WNDGETPOS )
{
   gint x, y;

   gtk_window_get_position( GTK_WINDOW( hb_parptr( 1 ) ), &x, &y );
   
   hb_reta( 2 );
   hb_storvnl( x, -1, 1 );
   hb_storvnl( y, -1, 2 );
}

HB_FUNC( WNDSETTEXT )
{
   gtk_window_set_title( GTK_WINDOW( hb_parptr( 1 ) ), hb_parc( 2 ) );
}

HB_FUNC( WNDGETTEXT )
{
   hb_retc( gtk_window_get_title( GTK_WINDOW( hb_parptr( 1 ) ) ) );
}

HB_FUNC( SHOWWINDOW )
{
   gtk_widget_show_all( ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( SHOWCONTROL )
{
   gtk_widget_show( ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( WNDENABLE )
{
   gtk_widget_set_sensitive( ( GtkWidget * ) hb_parptr( 1 ), hb_parl( 2 ) );
}

HB_FUNC( WNDHIDE )
{
   gtk_widget_hide( ( GtkWidget * ) hb_parptr( 1 ) );
}

HB_FUNC( WNDMAXIMIZE )
{
   gtk_window_maximize( GTK_WINDOW( hb_parptr( 1 ) ) );
}

HB_FUNC( WNDREFRESH )
{
   GtkWidget * hWnd  = ( GtkWidget * ) hb_parptr( 1 );
   GdkRectangle rect = { 0, 0,
                         gtk_widget_get_allocated_width( hWnd ),
                         gtk_widget_get_allocated_height( hWnd ) };

   gdk_window_invalidate_rect( gtk_widget_get_window( hWnd ), &rect, TRUE );
}

HB_FUNC( WNDCENTER )
{
   gtk_window_set_position( GTK_WINDOW( hb_parptr( 1 ) ), GTK_WIN_POS_CENTER );
}

HB_FUNC( WNDSETTOOLTIP )
{
   GtkWidget * hWnd = GTK_WIDGET( hb_parptr( 1 ) );
   const gchar * tooltip_text = hb_parc( 2 );

   gtk_widget_set_tooltip_text( hWnd, tooltip_text );
}

HB_FUNC( BRINGWINDOWTOTOP )
{
   GtkWidget * hWnd  = ( GtkWidget * ) hb_parptr( 1 );

   gtk_window_set_keep_above( GTK_WINDOW( hWnd ), hb_parl( 2 ) );
}

void set_widget_font_from_pango(GtkWidget *widget, PangoFontDescription *pango_font)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    GtkStyleContext *context = gtk_widget_get_style_context(widget);

    // Convertir PangoFontDescription a string
    char *font_string = pango_font_description_to_string(pango_font);

    // Crear el CSS
    char *css = g_strdup_printf("* { font: %s; }", font_string);
    
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    
    g_free(css);
    g_free(font_string);

    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

HB_FUNC( CTRLSETFONT )
{
    GtkWidget *hWnd = (GtkWidget *)hb_parptr(1);
    PangoFontDescription *pango_font = (PangoFontDescription *)hb_parptr(2);
    set_widget_font_from_pango(hWnd, pango_font);
}