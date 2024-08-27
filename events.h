void show_error_dialog(char *error_message)
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

void on_entry_changed(GtkEntry *entry, gpointer user_data) 
{
	const char *text = gtk_entry_get_text(entry);
	int length = strlen(text);
	if (length > 5 || strspn(text, "0123456789") != length) 
	{
		gtk_entry_set_text(entry, length > 0 ? g_strndup(text, length - 1) : "");
	}
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


void toggled(GtkCheckMenuItem *menu_item,void *ptr, gpointer user_data) 
{
	gboolean a = gtk_check_menu_item_get_active(menu_item);
	if (wlr)
	{
		if (a)
		{
			gtk_widget_show(syncchk);
			gtk_widget_show(synclabel);
		}
		else
		{
			gtk_widget_hide(syncchk);
			gtk_widget_hide(synclabel);
		}
	}
	else
	{
		if (a)
		{
			gtk_widget_show(scacombo);
			gtk_widget_show(scalabel);
		}
		else
		{
			gtk_widget_hide(scacombo);
			gtk_widget_hide(scalabel);
		}
	}
	*(int *)ptr = a ? 1 : 0;
}

void restart_program(GtkWidget *widget, gpointer data)
{
	printf("Program Reloaded...\n");
	char *args[] = {pm, NULL};
	execvp(args[0], args);
}

void on_submenu_item5_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	//on_apply_button_clicked(NULL, &num_rows);

	GtkWidget *dialog5 = gtk_file_chooser_dialog_new(_("Save resolutions"), GTK_WINDOW(userdata), GTK_FILE_CHOOSER_ACTION_SAVE, _("Cancel"), GTK_RESPONSE_CANCEL, _("Save"), GTK_RESPONSE_ACCEPT, NULL);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog5));
	if (result == GTK_RESPONSE_ACCEPT) 
	{
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog5));

		FILE *file = fopen(filename, "w");
		if (file == NULL) 
		{
			g_print("ERROR: Can't open file\n");
			g_free(filename);
			gtk_widget_destroy(dialog5);
			return;
		}


		char* savedcmd = (char*) malloc(sizeof(char) * 100);
		int i, j = 0;

		for (i = 0; command[i] != '\0'; i++) 
			{
			if (command[i] != '*') 
				{
				savedcmd[j] = command[i];
				j++;
				}
			}
	savedcmd[j] = '\0';

		fputs("#!/bin/sh", file);
		fputs("\n", file);
		fputs(savedcmd, file);
		printf("File saved: %s", filename);

		char chmodCommand[ML];
		sprintf(chmodCommand, "chmod +x %s", filename);
		system(chmodCommand);
		fclose(file);
		g_free(filename);
	}
	gtk_widget_destroy(dialog5);
}



void delempty(char *str)
{
	int limit = strlen(str);
	for (int i = 0; i < limit; i++)
	{
		if (str[i] == ' ')
		{
			for (int j = i; j < limit - 1; j++)
			{
				str[j] = str[j + 1];
			}
			str[limit - 1] = '\0';
			limit--;
			i--;
		}
	}
}

int file_exists_in_path(const char *filename)
{
	char *path_env = getenv("PATH");
	if (!path_env) return 0;

	char *path = strdup(path_env);
	char *token = strtok(path, ":");
	while (token != NULL)
	{
		char full_path[1024];
		snprintf(full_path, sizeof(full_path), "%s/%s", token, filename);
		if (access(full_path, F_OK) == 0)
		{
			free(path);
			return 1;
		}
		token = strtok(NULL, ":");
	}
	free(path);
	return 0;
}

void show_success_dialog(GtkWindow *parent) 
{
	GtkWidget *dialog = gtk_message_dialog_new(parent,
		GTK_DIALOG_MODAL,
		GTK_MESSAGE_INFO,
		GTK_BUTTONS_OK,
		_("Commands executed, please close the program and DON\'T execute again the same command"));
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
void on_execute_button_clicked(GtkButton *button, gpointer user_data) 
{
	int result = system(xcmd0);
	if (result == 0)
	{
		result = system(xcmd1);
		if (result == 0) 
		{
			GtkWindow *parent_window = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button)));
			show_success_dialog(parent_window);
		}
	}
}

void defcombo (GtkComboBox *combo, const gchar *value)
{
	int default_index = -1;
	GtkTreeModel *model = gtk_combo_box_get_model(combo);
	int num_elements = gtk_tree_model_iter_n_children(model, NULL);
	
	for (int i = 0; i < num_elements; i++)
	{
		GtkTreeIter iter;
		gtk_tree_model_iter_nth_child(model, &iter, NULL, i);
		
		gchar *text;
		gtk_tree_model_get(model, &iter, 0, &text, -1);
		
		if (strcmp(value, text) == 0)
		{
			default_index = i;
			g_free(text);
			break;
		}
		g_free(text);
	}
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), default_index);
}