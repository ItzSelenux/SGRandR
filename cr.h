void on_ok_button_clicked(GtkButton *button, gpointer user_data) 
{
	GtkWidget *window = GTK_WIDGET(user_data);
	gtk_widget_destroy(window);
}

void on_applybtn_clicked(GtkButton *button, gpointer user_data) 
{
	const char *srate = gtk_entry_get_text(GTK_ENTRY(rate)),
	*swidth = gtk_entry_get_text(GTK_ENTRY(width)),
	*sheight = gtk_entry_get_text(GTK_ENTRY(height));

	// Set default values if any of the variables are empty or NULL
	if (!srate || strcmp(srate, "") == 0) 
	{
		srate = "60";
	}
	if (!swidth || strcmp(swidth, "") == 0) 
	{
		swidth = "1920";
	}
	if (!sheight || strcmp(sheight, "") == 0) 
	{
		sheight = "1080";
	}

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL),
	*label0 = gtk_label_new(_("Copy this commands into a Terminal Window\n")),
	*label1 = gtk_label_new(""),
	*label2 = gtk_label_new("");
	const gchar *markupTitle = "SGRandR - %s",
	*translatedTitle = _("Commands for custom resolution");
	gchar *formattedMarkupTitle = g_markup_printf_escaped(markupTitle, translatedTitle);
	gtk_window_set_title(GTK_WINDOW(window), formattedMarkupTitle);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

	GtkIconTheme *theme = gtk_icon_theme_get_default();
	GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
	if (info != NULL)
	{
		GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}

	const char *display = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));

	// Long way to get data
	char command[ML], output[ML];
	sprintf(command, "cvt %s %s %s", swidth, sheight, srate);
	FILE *fp = popen(command, "r");
	if (fp == NULL) 
	{
		g_print("Failed to run command\n");
		return;
	}
	fgets(output, sizeof(output), fp);
	fgets(output, sizeof(output), fp); // read second line of output
	pclose(fp);

	// Remove "Modeline" from the output
	char *modeline_pos = strstr(output, "Modeline");
	if (modeline_pos != NULL) 
	{
		char new_output[ML];
		strcpy(new_output, modeline_pos + strlen("Modeline"));
		strcpy(output, new_output);
	}

	sprintf(xcmd0, "xrandr --newmode %s", output);
	sprintf(xcmd1, "xrandr --addmode %s %sx%s_%s.00 \n", display, swidth, sheight, srate);
	xcmd0[strcspn(xcmd0, "\n")] = '\0';

	// Set the label text with the command output
	gtk_label_set_text(GTK_LABEL(label1), xcmd0);
	gtk_label_set_text(GTK_LABEL(label2), xcmd1);
	gtk_label_set_selectable(GTK_LABEL(label1), TRUE);
	gtk_label_set_selectable(GTK_LABEL(label2), TRUE);

	// Create a vertical box layout to hold the label and buttons
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0),
	*save_button = gtk_button_new_with_label(_("Save into a file")),
	*execute_button = gtk_button_new_with_label(_("Execute commands")),
	*ok_button = gtk_button_new_with_label(_("OK"));

	gtk_box_pack_start(GTK_BOX(vbox), label0, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), execute_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), ok_button, FALSE, FALSE, 0);

	g_signal_connect(G_OBJECT(ok_button), "clicked", G_CALLBACK(on_ok_button_clicked), window);
	g_signal_connect(G_OBJECT(execute_button), "clicked", G_CALLBACK(on_execute_button_clicked), NULL);
	g_signal_connect(G_OBJECT(save_button), "clicked", G_CALLBACK(on_save_button_clicked), NULL);

	//fix save into a file bug
	global_label0 = label1;
	global_label3 = label2;
	// Add the layout to the window and show everything
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

void create_cr_window()
{
	//Main window
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//const gchar *title = "%s - SGRandR";
	const gchar *translatedTitle = _("Create a new resolution");
	//gchar *formattedTitle = g_markup_printf_escaped(title, translatedTitle);
	gtk_window_set_title(GTK_WINDOW(dialog), translatedTitle);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_widget_set_size_request(dialog, 400, 300);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

	g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
	if (info != NULL)
	{
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(dialog), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}
	image = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);

	// Create grid
	grid = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(dialog), grid);

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

	applybtn  = gtk_button_new_with_label(_("Generate"));

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
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Width:")), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), width, 1, 0, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Height:")), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), height, 1, 1, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Refresh Rate:")), 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rate, 1, 2, 1, 1);
	
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Output:")), 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 3, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), applybtn, 0, 4, 2, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("WARNING: adding a custom resolution may cause unexpected results, \n like black screen if is unsopported by the Display or errors on resolutions list, \n you can set default configuration with Ctrl+D")), 0, 5, 2, 1);

	g_signal_connect(width, "changed", G_CALLBACK(on_entry_changed), NULL);
	g_signal_connect(height, "changed", G_CALLBACK(on_entry_changed), NULL);
	g_signal_connect(rate, "changed", G_CALLBACK(on_entry_changed), NULL);
	g_signal_connect(applybtn, "clicked", G_CALLBACK(on_applybtn_clicked), window);
	g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);

	gtk_widget_show_all(dialog);
	gtk_main();
}