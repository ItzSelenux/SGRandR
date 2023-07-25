#include "sgrandr.h"


int main(int argc, char *argv[])
{

int nocsd = 0;

for(int i = 1; i < argc; i++) 
{
	if(strcmp(argv[i], "--nocsd") == 0) {
	nocsd = 1;
	printf("CSD Disabled, using fallback display \n");
	}
}
	gtk_init(&argc, &argv);
	//Main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Create a new resolution - SGRandR");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, 400, 300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
	if (info != NULL) {
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}

	 // Create the header bar
	headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);

	button = gtk_menu_button_new();
	image = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
	wtitle = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Create a new resolution - SGRandR</b>");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

	// Create the submenu
	submenu = gtk_menu_new();

	// Create the submenu items
	submenu_item3 = gtk_menu_item_new_with_label("About");

	// Add the submenu items to the submenu
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

	// Show all the submenu items
	gtk_widget_show_all(submenu);

	// Connect the button to the submenu
	gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);

	// Add the header bar to the main window
	if (nocsd == 0 )
	{
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	}

	// Create grid
	grid = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(window), grid);

	/// Widgets
	width = gtk_entry_new();
		gtk_entry_set_placeholder_text(GTK_ENTRY(width), "1920");
		gtk_entry_set_max_length(GTK_ENTRY(width), 5);
	height = gtk_entry_new();
		gtk_entry_set_placeholder_text(GTK_ENTRY(height), "1080");
		gtk_entry_set_max_length(GTK_ENTRY(height), 5);
	rate = gtk_entry_new();
		gtk_entry_set_placeholder_text(GTK_ENTRY(rate), "60");
		gtk_entry_set_max_length(GTK_ENTRY(rate), 3);
	outcombo = gtk_combo_box_text_new();
	
	applybtn  = gtk_button_new_with_label("Generate");

	// get outputs
	outputs = get_outputs();
	for (int i = 0; outputs[i] != NULL; i++) 
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo), outputs[i]);
		free(outputs[i]);
	}
	free(outputs);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0); 
	
	//Items Grid position
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Width:"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), width, 1, 0, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("height:"), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), height, 1, 1, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rate, 1, 2, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Output:"), 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 3, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), applybtn, 0, 4, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: adding a custom resolution may cause unexpected results, \n like black screen if is unsopported by the Display or errors on resolutions list, \n you can set default configuration with Ctrl+D"), 0, 5, 2, 1);

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
