#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libintl.h"
#include "locale.h"

//locale data
#define _(String) gettext(String)
#define GETTEXT_PACKAGE "sgrandr"
#define localedir "/usr/share/locale"

//system data
int wayland = 0;
const char* pver = mver;
#define BUFFER_SIZE 1024
int sm = 0;
int am = 0;
char *command;
int nocsd = 0;
int num_rows;
int *num_rows_ptr;
//logic data
char **resolutions;
char **rates;
char **outputs;
char *pm;
GtkAccelGroup *accel_group;

//ui data
GtkWidget *window;
GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;
GtkWidget *headerbar;
GtkWidget *button;
GtkWidget *image;
GtkWidget *wtitle;
GtkWidget *submenu;
GtkWidget *submenu_item1;
GtkWidget *submenu_item2;
GtkWidget *submenu_item3;
GtkWidget *submenu_item4;
GtkWidget *submenu_item5;
GtkWidget *submenu_item6;

GtkWidget *grid;
GtkTreeModel *model;
GtkWidget *rescombo;
GtkWidget *refcombo;
GtkWidget *rotcombo;
GtkWidget *outcombo;
GtkWidget *offon;
GtkWidget *pos;
GtkWidget *outcombo2;
GtkWidget *slider;
GtkWidget *scacombo;
GtkWidget *scalabel;
GtkWidget *outlabel;
GtkWidget *poslabel;
GtkWidget *defbtn;
GtkWidget *applybtn;
GtkWidget *synclabel;
GtkWidget *syncchk;

//ui data for cr
GtkWidget *global_label0;
GtkWidget *global_label3;
GtkWidget *width;
GtkWidget *height;
GtkWidget *rate;

char xcmd0[100];
char xcmd1[100];
gboolean gsync;

//init locale
int locale()
{
setlocale(LC_ALL, "");
bindtextdomain(GETTEXT_PACKAGE, localedir);
textdomain(GETTEXT_PACKAGE);
}


int getsync_status()
{
	FILE *inputFile = popen("wlr-randr", "r");
	if (inputFile == NULL)
	{
		perror("Error al ejecutar wlr-randr");
		exit(EXIT_FAILURE);
	}

	char line[256];
	while (fgets(line, sizeof(line), inputFile) != NULL)
	{
		if (strstr(line, "Adaptive Sync: disabled") != NULL)
		{
			pclose(inputFile);
			return 0;
		} 
		else if (strstr(line, "Adaptive Sync: enabled") != NULL)
		{
			pclose(inputFile);
			return 1;
		}
	}

	pclose(inputFile);
	return 0;
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

void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
{
	gsync = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(syncchk));
	char *wsync;
	if (gsync == 1)
	{
	wsync = "enabled";
	}
	else
	{
	wsync = "disabled";
	}

	int *num_rows_ptr = (int *)user_data;
	num_rows = *num_rows_ptr;
	locale();

	const char *cpos;
	const char *csca;
	const char *crot;
	
	int ps;
	const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
	const char *opwr = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(offon));
	const char *output2 = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo2));
	const char *resolution = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rescombo));
	const char *refresh_rate = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(refcombo));
	const char *rotation = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(rotcombo));
	const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(pos));
	const char *scalingmode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(scacombo));

	if (g_strcmp0(opwr, _("On")) == 0)
	{
		ps = 0;
	}
	else if (g_strcmp0(opwr, _("Off")) == 0)
	{
	ps = 1;
	}

	if (active_text != NULL) 
	{
		if (g_strcmp0(active_text, _("Same as")) == 0) 
		{
			cpos = "--same-as";
		}
		else if (g_strcmp0(active_text, _("Left of")) == 0) 
		{
			cpos = "--left-of";
		} 
		else if (g_strcmp0(active_text, _("Right of")) == 0) 
		{
			cpos = "--right-of";
		} 
		else if (g_strcmp0(active_text, _("Above of")) == 0) 
		{
			cpos = "--above";
		} 
		else if (g_strcmp0(active_text, _("Below of")) == 0) 
		{
			cpos = "--below";
		}
	}
	if (scalingmode != NULL) 
	{
		if (g_strcmp0(scalingmode, _("Full")) == 0) 
		{
			csca = "Full";
		}
		else if (g_strcmp0(scalingmode, _("Center")) == 0) 
		{
			csca = "Center";
		} 
		else if (g_strcmp0(scalingmode, _("Aspect")) == 0) 
		{
			csca = "Aspect";
		} 
		else if (g_strcmp0(scalingmode, _("1:1")) == 0) 
		{
			csca = "1:1";
		}
	}
	if (rotation != NULL) 
	{
		if (wayland == 0)
		{
			if (g_strcmp0(rotation, _("normal")) == 0) 
			{
				crot = "normal";
			}
			else if (g_strcmp0(rotation, _("left")) == 0) 
			{
				crot = "left";
			} 
			else if (g_strcmp0(rotation, _("right")) == 0) 
			{
				crot = "right";
			} 
			else if (g_strcmp0(rotation, _("inverted")) == 0) 
			{
				crot = "inverted";
			}
		}
		else
		{
			if (g_strcmp0(rotation, _("normal")) == 0) 
			{
				crot = "normal";
			}
			else if (g_strcmp0(rotation, _("left")) == 0) 
			{
				crot = "90";
			} 
			else if (g_strcmp0(rotation, _("right")) == 0) 
			{
				crot = "270";
			} 
			else if (g_strcmp0(rotation, _("inverted")) == 0) 
			{
				crot = "180";
			}
		}
	}

int sld = gtk_range_get_value(GTK_RANGE(slider));
double scl = (double)(sld) / (double)(100);
char slider[16];
snprintf(slider, sizeof(slider), "%.2f", scl);

// Replace commas with periods
char *ptr = strchr(slider, ',');
while (ptr != NULL) 
{
	*ptr = '.';
	ptr = strchr(ptr, ',');
}
		
	command = (char*) malloc(sizeof(char) * 100);

		if (wayland == 0)
		{
			if (ps == 1) 
			{
				sprintf(command, "xrandr --output %s --off ", output);
			}
			else if (num_rows > 1 && sm == 1) 
			{
				sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s --set \"scaling mode\" \" %s \"  %s %s ", output, resolution, refresh_rate, crot, slider, csca, cpos, output2);
			}
			else if (sm == 1) 
			{
				sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s --set \"scaling mode\" \"%s\" ", output, resolution, refresh_rate, crot, slider, csca);
			}
			else if (num_rows > 1) 
			{
				sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s %s %s", output, resolution, refresh_rate, crot, slider, cpos, output2);
			}
			else if (num_rows == 1) 
			{
				sprintf(command, "xrandr --output %s --mode %s --rate %s --rotation %s --scale %s", output, resolution, refresh_rate, crot, slider);
			}
		}
		else
		{
			if (ps == 1) 
			{
				sprintf(command, "wlr-randr --output %s --off ", output);
			}
			//else if (num_rows > 1 && sm == 1) 
			//{
				//sprintf(command, "wlr --output %s --mode %s --rate %s --rotation %s --scale %s --set \"scaling mode\" \" %s \"  %s %s ", output, resolution, refresh_rate, crot, slider, csca, cpos, output2);
			//}
			//else if (sm == 1) 
			//{
				//sprintf(command, "wlr --output %s --mode %s --rate %s --rotation %s --scale %s --set \"scaling mode\" \"%s\" ", output, resolution, refresh_rate, crot, slider, csca);
			//}
			else if (num_rows > 1) 
			{
				sprintf(command, "wlr-randr --output %s --mode %s@s --transform %s --scale %s %s %s", output, resolution, refresh_rate, crot, slider, cpos, output2);
			}
			else if (num_rows == 1) 
			{
				sprintf(command, "wlr-randr --output %s --mode  %s@%s --transform %s --scale %s", output, resolution, refresh_rate, crot, slider);
			}
			else if (num_rows == 1 && am == 1) 
			{
				sprintf(command, "wlr-randr --output %s --mode  %s@%s --transform %s --scale %s --adaptative-sync %s", output, resolution, refresh_rate, crot, slider, wsync);
			}
		}
 	printf("%s\n", command);


	char command_with_stderr[256];
	snprintf(command_with_stderr, sizeof(command_with_stderr), "%s 2>&1", command);

	FILE *cmd_output = popen(command_with_stderr, "r");
	if (cmd_output == NULL)
	{
		show_error_dialog("Error executing command");
		return;
	}

	char output_buffer[1024];
	size_t read_size;
	char stderr_buffer[1024];
	stderr_buffer[0] = '\0';
	char *error_message = NULL;
	
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
		{
			error_message = stderr_buffer;
		}
	}

	if (error_message)
	{
		show_error_dialog(error_message);
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


void on_submenu_item2_toggled(GtkCheckMenuItem *menu_item,void *ptr, gpointer user_data) 
{

if (gtk_check_menu_item_get_active(menu_item)) 
	{
			gtk_widget_show(scacombo);
			gtk_widget_show(scalabel);
			int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
			*int_ptr = 1;
	} 
else 
	{
			gtk_widget_hide(scacombo);
			gtk_widget_hide(scalabel);
			int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
			*int_ptr = 0;
	}
}

void on_submenu_item6_toggled(GtkCheckMenuItem *menu_item,void *ptr, gpointer user_data) 
{

if (gtk_check_menu_item_get_active(menu_item)) 
	{
			gtk_widget_show(syncchk);
			gtk_widget_show(synclabel);
			int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
			*int_ptr = 1;
	} 
else 
	{
			gtk_widget_hide(syncchk);
			gtk_widget_hide(synclabel);
			int *int_ptr = (int *) ptr; // cast the void pointer to an int pointer
			*int_ptr = 0;
	}
}

void on_default_button_clicked(GtkButton *button, gpointer user_data) 
{
	const char *output = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));

	char *command = (char*) malloc(sizeof(char) * 100);
	if (wayland == 0)
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
	gtk_combo_box_set_active(GTK_COMBO_BOX(rescombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(rotcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(scacombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(offon), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(outcombo2), 0);
	gtk_combo_box_set_active(GTK_COMBO_BOX(pos), 0);
}

void restart_program(GtkWidget *widget, gpointer data)
{
	printf("Program Reloaded...\n");
	char *args[] = {pm, NULL};
	execvp(args[0], args);
}

static void on_rescombo_changed(GtkComboBox *combo_box, gpointer user_data)
{
	FILE* pipe;
	GtkComboBoxText *combo_text = GTK_COMBO_BOX_TEXT(combo_box);
	   const gchar *pres = gtk_combo_box_text_get_active_text(combo_text);

	char *sres = g_strdup_printf("%s ", pres);  // allocate enough space for the copied string and the space character

	char **get_rates()
	{
		char buffer[BUFFER_SIZE];
		char **rates = malloc(BUFFER_SIZE * sizeof(char *));
		int count = 0;

			if (wayland == 0)
	{
		command = "xrandr";
	}
	else
	{
		command = "wlr-randr";
	}

	pipe = popen(command, "r");
		if (!pipe)
		{
			fprintf(stderr, "Error executing command.\n");
			return NULL;
		}

		int ures = 1;

	while (fgets(buffer, BUFFER_SIZE, pipe))
	{
		if (strstr(buffer, sres))
		{
			char *rate = strtok(buffer, " ");
			while (rate != NULL && count < BUFFER_SIZE)
			{
				rate[strcspn(rate, "+\n")] = 0;

				char *hz_ptr = strstr(rate, "Hz");
				if (hz_ptr != NULL)
				{
					*hz_ptr = '\0';
				}
				char *current_ptr = strstr(rate, "current");
				if (current_ptr != NULL)
				{
					*current_ptr = '\0';
				}
				char *preferred_ptr = strstr(rate, "preferred");
				if (preferred_ptr != NULL)
				{
					*preferred_ptr = '\0';
				}
				char *p1 = strstr(rate, "(");
				if (p1 != NULL)
				{
					*p1 = '\0';
				}

				if (!ures && strcmp(rate, "") != 0 && strchr(rate, 'x') == NULL)
				{
					rates[count] = malloc(strlen(rate) + 1);
					strcpy(rates[count], rate);

					count++;
				}

				if (ures)
				{
					ures = 0;
				}

				rate = strtok(NULL, " ");
			}
		}
	}

		pclose(pipe);

		// Add NULL terminator to array
		if (count < BUFFER_SIZE) 
		{
			rates[count] = NULL;
		}
		else
		{
			fprintf(stderr, "Array index out of bounds.\n");
		}

		return rates;
	}

	char **rates = get_rates();

			g_print("Refresh Rates changed to display mode: %s\n", sres);
			
	rates = get_rates();
	GtkComboBox *refcombo = GTK_COMBO_BOX(user_data);
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(refcombo));
	for (int i = 0; rates[i] != NULL; i++) 
	{
		printf("%s\n", rates[i]);
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(refcombo), rates[i]);
		gtk_combo_box_set_active(GTK_COMBO_BOX(refcombo), 0);

	}
	free(rates);

	g_free(sres);

}

char** get_resolutions()
{
	FILE* pipe;
	char* command;

	if (wayland == 0)
	{
		command = "xrandr";
	}
	else
	{
		command = "wlr-randr";
	}

	pipe = popen(command, "r");

	if (pipe == NULL)
	{
		fprintf(stderr, "Error opening pipe.\n");
		exit(EXIT_FAILURE);
	}

	char** resolutions = malloc(BUFFER_SIZE * sizeof(char*));
	char buffer[BUFFER_SIZE];
	int i = 0;

	while (fgets(buffer, BUFFER_SIZE, pipe) != NULL)
	{
		if (wayland == 0 && strstr(buffer, "  ") != NULL)
		{
			char* resolution = strtok(buffer, " ");
			resolutions[i] = strdup(resolution);
			i++;
		}
		else if (wayland == 1 && strstr(buffer, "px") != NULL)
		{
			char* resolution = strtok(buffer, " ");
			resolutions[i] = strdup(resolution);
			i++;
		}
	}

	resolutions[i] = NULL;
	pclose(pipe);

	// Delete duplicated entries
	if (wayland == 1)
	{
		char** unique_resolutions = malloc(BUFFER_SIZE * sizeof(char*));
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
	char buffer[BUFFER_SIZE];
	FILE* fp = popen(command, "r");
	if (fp == NULL) 
	{
		fprintf(stderr, "Failed to execute command\n");
		exit(EXIT_FAILURE);
	}

	char** outputs = (char**)malloc(sizeof(char*));
	int num_outputs = 0;

	while (fgets(buffer, BUFFER_SIZE, fp) != NULL) 
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


void on_submenu_item1_selected(GtkMenuItem *menuitem, gpointer userdata)
{
	if (nocsd == 1)
	{
		if (access("/usr/bin/sgrandr-cr", F_OK) == 0)
		{
			system("/usr/bin/sgrandr-cr --nocsd");
		} 
		else if (access("./sgrandr-cr", F_OK) == 0)
		{
			system("./sgrandr-cr --nocsd");
		} 
		else
		{
			printf("\033[1;31mERROR\033[0m: sgrandr-cr not detected, are you in testmode?\n");
		}
	} 
	else 
	{
		if (access("/usr/bin/sgrandr-cr", F_OK) == 0)
		{
			system("/usr/bin/sgrandr-cr");
		} 
		else if (access("./sgrandr-cr", F_OK) == 0)
		{
			system("./sgrandr-cr");
		}
		else
		{
			printf("\033[1;31mERROR\033[0m: sgrandr-cr not detected, are you in testmode?\n");
		}
	}
}


void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	GtkWidget *dialog;
	dialog = gtk_about_dialog_new();
	
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

void on_submenu_item5_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	on_apply_button_clicked(NULL, &num_rows);

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
		
		char chmodCommand[100];
		sprintf(chmodCommand, "chmod +x %s", filename);
		system(chmodCommand);
		fclose(file);
		g_free(filename);
	}

	gtk_widget_destroy(dialog5);

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


void on_save_button_clicked(GtkButton *button, gpointer user_data) 
{
	const char *text0 = gtk_label_get_text(GTK_LABEL(global_label0));
	const char *text3 = gtk_label_get_text(GTK_LABEL(global_label3));

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

	   
		char content[200];
		sprintf(content, "%s &&\nsleep 2 &&\n%s", text0, text3);
		fputs(content, file);
		printf("File saved: %s", filename);
		
		char chmodCommand[100];
		sprintf(chmodCommand, "chmod +x %s", filename);
		system(chmodCommand);
		fclose(file);
		g_free(filename);
	}

	gtk_widget_destroy(dialog);
}

void on_ok_button_clicked(GtkButton *button, gpointer user_data) 
{
	// Close the window when the "OK" button is clicked
	GtkWidget *window = GTK_WIDGET(user_data);
	gtk_widget_destroy(window);
}


void on_applybtn_clicked(GtkButton *button, gpointer user_data) 
{
	const char *srate = gtk_entry_get_text(GTK_ENTRY(rate));
	const char *swidth = gtk_entry_get_text(GTK_ENTRY(width));
	const char *sheight = gtk_entry_get_text(GTK_ENTRY(height));

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

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	const gchar *markupTitle = "SGRandR - %s";
	const gchar *translatedTitle = _("Commands for custom resolution");
	gchar *formattedMarkupTitle = g_markup_printf_escaped(markupTitle, translatedTitle);
	gtk_window_set_title(GTK_WINDOW(window), formattedMarkupTitle);

	GtkIconTheme *theme = gtk_icon_theme_get_default();
	GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "video-display", 48, 0);

	// Create the header bar
	GtkWidget *headerbar = gtk_header_bar_new();
	gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
	
	GtkWidget *wicon = gtk_image_new_from_icon_name("video-display", GTK_ICON_SIZE_BUTTON);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wicon);

	GtkWidget *wtitle = gtk_label_new(NULL);
	const gchar *markupHeader = "<b>%s - SGRandR</b>";
	gchar *formattedMarkupHeader = g_markup_printf_escaped(markupTitle, translatedTitle);
	gtk_label_set_markup(GTK_LABEL(wtitle), formattedMarkupHeader);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
	if (nocsd == 0 )
	{
	gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);
	}
	
	if (info != NULL)
	{
		GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}

	// Create a label 
	const char *display = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(outcombo));
	GtkWidget *label0 = gtk_label_new(_("Copy this commands into a Terminal Window\n"));
	GtkWidget *label1 = gtk_label_new("");
	GtkWidget *label2 = gtk_label_new("");

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

	sprintf(xcmd0, "xrandr --newmode %s", output);
	sprintf(xcmd1, "xrandr --addmode %s %sx%s_%s.00 \n", display, swidth, sheight, srate);
	
xcmd0[strcspn(xcmd0, "\n")] = '\0';


	// Set the label text with the command output
	gtk_label_set_text(GTK_LABEL(label1), xcmd0);
	gtk_label_set_text(GTK_LABEL(label2), xcmd1);
	gtk_label_set_selectable(GTK_LABEL(label1), TRUE);
	gtk_label_set_selectable(GTK_LABEL(label2), TRUE);

	// Create a vertical box layout to hold the label and buttons
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label0, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
	GtkWidget *save_button = gtk_button_new_with_label(_("Save into a file"));
	GtkWidget *execute_button = gtk_button_new_with_label(_("Execute commands"));
	GtkWidget *ok_button = gtk_button_new_with_label(_("OK"));
	
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
