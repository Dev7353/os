#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{

	int c = 0;
	while ((c = getopt (argc, argv, "hpl")) != -1)
		switch (c)
		{
		case 'h':
			printf("Help\n");
			return 0;
		case '?':
			if (optopt == 'i')
				fprintf (stderr,
					 "Option -%c requires an argument.\n",
					 optopt);
		}
}
