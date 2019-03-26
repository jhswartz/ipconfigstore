#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "ipconfig.h"
#include "error.h"

static void usage(FILE *stream)
{
	fprintf(stream, "usage: ipconfigstore OPTION\n");
	fprintf(stream, "\n");
	fprintf(stream, "Options:\n");
	fprintf(stream, "  -p VERSION    Pack IP configuration\n");
	fprintf(stream, "  -u            Unpack IP configuration\n");
	fprintf(stream, "\n");
}

int main(int argc, char *argv[])
{
	int option = 0;
	struct IPConfig config = {0};

	option = getopt(argc, argv, "hp:u");

	if (option == 'h')
	{
		usage(stdout);
		return EXIT_SUCCESS;
	}

	else if (option == 'p')
	{
		config.version = *optarg - 0x30;

		if (!readUnpackedIPConfig(stdin, &config))
		{
			return EXIT_FAILURE;
		}

		if (!writePackedIPConfig(&config, stdout))
		{
			return EXIT_FAILURE;
		}

		deinitializeIPConfig(&config);
	}

	else if (option == 'u')
	{
		if (!readPackedIPConfig(stdin, &config))
		{
			return EXIT_FAILURE;
		}

		if (!writeUnpackedIPConfig(&config, stdout))
		{
			return EXIT_FAILURE;
		}

		deinitializeIPConfig(&config);
	}

	else
	{
		usage(stderr);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
