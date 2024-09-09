void create_cr_window();
//init locale
int locale()
{
	setlocale(LC_ALL, "");
	bindtextdomain(GETTEXT_PACKAGE, localedir);
	textdomain(GETTEXT_PACKAGE);
	return 0;
}

void on_submenu_item1_selected(GtkMenuItem *menuitem, gpointer userdata)
{
	create_cr_window();
}

void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	GtkWidget *dialog = gtk_about_dialog_new();
	theme = gtk_icon_theme_get_default();
		info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGRandR");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 Simple GTK Desktop Environment");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), _("SGDE Display Configurator"));
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://sgde.github.io/sgrandr");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), _("Project WebSite"));
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"video-display");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void on_save_button_clicked(GtkButton *button, gpointer user_data) 
{
	const char *text0 = gtk_label_get_text(GTK_LABEL(global_label0)), *text3 = gtk_label_get_text(GTK_LABEL(global_label3));
	char content[ML], chmodCommand[ML];
	GtkWidget *dialog = gtk_file_chooser_dialog_new(_("Save resolutions"), GTK_WINDOW(user_data), GTK_FILE_CHOOSER_ACTION_SAVE, _("Cancel"), GTK_RESPONSE_CANCEL, _("Save"), GTK_RESPONSE_ACCEPT, NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_ACCEPT) 
	{
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

		FILE *file = fopen(filename, "w");
		if (file == NULL) 
		{
			g_print("ERROR: Can't open file\n");
			g_free(filename);
			gtk_widget_destroy(dialog);
			return;
		}

		fputs("#!/bin/sh", file);
		fputs("\n", file);

		sprintf(content, "%s &&\nsleep 2 &&\n%s", text0, text3);
		fputs(content, file);
		printf("File saved: %s", filename);

		sprintf(chmodCommand, "chmod +x %s", filename);
		system(chmodCommand);
		fclose(file);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}


int comboindex(GtkComboBox *combo, const gchar *target_text)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint index = 0;

	model = gtk_combo_box_get_model(combo);

	if (gtk_tree_model_get_iter_first(model, &iter))
	{
		do
		{
			gchar *text;
			gtk_tree_model_get(model, &iter, 0, &text, -1);
			if (g_strcmp0(text, target_text) == 0)
			{
				g_free(text);
				return index;
			}
			g_free(text);
			index++;
		}
		while (gtk_tree_model_iter_next(model, &iter));
	}
	return -1;
}

void create_window()
{
	//Main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	const gchar *title = "%s - SGRandR";
	const gchar *translatedTitle = _("Display Settings");
	gchar *formattedTitle = g_markup_printf_escaped(title, translatedTitle);
	gtk_window_set_title(GTK_WINDOW(window), formattedTitle);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_widget_set_size_request(window, 400, 300);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_free(formattedTitle);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

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
	const gchar *header = "<b>%s - SGRandR</b>";
	gchar *formattedHeader = g_markup_printf_escaped(header, translatedTitle);
	gtk_label_set_markup(GTK_LABEL(wtitle), formattedHeader);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

	// Create the submenu
	submenu = gtk_menu_new();

	// Create the submenu items
	submenu_item1 = gtk_menu_item_new_with_label(_("Add a custom resolution"));
	submenu_item2 = gtk_check_menu_item_new_with_label(_("Show \"Scaling mode \" option"));
	submenu_item6 = gtk_check_menu_item_new_with_label(_("Show \"Adaptative Sync \" option"));
	submenu_item4 = gtk_menu_item_new_with_label(_("Reload Program"));
	submenu_item5 = gtk_menu_item_new_with_label(_("Save Configuration"));
	submenu_item3 = gtk_menu_item_new_with_label(_("About"));


	// Add the submenu items to the submenu
	if (wlr == 0)
	{
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item2);
	}
	else if (wlr == 1)
	{
		gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item6);
	}
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item4);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item5);
	gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

	// Show all the submenu items
	gtk_widget_show_all(submenu);

	// Connect the button to the submenu
	gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);

	// Add the header bar to the main window
	if (nocsd == 0)
		gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

	// Create grid
	grid = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(window), grid);

	/// Widgets
	rescombo = gtk_combo_box_text_new();

	refcombo = gtk_combo_box_text_new();
	rotcombo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), _("normal"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), _("left"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), _("right"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rotcombo), _("inverted"));

	outcombo = gtk_combo_box_text_new();
	offon = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), _("On"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(offon), _("Off"));

	pos = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), _("Same as"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), _("Left of"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), _("Right of"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), _("Above of"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(pos), _("Below of"));
	outcombo2 = gtk_combo_box_text_new();

	slider = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 35, 200, 100);
	gtk_scale_set_draw_value(GTK_SCALE(slider), TRUE);
	gtk_widget_set_size_request(slider, 200, 50);
	gtk_range_set_value(GTK_RANGE(slider), 100);


	scacombo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), _("Full"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), _("Center"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), _("Aspect"));
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scacombo), _("1:1"));
	scalabel = gtk_label_new(_("Scaling Mode:"));
	outlabel = gtk_label_new(_("Output:"));
	poslabel = gtk_label_new(_("Position:"));
	synclabel = gtk_label_new(_("Adaptative Sync:"));
		syncchk = gtk_check_button_new();

	defbtn = gtk_button_new_with_label(_("Default"));
		gtk_widget_set_tooltip_text(defbtn, "Ctrl+D");
	applybtn = gtk_button_new_with_label(_("Apply"));

	// Get the Values of comboboxes
	resolutions = get_resolutions();
	for (int i = 0; resolutions[i] != NULL; i++)
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(rescombo), resolutions[i]);
		free(resolutions[i]);
	}
	free(resolutions);

	outputs = get_outputs();
	for (int i = 0; outputs[i] != NULL; i++)
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo), outputs[i]);
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(outcombo2), outputs[i]);
		free(outputs[i]);
	}
	// free(outputs);

	// Open Extra option is there is more than one option
	model = gtk_combo_box_get_model(GTK_COMBO_BOX(outcombo));
	num_rows = gtk_tree_model_iter_n_children(model, NULL);
	if (outputs[1] != 0 || testmode == 1)
	{
		gtk_grid_attach(GTK_GRID(grid), applybtn, 2, 8, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), defbtn,   1, 8, 1, 1);
	}

	getres();
	g_print("%s\n", defrate);
	defcombo (GTK_COMBO_BOX(rescombo), defres);
	on_rescombo_changed(GTK_COMBO_BOX(rescombo), refcombo);
	defcombo (GTK_COMBO_BOX(refcombo), defrate);

	//gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);
	
	int syncs = get_sync();
	if (syncs)
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(syncchk), TRUE);
	else
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(syncchk), FALSE);

	//Items Grid position
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Resolution:")), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rescombo, 1, 0, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Refresh Rate:")), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), refcombo, 1, 1, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Rotation:")), 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), rotcombo, 1, 2, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new(_("Scale (%):")), 0, 4, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), slider,   1, 4, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), synclabel, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), syncchk,   1, 5, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), poslabel, 0, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), pos, 1, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo2, 2, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outlabel, 0, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), outcombo, 1, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), offon, 2, 6, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), scalabel, 0, 8, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), scacombo,   1, 8, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 9, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 9, 1, 1);


	g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), &num_rows);
	g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);
	g_signal_connect(rescombo, "changed", G_CALLBACK(on_rescombo_changed), refcombo);

		// Connect the submenu items to the callback function
	g_signal_connect(submenu_item1, "activate", G_CALLBACK(on_submenu_item1_selected), NULL);
	g_signal_connect(submenu_item4, "activate", G_CALLBACK(restart_program), pm);
	g_signal_connect(submenu_item5, "activate", G_CALLBACK(on_submenu_item5_selected), NULL);
	g_signal_connect(submenu_item2, "toggled", G_CALLBACK(toggled), &sm);
	g_signal_connect(submenu_item6, "toggled", G_CALLBACK(toggled), &am);


	g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
	g_signal_connect(window, "button-press-event", G_CALLBACK(on_button_press), submenu);

	gtk_widget_add_accelerator(submenu_item1, "activate", accel_group, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(submenu_item5, "activate", accel_group, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(submenu_item4, "activate", accel_group, GDK_KEY_R, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_widget_add_accelerator(defbtn, "activate", accel_group, GDK_KEY_D, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(applybtn, "activate", accel_group, GDK_KEY_Return, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	// End


	gtk_widget_show_all(window);

	if (outputs[1] != 0 || testmode == 1)
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
	gtk_widget_hide(syncchk);
	gtk_widget_hide(synclabel);
	free(outputs);
	gtk_main();
}