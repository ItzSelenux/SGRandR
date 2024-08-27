#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libintl.h"
#include "locale.h"

#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>

//locale data
#define _(String) gettext(String)
#define GETTEXT_PACKAGE "sgrandr"
#define localedir "/usr/share/locale"

//system data
const char* pver = mver;
#define BUFFER_SIZE 1024
int sm = 0;
char *command;
int num_rows;
int *num_rows_ptr;

int testmode = 0, nocsd = 0;

//logic data
char **resolutions;
char **rates;
char **outputs;
char *pm;
GtkAccelGroup *accel_group;

// GtkWidget pointers
GtkWidget *window, *headerbar, *button, *image, *wtitle, *submenu,
	*submenu_item1, *submenu_item2, *submenu_item3, *submenu_item4, *submenu_item5,
	*grid, *rescombo, *refcombo, *rotcombo, *outcombo, *offon, *pos,
	*outcombo2, *slider, *scacombo, *scalabel, *outlabel, *poslabel,
	*defbtn, *applybtn,
	*global_label0, *global_label3, *width, *height, *rate;

// GtkIconTheme
GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

GtkTreeModel *model;

char xcmd0[100];
char xcmd1[100];

#include "outputmanager.h"
#include "events.h"
#include "mainwindow.h"