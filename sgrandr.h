#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libintl.h"
#include <locale.h>

//locale data
#define _(String) gettext(String)
#define GETTEXT_PACKAGE "sgrandr"
#define localedir "/usr/share/locale"
#define ML 4096
//system data
int wlr = 0, sm = 0, am = 0, nocsd = 0, num_rows, testmode = 0;
int *num_rows_ptr;
const char* pver = mver;
char *command, *pm;
//logic data
char **resolutions, **rates, **outputs;
GtkAccelGroup *accel_group;

GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;
GtkTreeModel *model;
//ui data
GtkWidget *window, *dialog,
*headerbar,*button,*image,*wtitle,
*submenu,*submenu_item1,*submenu_item2,*submenu_item3,
*submenu_item4,*submenu_item5,*submenu_item6,

*grid,
*rescombo,*refcombo,*rotcombo,*outcombo,*outcombo2,*scacombo,
*offon,*pos,
*scalabel,*outlabel,*poslabel,*synclabel,
*syncchk,*slider,*defbtn,*applybtn,

//ui data for cr
*global_label0,*global_label3,
*width,*height,*rate;

char xcmd0[ML], xcmd1[ML], defres[ML],defrate[ML];
gboolean gsync;

#include "events.h"
#include "outputmanager.h"
#include "mainwindow.h"
#include "cr.h"