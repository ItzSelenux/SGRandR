#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);


    //Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SeleRandr - Create a new resolution");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
        GtkIconTheme *theme = gtk_icon_theme_get_default();
    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
    if (info != NULL) {
        GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
        g_object_unref(info);
    }

    // Create grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    /// Widgets
    GtkWidget *width = gtk_entry_new();
    GtkWidget *height = gtk_entry_new();
    GtkWidget *rate = gtk_entry_new();

    GtkWidget *applybtn  = gtk_button_new_with_label("Generate");


    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Width:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("height:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height, 1, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rate, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), applybtn, 0, 3, 2, 1);

    // End
    gtk_widget_show_all(window);
    gtk_main();

return 0;
}
