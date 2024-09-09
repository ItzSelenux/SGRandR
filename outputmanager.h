
void getres()
{
	FILE *fp;
	char path[ML];
	int line = 0;

	if (wlr)
	{
		fp = popen("wlr-randr", "r");

		while (fgets(path, sizeof(path)-1, fp) != NULL)
		{
			if (strstr(path, "current") != NULL)
				line = 1;

			if (line)
			{
				char *px_position = strstr(path, "px");
				if (px_position != NULL)
				{
					strncpy(defres, px_position - 11, 12);
					defres[11] = '\0';
				}

				char *hz_position = strstr(path, "Hz");
				if (hz_position != NULL)
				{
					strncpy(defrate, hz_position - 11, 11);
					defrate[11] = '\0';
					break;
				}
			}
		}
		pclose(fp);
	}
	else
	{
		fp = popen("xrandr", "r");

		while (fgets(path, sizeof(path) - 1, fp) != NULL)
		{
			if (strstr(path, "*") != NULL)
			{
				line = 1;
			}

			if (line)
			{
				char *px_position = strstr(path, " ");
				if (px_position != NULL)
				{
					strncpy(defres, px_position + 1, 12);
					defres[11] = '\0';
				}

				char *hz_position = strstr(path, "*");
				if (hz_position != NULL)
				{
					strncpy(defrate, hz_position - 11, 11);
					defrate[11] = '*';
					break;
				}
			}
		}
		pclose(fp);
	}

	delempty(defres);
	delempty(defrate);
}

int get_sync()
{
	FILE *inputFile = popen("wlr-randr 2>&1", "r");

	char line[ML];
	int status = 0;

	while (fgets(line, sizeof(line), inputFile) != NULL)
	{
		char *syncStr = strstr(line, "Adaptive Sync:");
		if (syncStr != NULL)
		{
			char *enabledStr = strstr(syncStr, "enabled");
			if (enabledStr != NULL)
			{
				status = 1;
				break;
			}
		}
	}

	pclose(inputFile);
	return status;
}

char **get_rates(char *sres)
{
	char buffer[ML];
	char **rates = malloc(ML * sizeof(char *));
	int count = 0;

	if (rates == NULL)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		return NULL;
	}

	const char *command = (wlr == 0) ? "xrandr" : "wlr-randr";
	FILE *pipe = popen(command, "r");
	if (!pipe)
	{
		fprintf(stderr, "Error executing command.\n");
		free(rates);
		return NULL;
	}

	int ures = 1;

	while (fgets(buffer, ML, pipe))
	{
		if (strstr(buffer, sres))
		{
			char *rate = strtok(buffer, " ");
			while (rate != NULL && count < ML)
			{
				rate[strcspn(rate, "+\n")] = '\0';

				char *hz_ptr = strstr(rate, "Hz");
				if (hz_ptr != NULL)
					*hz_ptr = '\0';
				char *current_ptr = strstr(rate, "current");
				if (current_ptr != NULL)
					*current_ptr = '\0';
				char *preferred_ptr = strstr(rate, "preferred");
				if (preferred_ptr != NULL)
					*preferred_ptr = '\0';
				char *p1 = strstr(rate, "(");
				if (p1 != NULL)
					*p1 = '\0';

				if (!ures && strcmp(rate, "") != 0 && strchr(rate, 'x') == NULL)
				{
					rates[count] = malloc(strlen(rate) + 1);
					if (rates[count] == NULL)
					{
						fprintf(stderr, "Memory allocation failed.\n");
						pclose(pipe);

						for (int i = 0; i < count; i++)
							free(rates[i]);
						free(rates);
						return NULL;
					}
					strcpy(rates[count], rate);
					count++;
				}

				if (ures)
					ures = 0;

				rate = strtok(NULL, " ");
			}
		}
	}

	pclose(pipe);

	if (count < ML)
		rates[count] = NULL;
	else
	{
		fprintf(stderr, "Array index out of bounds.\n");

		for (int i = 0; i < count; i++)
			free(rates[i]);
		free(rates);
		return NULL;
	}

	return rates;
}

static void on_rescombo_changed(GtkComboBox *combo_box, gpointer user_data)
{
	GtkComboBoxText *combo_text = GTK_COMBO_BOX_TEXT(combo_box);
	const gchar *pres = gtk_combo_box_text_get_active_text(combo_text);

	char *sres = g_strdup_printf("%s ", pres);

	char **rates = get_rates(sres);
	if (rates == NULL)
	{
		g_free(sres);
		return;
	}

	g_print("Refresh Rates changed to display mode: %s\n", sres);

	GtkComboBox *refcombo = GTK_COMBO_BOX(user_data);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(refcombo));
	for (int i = 0; rates[i] != NULL; i++) 
	{
		printf("%s\n", rates[i]);
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
		free(rates[i]);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
	free(rates);
	g_free(sres);
}

void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
{
	setlocale(LC_NUMERIC, "C");
	const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pos));
	const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo)),
	*opwr = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(offon)),
	*output2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo2)),
	*resolution = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rescombo)),
	*refresh_rate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(refcombo)),
	*rotation = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rotcombo)),
	*scalingmode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(scacombo)),
	*cpos,*csca,*crot;
	int *num_rows_ptr = (int *)user_data;
	int sld = gtk_range_get_value(GTK_RANGE(slider)), ps;
	double scl = (double)(sld) / (double)(100);
	char slider[ML] = "", output_buffer[ML], stderr_buffer[ML], command_with_stderr[ML];
	char *wsync, *error_message = NULL, *ptr = strchr(slider, ',');

	gsync = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(syncchk));

	if (gsync == 1)
		wsync = "enabled";
	else
		wsync = "disabled";

	num_rows = *num_rows_ptr;

	ps = (g_strcmp0(opwr, _("On")) == 0) ? 0 : ((g_strcmp0(opwr, _("Off")) == 0) ? 1 : -1);


	if (active_text != NULL) 
	{
		if (g_strcmp0(active_text, _("Same as")) == 0) 
			cpos = "--same-as";
		else if (g_strcmp0(active_text, _("Left of")) == 0) 
			cpos = "--left-of";
		else if (g_strcmp0(active_text, _("Right of")) == 0) 
			cpos = "--right-of";
		else if (g_strcmp0(active_text, _("Above of")) == 0) 
			cpos = "--above";
		else if (g_strcmp0(active_text, _("Below of")) == 0) 
			cpos = "--below";
	}
	if (scalingmode != NULL)
	{
		if (g_strcmp0(scalingmode, _("Full")) == 0) 
			csca = "Full";
		else if (g_strcmp0(scalingmode, _("Center")) == 0) 
			csca = "Center";
		else if (g_strcmp0(scalingmode, _("Aspect")) == 0) 
			csca = "Aspect";
		else if (g_strcmp0(scalingmode, _("1:1")) == 0) 
			csca = "1:1";
	}
	if (rotation != NULL)
	{
		if (wlr == 0)
		{
			if (g_strcmp0(rotation, _("normal")) == 0) 
				crot = "normal";
			else if (g_strcmp0(rotation, _("left")) == 0) 
				crot = "left";
			else if (g_strcmp0(rotation, _("right")) == 0) 
				crot = "right";
			else if (g_strcmp0(rotation, _("inverted")) == 0) 
				crot = "inverted";
		}
		else
		{
			if (g_strcmp0(rotation, _("normal")) == 0) 
				crot = "normal";
			else if (g_strcmp0(rotation, _("left")) == 0) 
				crot = "90";
			else if (g_strcmp0(rotation, _("right")) == 0) 
				crot = "270";
			else if (g_strcmp0(rotation, _("inverted")) == 0) 
				crot = "180";
		}
	}

	snprintf(slider, sizeof(slider), "%.2f", scl);

	// Replace commas with periods
	while (ptr != NULL) 
	{
		*ptr = '.';
		ptr = strchr(ptr, ',');
	}

		command = (char*) malloc(sizeof(char) * 100);

	char *x,*xl, *O = "--output", *m = "--mode", *R, *r, *S = "--scale",
	*o = (ps) ? "--off" : "", *s = malloc(ML);;

	if (sm)
		sprintf(s, "--set \"scaling mode\" \"%s\"", csca);
	else if (am)
		sprintf(s, "--adaptive-sync %s", wsync);
	else
		s="";

	if (wlr)
		x="wlr-randr",
		r="--transform",
		R="@";
	else
		x="xrandr",
		r="--rotate",
		R=" --rate ";

	if (num_rows <= 1)
		xl="%s %s %s %s %s %s%s%s %s %s %s %s";
	else if (ps)
		xl="%s %s %s %s";
	else
		xl="%s %s %s %s %s %s%s%s %s %s %s %s %s %s";

	sprintf(command, xl, x,
		O, output, o, m, resolution, R, refresh_rate, r, crot, S, slider, s,
		cpos, output2);
	printf("%s\n", command);

	snprintf(command_with_stderr, sizeof(command_with_stderr), "%s 2>&1", command);

	FILE *cmd_output = popen(command_with_stderr, "r");
	if (cmd_output == NULL)
	{
		show_error_dialog("Error executing command");
		return;
	}

	size_t read_size;
	stderr_buffer[0] = '\0';
	
	while ((read_size = fread(output_buffer, 1, sizeof(output_buffer), cmd_output)) > 0)
	{
		output_buffer[read_size] = '\0';
		strncat(stderr_buffer, output_buffer, sizeof(stderr_buffer) - strlen(stderr_buffer) - 1);
	}

	int return_code = pclose(cmd_output);

	if (WIFEXITED(return_code))
	{
		int exit_status = WEXITSTATUS(return_code);
		if (exit_status != 0)
			error_message = stderr_buffer;
	}

	if (error_message)
		show_error_dialog(error_message);
}




char** get_resolutions()
{
	FILE* pipe;
	char* command;

	if (wlr == 0)
		command = "xrandr";
	else
		command = "wlr-randr";

	pipe = popen(command, "r");

	if (pipe == NULL)
	{
		fprintf(stderr, "Error opening pipe.\n");
		exit(EXIT_FAILURE);
	}

	char** resolutions = malloc(ML * sizeof(char*));
	char buffer[ML];
	int i = 0;

	while (fgets(buffer, ML, pipe) != NULL)
	{
		if (wlr == 0 && strstr(buffer, "  ") != NULL)
		{
			char* resolution = strtok(buffer, " ");
			resolutions[i] = strdup(resolution);
			i++;
		}
		else if (wlr == 1 && strstr(buffer, "px") != NULL)
		{
			char* resolution = strtok(buffer, " ");
			resolutions[i] = strdup(resolution);
			i++;
		}
	}
	resolutions[i] = NULL;
	pclose(pipe);

	if (wlr == 1)
	{
		char** unique_resolutions = malloc(ML * sizeof(char*));
		int j = 0;

		for (int k = 0; resolutions[k] != NULL; k++)
		{
			int duplicate = 0;

			for (int l = 0; l < j; l++)
			{
				if (strcmp(resolutions[k], unique_resolutions[l]) == 0)
				{
					duplicate = 1;
					break;
				}
			}
			if (!duplicate)
			{
				unique_resolutions[j] = strdup(resolutions[k]);
				j++;
			}
		}
		for (int k = 0; resolutions[k] != NULL; k++)
		{
			free(resolutions[k]);
		}
		free(resolutions);
		unique_resolutions[j] = NULL;

		return unique_resolutions;
	}
	return resolutions;
}


char** get_outputs() 
{
	char* command = "xrandr";
	char buffer[ML];
	FILE* fp = popen(command, "r");
	if (fp == NULL) 
	{
		fprintf(stderr, "Failed to execute command\n");
		exit(EXIT_FAILURE);
	}

	char** outputs = (char**)malloc(sizeof(char*));
	int num_outputs = 0;

	while (fgets(buffer, ML, fp) != NULL) 
	{
		if (strstr(buffer, " connected")) 
		{
			char* output_name = strtok(buffer, " ");
			outputs[num_outputs] = (char*)malloc(sizeof(char) * strlen(output_name));
			strcpy(outputs[num_outputs], output_name);
			num_outputs++;
			outputs = (char**)realloc(outputs, sizeof(char*) * (num_outputs + 1));
		}
	}
	outputs[num_outputs] = NULL;
	pclose(fp);

	return outputs;
}

void on_default_button_clicked(GtkButton *button, gpointer user_data) 
{
	const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));

	char *command = (char*) malloc(sizeof(char) * 100);
	if (wlr == 0)
	{
		sprintf(command, "xrandr --output %s --auto --scale 1 --rotate normal", output);
	}
	else
	{
		sprintf(command, "wlr-randr --output %s --preferred --scale 1 --transform normal", output);
	}
	printf("Reverted to default configuration\n");
	system(command);
	free(command);
	gtk_range_set_value(GTK_RANGE(slider), 100);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);
	getres();
	defcombo (GTK_COMBO_BOX(rescombo), defres);
	on_rescombo_changed(GTK_COMBO_BOX(rescombo), refcombo);
	defcombo (GTK_COMBO_BOX(refcombo), defrate);
}