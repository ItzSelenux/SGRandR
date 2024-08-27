#include "sgrandr.h"

int main(int argc, char *argv[])
{
	if (argc > 0)
		pm = argv[0];
	else
		pm = "sgrandr";

	locale();

	int testmode = 0;

	const char* env_sgcsd = getenv("SGCSD");
	const char* env_SType = getenv("XDG_SESSION_TYPE");

	if (env_SType != NULL)
		{
			if (strcmp(env_SType, "wayland") == 0)
			{
				g_print("Running SGRandr on wayland mode, wlr-randr will be used and some features are not available\n");
				wlr = 1;
			} 
			else if (strcmp(env_SType, "x11") == 0)
			{
				g_print("Running SGRandr on X11, xrandr will be used\n");
				wlr = 0;
			}
			else
			{
				g_print("WARNING: XDG_SESSION_TYPE is not wayland or X11, asumming that x11 session is running\n");
				g_print("Running SGRandr on X11, xrandr will be used\n");
				wlr = 0;
			}
		}

	nocsd = (env_sgcsd != NULL) ? atoi(env_sgcsd) == 0 : 0;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--nocsd") == 0)
		{
			nocsd = 1;
			printf("CSD Disabled, using fallback display\n");
		}
		else if (strcmp(argv[i], "--testmode") == 0)
		{
			testmode = 1;
		}
	}

	gtk_init(&argc, &argv);

	if(testmode)
		printf("--testmode is eneable, displaying all options \n");

	if ( !file_exists_in_path("sgrandr-cr") || access("./sgrandr-cr", F_OK) == 0)
	{
		printf("sgrandr-cr detected\n");
	}
	else
	{
		printf("\033[1;33mWARNING\033[0m: sgrandr-cr not detected, hiding button.\n");
		gtk_widget_hide(submenu_item1);
	}


	if ((wlr == 1 && !file_exists_in_path("wlr-randr")) || (wlr == 0 && !file_exists_in_path("xrandr")))
	{
		show_error_dialog("ERROR: wlr-randr or xrandr is not installed");
		exit(EXIT_FAILURE);
	}

create_window();

return 0;
}
