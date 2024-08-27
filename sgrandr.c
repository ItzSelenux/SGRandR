#include "sgrandr.h"

 int main(int argc, char *argv[])
{
	locale();

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
	create_window();
}