#include <gtk/gtk.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

    void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        GtkWidget *dialog;
        dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGRandR-CR");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "SGDE Custom Resolution Generator");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2023 ItzSelenux for Simple GTK Desktop Environment");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sgrandr");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_MIT_X11);
    gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"video-display");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    }
void on_entry_changed(GtkEntry *entry, gpointer user_data) 
{
    const char *text = gtk_entry_get_text(entry);
    int length = strlen(text);

    // Check if the entered text is a valid number and has a length of no more than 5 characters
    int valid = 1;
    for (int i = 0; i < length; i++) 
    {
        if (!isdigit(text[i])) 
        {
            valid = 0;
            break;
        }
    }
    if (length > 5 || !valid) 
    {
        // Delete the last character or set the text to an empty string
        gtk_entry_set_text(entry, length > 0 ? g_strndup(text, length - 1) : "");
    }
        

}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);


    //Main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Create a new resolution - SGRandR");
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
    
   GtkWidget *button = gtk_menu_button_new();
    GtkWidget *image = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Create a new resolution - SGRandR</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

    // Create the submenu
    GtkWidget *submenu = gtk_menu_new();

    // Create the submenu items
    GtkWidget *submenu_item3 = gtk_menu_item_new_with_label("About");

    // Add the submenu items to the submenu
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
    GtkWidget *width = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(width), "1920");
        gtk_entry_set_max_length(GTK_ENTRY(width), 5);
    GtkWidget *height = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(height), "1080");
        gtk_entry_set_max_length(GTK_ENTRY(height), 5);
    GtkWidget *rate = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(rate), "60");
        gtk_entry_set_max_length(GTK_ENTRY(rate), 3);
    
    GtkWidget *applybtn  = gtk_button_new_with_label("Generate");


    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Width:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), width, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("height:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), height, 1, 1, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rate, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), applybtn, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: adding a custom resolution may cause unexpected results, \n like black screen if is unsopported by the Display or errors on resolutions list"), 0, 4, 2, 1);
    
    
    void on_applybtn_clicked(GtkButton *button, gpointer user_data) 
    {
    const char *srate = gtk_entry_get_text(GTK_ENTRY(rate));
    const char *swidth = gtk_entry_get_text(GTK_ENTRY(width));
    const char *sheight = gtk_entry_get_text(GTK_ENTRY(height));

    // Set default values if any of the variables are empty or NULL
    if (!srate || strcmp(srate, "") == 0) {
        srate = "60";
    }
    if (!swidth || strcmp(swidth, "") == 0) {
        swidth = "1920";
    }
    if (!sheight || strcmp(sheight, "") == 0) {
        sheight = "1080";
    }
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "SGRandR - Commands for custom resolution");
                GtkIconTheme *theme = gtk_icon_theme_get_default();
    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
    
    
     // Create the header bar
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
    
    GtkWidget *wicon = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wicon);

    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b> Commands for custom resolution - SGRandR</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
    gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
    
    if (info != NULL) {
        GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
        g_object_unref(info);
    }


        // Create a label 
        GtkWidget *label0 = gtk_label_new("Copy this commands to a Terminal Window and then, reopen SeleRandr, <DISPLAY> refers to the output you want to use, for example HDMI1 \n");
        GtkWidget *label1 = gtk_label_new("");
        GtkWidget *label2 = gtk_label_new("");
        GtkWidget *label3 = gtk_label_new("Saving the command in a text file is a good idea");

            // Long way to get data
            char command[256];
            sprintf(command, "cvt %s %s %s", swidth, sheight, srate);
            FILE *fp = popen(command, "r");
            if (fp == NULL) 
            {
            g_print("Failed to run command\n");
            return;
            }
            char output[2048];
            fgets(output, sizeof(output), fp);
            fgets(output, sizeof(output), fp); // read second line of output
            pclose(fp);

            // Remove "Modeline" from the output
            char *modeline_pos = strstr(output, "Modeline");
            if (modeline_pos != NULL) 
            {

            char new_output[1024];
            strcpy(new_output, modeline_pos + strlen("Modeline"));
            strcpy(output, new_output);
            }
            char xcmd0[100];
            char xcmd1[100];
            sprintf(xcmd0, "xrandr --newmode %s", output);
            sprintf(xcmd1, "xrandr --addmode <DISPLAY> %sx%s_%s.00 \n", swidth, sheight, srate);
            // Set the label text with the command output
            gtk_label_set_text(GTK_LABEL(label1), xcmd0);
            gtk_label_set_text(GTK_LABEL(label2), xcmd1);

        // Create three buttons
        GtkWidget *button1 = gtk_button_new_with_label("OK");

        gtk_label_set_selectable(GTK_LABEL(label1), TRUE);
        gtk_label_set_selectable(GTK_LABEL(label2), TRUE);
        // Create a vertical box layout to hold the label and buttons
        GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label0, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), button1, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox), label3, FALSE, FALSE, 0);
        
        void on_button1_clicked(GtkButton *button, gpointer user_data)
        {
            system(xcmd0);
            system(xcmd1);
        }
        
        g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), window);

        // Add the layout to the window and show everything
        gtk_container_add(GTK_CONTAINER(window), vbox);
        gtk_widget_show_all(window);
    }
    
    
    g_signal_connect(width, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(height, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(rate, "changed", G_CALLBACK(on_entry_changed), NULL);
    g_signal_connect(applybtn, "clicked", G_CALLBACK(on_applybtn_clicked), window);
    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);

    // End
    gtk_widget_show_all(window);
    gtk_main();

return 0;
}
