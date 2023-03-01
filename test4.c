#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char **rates;
#define BUFFER_SIZE 1024

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

        while (fgets(buffer, BUFFER_SIZE, pipe)) {
            if (strstr(buffer, sres)) {
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
        if (count < BUFFER_SIZE) {  // check if count is less than BUFFER_SIZE before accessing rates array
            rates[count] = NULL;
        } else {
            fprintf(stderr, "Array index out of bounds.\n");
        }

        return rates;
    }

    char **rates = get_rates();

            g_print("Refresh Rates changed to display mode: %s\n", sres);
    rates = get_rates();
    for (int i = 0; rates[i] != NULL; i++) 
    {
        GtkComboBox *refcombo = GTK_COMBO_BOX(user_data);
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
        free(rates[i]);
    }
    free(rates);

    g_free(sres);

}




int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *rescombo;
    GtkWidget *refcombo;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "SeleRandr");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    rescombo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), "1440x900");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), "640x480");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), "1280x720");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), "720x400");

    gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);

    g_signal_connect(rescombo, "changed", G_CALLBACK(on_rescombo_changed), refcombo);

    refcombo = gtk_combo_box_text_new();

    gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);



    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Resolution:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), rescombo, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), refcombo, 1, 1, 1, 1);



    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
