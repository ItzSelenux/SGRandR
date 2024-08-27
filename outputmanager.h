void setresolution(int screenx, int screeny, double rrate, char *target, Rotation rotation)
{
	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "Error: Can't open display\n");
		return;
	}

	Window root = DefaultRootWindow(display);
	XRRScreenResources *res = XRRGetScreenResources(display, root);
	if (res == NULL)
	{
		fprintf(stderr, "Error: can't get screen resources\n");
		XCloseDisplay(display);
		return;
	}

	RROutput output = None;
	XRROutputInfo *output_info = NULL;
	for (int i = 0; i < res->noutput; i++)
	{
		output_info = XRRGetOutputInfo(display, res, res->outputs[i]);
		if (output_info != NULL && strcmp(output_info->name, target) == 0)
		{
			output = res->outputs[i];
			break;
		}
		XRRFreeOutputInfo(output_info);
	}

	if (output == None)
	{
		fprintf(stderr, "Error: Output %s is not available.\n", target);
		XRRFreeScreenResources(res);
		XCloseDisplay(display);
		return;
	}

	if (output_info == NULL)
	{
		fprintf(stderr, "Error: can't get output information for %s.\n", target);
		XRRFreeScreenResources(res);
		XCloseDisplay(display);
		return;
	}

	RRCrtc crtc = output_info->crtc;

	RRMode mode = None;
	for (int i = 0; i < res->nmode; i++) {
		XRRModeInfo mode_info = res->modes[i];
		if (mode_info.width == screenx && mode_info.height == screeny)
		{
			double refresh_rate = (double)mode_info.dotClock / (mode_info.hTotal * mode_info.vTotal);
			if (abs(refresh_rate - rrate) < 0.1)
			{
				mode = mode_info.id;
				break;
			}
		}
	}

	if (mode == None)
	{
		fprintf(stderr, "Error: %dx%d @ %.2fHz is not a valid display mode\n", screenx, screeny, rrate);
		XRRFreeOutputInfo(output_info);
		XRRFreeScreenResources(res);
		XCloseDisplay(display);
		return;
	}

	// Apply the configuration with rotation
	Status status = XRRSetCrtcConfig(display, res, crtc, CurrentTime,
		0, 0, mode, rotation, &output, 1);

	if (status != Success)
	{
		fprintf(stderr, "Error: Failed to set screen mode\n");
	}

	XRRFreeOutputInfo(output_info);
	XRRFreeScreenResources(res);
	XCloseDisplay(display);
}


char** get_resolutions() 
{
	Display* display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "ERROR: Could not open screen\n");
		return NULL;
	}

	XRRScreenResources* screenResources = XRRGetScreenResources(display, DefaultRootWindow(display));
	if (screenResources == NULL)
	{
		fprintf(stderr, "ERROR: Could not get screen properties\n");
		XCloseDisplay(display);
		return NULL;
	}

	char** resolutions = malloc((screenResources->nmode + 1) * sizeof(char*));
	if (resolutions == NULL)
	{
		fprintf(stderr, "ERROR: Memory allocation error\n");
		XCloseDisplay(display);
		XRRFreeScreenResources(screenResources);
		return NULL;
	}

	// fix repeated resolutions
	char** unique_resolutions = malloc((screenResources->nmode + 1) * sizeof(char*));
	if (unique_resolutions == NULL)
	{
		fprintf(stderr, "ERROR: Memory allocation error\n");
		XCloseDisplay(display);
		XRRFreeScreenResources(screenResources);
		free(resolutions);
		return NULL;
	}

	int i, unique_count = 0;
	for (i = 0; i < screenResources->nmode; i++)
	{
		XRRModeInfo* modeInfo = &screenResources->modes[i];
		char resolutionStr[32];
		snprintf(resolutionStr, sizeof(resolutionStr), "%dx%d", modeInfo->width, modeInfo->height);


		int is_unique = 1;
		for (int j = 0; j < unique_count; j++)
		{
			if (strcmp(resolutionStr, unique_resolutions[j]) == 0)
			{
				is_unique = 0;
				break;
			}
		}

		if (is_unique)
		{
			resolutions[unique_count] = strdup(resolutionStr);
			unique_resolutions[unique_count] = strdup(resolutionStr);
			unique_count++;
		}
	}

	resolutions[unique_count] = NULL;

	for (int j = 0; j < unique_count; j++)
	{
		free(unique_resolutions[j]);
	}
	free(unique_resolutions);

	XRRFreeScreenResources(screenResources);
	XCloseDisplay(display);
	return resolutions;
}

char** get_rates() 
{
	Display* display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "ERROR: Could not open screen\n");
		return NULL;
	}

	XRRScreenResources* screenResources = XRRGetScreenResources(display, DefaultRootWindow(display));
	if (screenResources == NULL)
	{
		fprintf(stderr, "ERROR: Could not get screen properties\n");
		XCloseDisplay(display);
		return NULL;
	}

	char** rates = malloc((screenResources->nmode + 1) * sizeof(char*));
	if (rates == NULL)
	{
		fprintf(stderr, "ERROR: Memory allocation error\n");
		XCloseDisplay(display);
		XRRFreeScreenResources(screenResources);
		return NULL;
	}

	int unique_count = 0;
	for (int i = 0; i < screenResources->nmode; i++)
	{
		XRRModeInfo* modeInfo = &screenResources->modes[i];

		int refresh_rate = modeInfo->dotClock / (modeInfo->hTotal * modeInfo->vTotal);
		int is_unique = 1;

		// Convert the refresh rate to a string
		char rateStr[16];
		snprintf(rateStr, sizeof(rateStr), "%d Hz", refresh_rate);

		for (int j = 0; j < unique_count; j++)
		{
			if (strcmp(rateStr, rates[j]) == 0)
			{
				is_unique = 0;
				break;
			}
		}

		if (is_unique)
		{
			rates[unique_count] = strdup(rateStr);
			unique_count++;
		}
	}

	rates[unique_count] = NULL;

	XRRFreeScreenResources(screenResources);
	XCloseDisplay(display);
	return rates;
}

char** get_outputs()
{
	Display* display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "ERROR: Could not open display\n");
		return NULL;
	}

	XRRScreenResources* screenResources = XRRGetScreenResources(display, DefaultRootWindow(display));
	if (screenResources == NULL)
	{
		fprintf(stderr, "ERROR: Could not get screen properties\n");
		XCloseDisplay(display);
		return NULL;
	}

	char** outputs = malloc((screenResources->noutput + 1) * sizeof(char*));
	if (outputs == NULL)
	{
		fprintf(stderr, "ERROR: Memory allocation error\n");
		XCloseDisplay(display);
		XRRFreeScreenResources(screenResources);
		return NULL;
	}

	int outputCount = 0;
	for (int i = 0; i < screenResources->noutput; i++)
	{
		XRROutputInfo* outputInfo = XRRGetOutputInfo(display, screenResources, screenResources->outputs[i]);
		if (outputInfo == NULL)
		{
			fprintf(stderr, "ERROR: Could not get details of the output\n");
			XCloseDisplay(display);
			XRRFreeScreenResources(screenResources);
			free(outputs);
			return NULL;
		}

		if (outputInfo->connection == RR_Connected)
		{
			outputs[outputCount] = strdup(outputInfo->name);
			outputCount++;
		}

		XRRFreeOutputInfo(outputInfo);
	}

	outputs[outputCount] = NULL;

	XRRFreeScreenResources(screenResources);
	XCloseDisplay(display);
	return outputs;
}

static void on_rescombo_changed(GtkComboBox *combo_box, gpointer user_data)
{
	GtkComboBoxText *combo_text = GTK_COMBO_BOX_TEXT(combo_box);
	const gchar *pres = gtk_combo_box_text_get_active_text(combo_text);

	Display *display = XOpenDisplay(NULL);
	if (!display)
	{
		fprintf(stderr, "Failed to open display\n");
		return;
	}

	Window root = DefaultRootWindow(display);
	XRRScreenResources *res = XRRGetScreenResourcesCurrent(display, root);
	if (!res)
	{
		fprintf(stderr, "Failed to get screen resources\n");
		XCloseDisplay(display);
		return;
	}

	//int selected_output = 0;
	XRRModeInfo *mode_info;

	char **rates = NULL;
	int rate_count = 0;

	for (int i = 0; i < res->nmode; i++)
	{
		mode_info = &res->modes[i];
		if (g_strstr_len(mode_info->name, -1, pres))
		{
			int rate = (int)((double)mode_info->dotClock / ((double)mode_info->hTotal * (double)mode_info->vTotal));
			char *rate_str = g_strdup_printf("%d Hz", rate);

			rates = g_realloc(rates, (rate_count + 1) * sizeof(char *));
			rates[rate_count] = rate_str;
			rate_count++;
		}
	}

	g_print("Refresh Rates changed to display mode: %s\n", pres);

	GtkComboBox *refcombo = GTK_COMBO_BOX(user_data);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(refcombo));

	for (int i = 0; i < rate_count; i++)
	{
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
	}

	gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);

	for (int i = 0; i < rate_count; i++)
	{
		g_free(rates[i]);
	}
	g_free(rates);

	XRRFreeScreenResources(res);
	XCloseDisplay(display);
}