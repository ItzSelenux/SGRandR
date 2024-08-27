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

void show_error_dialog(const char *error_message)
{
	GtkWidget *dialog;
	dialog = gtk_message_dialog_new(NULL,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_OK,
		"%s",
		error_message);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void on_apply_button_clicked(GtkButton *button, gpointer user_data)
{
	int i;
	int mode_id = 0;
	Rotation rotation = RR_Rotate_0; // Default to normal rotation

	// Retrieve user settings
	char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
	const char *opwr = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(offon));
	const char *preresolution = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rescombo));
	const char *prerefresh_rate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(refcombo));
	const char *rotation_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rotcombo));


	if (!output || !opwr || !preresolution || !prerefresh_rate || !rotation_text) {
		show_error_dialog("Error: One or more combo box selections are invalid.");
		return;
	}

	char refresh_rate[50];
	strcpy(refresh_rate, prerefresh_rate);
	char *hz_position = strstr(refresh_rate, "Hz");
	
	if (hz_position)
	{
		char *space_position = hz_position;
		while (space_position > refresh_rate && *(space_position - 1) == ' ')
		{
			space_position--;
		}
		*space_position = '\0';
	}
	double trate = atof(refresh_rate);

	int twidth, theight;
	sscanf(preresolution, "%dx%d", &twidth, &theight);

	g_print("%s%d%d%s",refresh_rate, twidth, theight, output);


	// Process slider value
	double scl = gtk_range_get_value(GTK_RANGE(slider)) / 100.0;

	// Set power state
	int ps = g_strcmp0(opwr, _("On")) == 0 ? 0 : 1;

	// Set rotation
	if (g_strcmp0(rotation_text, _("normal")) == 0)
	{
		rotation = RR_Rotate_0;
	}
	else if (g_strcmp0(rotation_text, _("left")) == 0)
	{
		rotation = RR_Rotate_270;
	}
	else if (g_strcmp0(rotation_text, _("right")) == 0)
	{
		rotation = RR_Rotate_90;
	}
	else if (g_strcmp0(rotation_text, _("inverted")) == 0)
	{
		rotation = RR_Rotate_180;
	}
	else
	{
		rotation = RR_Rotate_0;
	}

	setresolution(twidth, theight, trate, output, rotation);
}

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		GtkWidget *submenu = GTK_WIDGET(data);
		gtk_menu_popup_at_pointer(GTK_MENU(submenu), NULL);
		return TRUE;
	}

	return FALSE;
}

void on_default_button_clicked(GtkButton *button, gpointer user_data) 
{
	gtk_range_set_value(GTK_RANGE(slider), 100);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);
	on_apply_button_clicked(GTK_BUTTON(defbtn), NULL);
}