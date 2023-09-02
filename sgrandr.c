#include "sgrandr.h"

 int main(int argc, char *argv[])
{
	if (argc > 0)
	{
		pm = argv[0];
	}
	else
	{
		pm = "sgramdr";
	}

	int testmode = 0;

for(int i = 1; i < argc; i++)
{
	if(strcmp(argv[i], "--nocsd") == 0)
	{
	nocsd = 1;
	printf("CSD Disabled, using fallback display \n");
	}
}

for(int i = 1; i < argc; i++)
{
	if(strcmp(argv[i], "--testmode") == 0)
	{
	  testmode = 1;
	}
}
	gtk_init(&argc, &argv);

if(testmode)
{
	printf("--testmode is eneable, displaying all options \n");
}


	//Main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Display Settings - SGRandR");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, 400, 300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

			theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
	if (info != NULL)
	{
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}
	//accelerator header
	accel_group = gtk_accel_group_new();
		gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// Create the header bar
	headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);

	button = gtk_menu_button_new();
	image = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
	wtitle = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Display Settings - SGRandR</b>");
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

	// Create the submenu
	submenu = gtk_menu_new();

	// Create the submenu items

	submenu_item1 = gtk_menu_item_new_with_label("Add a custom resolution");
	submenu_item2 = gtk_check_menu_item_new_with_label("Show \"Scaling mode \" option");
	submenu_item4 = gtk_menu_item_new_with_label("Reload Program");
	submenu_item5 = gtk_menu_item_new_with_label("Save Configuration");
	submenu_item3 = gtk_menu_item_new_with_label("About");


	// Add the submenu items to the submenu
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item2);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item4);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item5);
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
	rescombo = gtk_combo_box_text_new();

	refcombo = gtk_combo_box_text_new();
	rotcombo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "normal");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "left");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "right");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), "inverted");

	outcombo = gtk_combo_box_text_new();
	offon = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "On");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), "Off");

	pos = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Same as");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Left of");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Right of");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Above of");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), "Below of");
	outcombo2 = gtk_combo_box_text_new();

	slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 35, 200, 100);
	gtk_scale_set_draw_value(GTK_SCALE(slider), TRUE);
	gtk_widget_set_size_request(slider, 200, 50);
	gtk_range_set_value(GTK_RANGE(slider), 100);


	scacombo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Full");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Center");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "Aspect");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), "1:1");
	scalabel = gtk_label_new("Scaling Mode:");
	outlabel = gtk_label_new("Output:");
	poslabel = gtk_label_new("Position:");

	defbtn    = gtk_button_new_with_label("Default");
		gtk_widget_set_tooltip_text(defbtn, "Ctrl+D");
	applybtn  = gtk_button_new_with_label("Apply");

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
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(outcombo));
	int num_rows = gtk_tree_model_iter_n_children(model, NULL);
	if (!num_rows == 1 || testmode == 1)
	{
		gtk_grid_attach(GTK_GRID(grid), applybtn, 2, 8, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), defbtn,   1, 8, 1, 1);
	}



	//Set first option of comboboxes as default
	gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);

	//Items Grid position
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Resolution:"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rescombo, 1, 0, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Refresh Rate:"), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), refcombo, 1, 1, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Rotation:"), 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rotcombo, 1, 2, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Scale (%):"), 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), slider,   1, 4, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), poslabel, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), pos, 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo2, 2, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outlabel, 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), offon, 2, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), scalabel, 0, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), scacombo,   1, 7, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 8, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 8, 1, 1);


	g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), &num_rows);
	g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);
	g_signal_connect(rescombo, "changed", G_CALLBACK(on_rescombo_changed), refcombo);

		// Connect the submenu items to the callback function
	g_signal_connect(submenu_item1, "activate", G_CALLBACK(on_submenu_item1_selected), NULL);
	g_signal_connect(submenu_item4, "activate", G_CALLBACK(restart_program), pm);
	g_signal_connect(submenu_item5, "activate", G_CALLBACK(on_submenu_item5_selected), NULL);
	g_signal_connect(submenu_item2, "toggled", G_CALLBACK(on_submenu_item2_toggled), &sm);
	g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
	g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press), submenu);

	gtk_widget_add_accelerator(submenu_item1, "activate", accel_group, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(submenu_item5, "activate", accel_group, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(submenu_item4, "activate", accel_group, GDK_KEY_R, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_widget_add_accelerator(defbtn, "activate", accel_group, GDK_KEY_D, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(applybtn, "activate", accel_group, GDK_KEY_Return, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	// End
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		if (access("/usr/bin/sgrandr-cr", F_OK) == 0)
	{
		printf("sgrandr-cr detected \n");
	}
	else if
	(access("./sgrandr-cr", F_OK) == 0)
	{
		printf("sgrandr-cr detected \n");
	}
	else
	{
		printf("\033[1;33mWARNING\033[0m: sgrandr-cr not detected, hiding button.\n");
		gtk_widget_hide(submenu_item1);
	}

		gtk_widget_show_all(window);
	if (!num_rows == 1 || testmode == 1)
	{
		gtk_widget_show(pos);
		gtk_widget_show(offon);
		gtk_widget_show(poslabel);
		gtk_widget_show(outcombo);
		gtk_widget_show(outcombo2);
		gtk_widget_show(outlabel);
		gtk_widget_show(submenu_item1);
	}
	else
	{
	gtk_widget_hide(pos);
	gtk_widget_hide(offon);
	gtk_widget_hide(poslabel);
	gtk_widget_hide(outcombo);
	gtk_widget_hide(outcombo2);
	gtk_widget_hide(outlabel);
	}

	gtk_widget_hide(scacombo);
	gtk_widget_hide(scalabel);
	on_rescombo_changed(GTK_COMBO_BOX(rescombo), refcombo);
	gtk_main();

return 0;
}
