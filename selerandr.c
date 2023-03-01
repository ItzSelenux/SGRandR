#include <gtk/gtk.h>
#include "selerandr.h"
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);



    //Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SeleRandr");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    /// Widgets
    GtkWidget *reslabel = gtk_label_new("Resolution:");
    GtkWidget *rescombo = gtk_combo_box_text_new();

    
    GtkWidget *reflabel = gtk_label_new("Refresh Rate:");
    GtkWidget *refcombo = gtk_combo_box_text_new();
    
    GtkWidget *rotlabel = gtk_label_new("Rotation:");
    GtkWidget *rotcombo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "normal");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "left");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "right");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "inverted");
    
    GtkWidget *outlabel = gtk_label_new("Output:");
    GtkWidget *outcombo = gtk_combo_box_text_new();
    GtkWidget *offon = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "On");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "Off");

    GtkWidget *label2 = gtk_label_new("Slider:");
    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
    gtk_scale_set_draw_value(GTK_SCALE(slider), FALSE);
    
    GtkWidget *ctmbtn = gtk_button_new_with_label("Custom Resolution");
    GtkWidget *defbtn    = gtk_button_new_with_label("Default");
    GtkWidget *applybtn  = gtk_button_new_with_label("Apply");

    // Get the Values of comboboxes
    resolutions = get_resolutions();
    for (int i = 0; resolutions[i] != NULL; i++) 
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), resolutions[i]);
        free(resolutions[i]);
    }
    free(resolutions);
    
    rates = get_rates();
    for (int i = 0; rates[i] != NULL; i++) 
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
        free(rates[i]);
    }
    free(rates);
    
    outputs = get_outputs();
    for (int i = 0; outputs[i] != NULL; i++) 
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo), outputs[i]);
        free(outputs[i]);
    }
    free(outputs);
    
    // Disable OutCombo if it only have one option
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(outcombo));
    int num_rows = gtk_tree_model_iter_n_children(model, NULL);
    
    if (num_rows == 1) 
    {
        gtk_widget_set_sensitive(offon, FALSE);
        gtk_widget_set_sensitive(outcombo, FALSE);
    }

    //Set first option of comboboxes as default
    gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);

    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), reslabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rescombo, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), reflabel, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), refcombo, 1, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), rotlabel, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rotcombo, 1, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), outlabel, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), offon,    2, 3, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), ctmbtn,   0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), defbtn,   1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), applybtn, 2, 4, 1, 1);

    //command
    void chrs() 
    {
        const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
        const char *resolution = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rescombo));
        const char *refresh_rate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(refcombo));
        const char *rotation = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rotcombo));
    
    char *command = (char*) malloc(sizeof(char) * 100);
    sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s", output, resolution, refresh_rate, rotation);
    
    system(command);
    free(command);
    }
    
    //button void
    void on_apply_button_clicked(GtkButton *button, gpointer user_data) {
    chrs();
    }

    g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), NULL);

    // End
    gtk_widget_show_all(window);
    gtk_main();

return 0;
}
