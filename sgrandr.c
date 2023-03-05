#include <gtk/gtk.h>
#include "sgrandr.h"

  int sm = 0;
//Function to change rate when other resolution is selected
GtkWidget *dialog;
static void on_rescombo_changed(GtkComboBox *combo_box, gpointer user_data) {
    GtkComboBoxText *combo_text = GTK_COMBO_BOX_TEXT(combo_box);
       const gchar *pres = gtk_combo_box_text_get_active_text(combo_text);

    char *sres = g_strdup_printf("%s ", pres);  // allocate enough space for the copied string and the space character

    char **get_rates() {
        char buffer[BUFFER_SIZE];
        char **rates = malloc(BUFFER_SIZE * sizeof(char *));
        int count = 0;

        FILE *pipe = popen("xrandr", "r");
        if (!pipe) {
            fprintf(stderr, "Error executing command.\n");
            return NULL;
        }

        int ures = 1;

        while (fgets(buffer, BUFFER_SIZE, pipe)) 
        {
            if (strstr(buffer, sres)) 
            {
                char *rate = strtok(buffer, " ");
                while (rate != NULL && count < BUFFER_SIZE) {  // check if count is less than BUFFER_SIZE before accessing rates array
                    // remove "+" and newlines
                    rate[strcspn(rate, "+\n")] = 0;

                    if (!ures && strcmp(rate, "") != 0) {
                        rates[count] = malloc(strlen(rate) + 1);
                        strcpy(rates[count], rate);

                        count++;
                    }

                    if (ures) {
                        ures = 0;
                    }

                    rate = strtok(NULL, " ");
                }
            }
        }

        pclose(pipe);

        // Add NULL terminator to array
        if (count < BUFFER_SIZE) 
        {  // check if count is less than BUFFER_SIZE before accessing rates array
            rates[count] = NULL;
        } else {
            fprintf(stderr, "Array index out of bounds.\n");
        }

        return rates;
    }

    char **rates = get_rates();

            g_print("Refresh Rates changed to display mode: %s\n", sres);
            
    rates = get_rates();
    GtkComboBox *refcombo = GTK_COMBO_BOX(user_data);
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(refcombo));
    for (int i = 0; rates[i] != NULL; i++) 
    {
        printf("%s\n", rates[i]);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);

    }
    free(rates);

    g_free(sres);

}

 int main(int argc, char *argv[])
{
  int testmode = 0;


  for(int i = 1; i < argc; i++) {
    if(strcmp(argv[i], "--testmode") == 0) {
      testmode = 1;
    }
  }
    gtk_init(&argc, &argv);

  if(testmode) {
    printf("--testmode is eneable, displaying all options \n");
  }


    //Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Display Settings - SGRandR");
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
    
    
    // Create the header bar
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
    
    GtkWidget *wicon = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wicon);

    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Display Settings - SGRandR</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
    

    // Create the button with an icon
    GtkWidget *button = gtk_menu_button_new();
    GtkWidget *image = gtk_image_new_from_icon_name("open-menu-symbolic", GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), button);

    // Create the submenu
    GtkWidget *submenu = gtk_menu_new();

    // Create the submenu items
    GtkWidget *submenu_item1 = gtk_menu_item_new_with_label("Add a custom resolution");
    GtkWidget *submenu_item2 = gtk_check_menu_item_new_with_label("Show \"Scaling mode \" option");
    GtkWidget *submenu_item3 = gtk_menu_item_new_with_label("About");

    // Add the submenu items to the submenu
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item2);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

    // Show all the submenu items
    gtk_widget_show_all(submenu);

    // Connect the button to the submenu
    gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);

    // Add the header bar to the main window
    gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

    // Create grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    /// Widgets
    GtkWidget *rescombo = gtk_combo_box_text_new();

    GtkWidget *refcombo = gtk_combo_box_text_new();
    
    GtkWidget *rotcombo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "normal");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "left");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "right");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "inverted");
    
    GtkWidget *outcombo = gtk_combo_box_text_new();
    GtkWidget *offon = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "On");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "Off");
        
    GtkWidget *pos = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Same as");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Left of");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Right of");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Above of");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Below of");
    GtkWidget *outcombo2 = gtk_combo_box_text_new();

    GtkWidget *slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 35, 200, 100);
    gtk_scale_set_draw_value(GTK_SCALE(slider), TRUE);
    gtk_widget_set_size_request(slider, 200, 50);
    gtk_range_set_value(GTK_RANGE(slider), 100);
    
    
            GtkWidget *scacombo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Full");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Center");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Aspect");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "1:1");
            GtkWidget *scalabel = gtk_label_new("Scaling Mode:");
        
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
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo2), outputs[i]);
        free(outputs[i]);
    }
    free(outputs);
    
    // Open Extra option is there is more than one option
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(outcombo));
    int num_rows = gtk_tree_model_iter_n_children(model, NULL);
    if (!num_rows == 1 || testmode == 1) 
    {
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Position:"), 0, 5, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), pos, 1, 5, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), outcombo2, 2, 5, 1, 1);
         gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Output:"), 0, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), offon, 2, 3, 1, 1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
        gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);
        gtk_grid_attach(GTK_GRID(grid), applybtn, 2, 8, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), defbtn,   1, 8, 1, 1);
    }
    


    //Set first option of comboboxes as default
    gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
    gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);

    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Resolution:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rescombo, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), refcombo, 1, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Rotation:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rotcombo, 1, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Scale (%):"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), slider,   1, 4, 1, 1);
    
    
    gtk_grid_attach(GTK_GRID(grid), scalabel, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scacombo,   1, 7, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 8, 1, 1);
    


    //command
    void chrs() 
    {
        const char *cpos;
        int ps;
        const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
        const char *opwr = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(offon));
        const char *output2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo2));
        const char *resolution = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rescombo));
        const char *refresh_rate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(refcombo));
        const char *rotation = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rotcombo));
        const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pos));
        const char *scalingmode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(scacombo));
        
        if (opwr == "On")
        {
        ps = 0;
        }
        else if (opwr == "Off")
        {
        ps = 1;
        }
        

        
        if (active_text != NULL) 
        {
            if (g_strcmp0(active_text, "Same as") == 0) 
            {
                cpos = "--same-as";
            } else if (g_strcmp0(active_text, "Left of") == 0) 
            {
                cpos = "--left-of";
            } else if (g_strcmp0(active_text, "Right of") == 0) 
            {
                cpos = "--right-of";
            } else if (g_strcmp0(active_text, "Above of") == 0) 
            {
                cpos = "--above";
            } else if (g_strcmp0(active_text, "Below of") == 0) 
            {
                cpos = "--below";
            }
        }
        
        
int sld = gtk_range_get_value(GTK_RANGE(slider));
double scl = (double)(sld) / (double)(100);
char slider[16];
snprintf(slider, sizeof(slider), "%.2f", scl);

// Replace commas with periods
char *ptr = strchr(slider, ',');
while (ptr != NULL) 
{
    *ptr = '.';
    ptr = strchr(ptr, ',');
}
        
    char *command = (char*) malloc(sizeof(char) * 100);
    char **outputs = get_outputs();
        if (outputs != NULL) 
        {
    for (int i = 0; outputs[i] != NULL; i++) 
            {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo), outputs[i]);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo2), outputs[i]);
        char fback[strlen(outputs[i])+1];
        strcpy(fback, outputs[i]);
        
        printf(fback);
        if (ps == 1) 
        {
            sprintf(command, "xrandr --output %s --off ", output);
        }
        else if (num_rows > 1 && sm == 1) 
        {
            sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s %s %s --set \"scaling mode\" \" %s \" ", output, resolution, refresh_rate, rotation, slider, cpos, output2, scalingmode);
        }
        else if (sm == 1) 
        {
            sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s --set \"scaling mode\" \"%s\" ", fback, resolution, refresh_rate, rotation, slider, scalingmode);
        }
        else if (num_rows > 1) 
        {
            sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s %s %s %s", output, resolution, refresh_rate, rotation, slider, cpos, output2);
        }
        else if (num_rows == 1) 
        {
            sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s", fback, resolution, refresh_rate, rotation, slider);
        }



        
            }
    free(outputs);
        }
    printf(command, "\n");
    
    system(command);
    free(command);
    }
    
    //command
    void dfrs() 
    {
        const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
    
    char *command = (char*) malloc(sizeof(char) * 100);
    char **outputs = get_outputs();
    if (outputs != NULL) 
        {
    for (int i = 0; outputs[i] != NULL; i++) 
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo), outputs[i]);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo2), outputs[i]);
        char fback[strlen(outputs[i])+1]; // Allocate an array with enough space to hold the string
        strcpy(fback, outputs[i]);
    sprintf(command, "xrandr --output %s --auto --scale 1", fback);
    
    system(command);
    free(command);
    }
        }
    }
    
    //button void
    void on_default_button_clicked(GtkButton *button, gpointer user_data) 
    {
    dfrs();
    }
    
    void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
    {
    chrs();
    }
    
    void on_submenu_item1_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        system("./sgrandr-cr");
    }
    
    void on_submenu_item2_toggled(GtkCheckMenuItem *menu_item,void *ptr, gpointer user_data) 
    {
        
    if (gtk_check_menu_item_get_active(menu_item)) 
        {
                gtk_widget_show(scacombo);
                gtk_widget_show(scalabel);
                int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
                *int_ptr = 1;
        } 
    else 
        {
                gtk_widget_hide(scacombo);
                gtk_widget_hide(scalabel);
                int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
                *int_ptr = 0;
        }
    }
    
    void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGRandR");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2023 ItzSelenux for Simple GTK Desktop Environment");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "SGDE Display Configurator");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sgrandr");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_MIT_X11);
    gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"video-display");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    }
    
    g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), NULL);
    g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);
    g_signal_connect(rescombo, "changed", G_CALLBACK(on_rescombo_changed), refcombo);
        // Connect the submenu items to the callback function
    g_signal_connect(submenu_item1, "activate", G_CALLBACK(on_submenu_item1_selected), NULL);
    g_signal_connect(submenu_item2, "toggled", G_CALLBACK(on_submenu_item2_toggled), &sm);
    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);


    // End
    gtk_widget_show_all(window);
    gtk_widget_hide(scacombo);
    gtk_widget_hide(scalabel);
    gtk_main();

return 0;
}
