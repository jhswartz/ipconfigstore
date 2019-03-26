#ifndef IPCONFIG_ERROR_H
#define IPCONFIG_ERROR_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define printError(message) \
	fprintf(stderr, "%s: %s\n", __func__, message)

#define printLibraryError(message) \
	fprintf(stderr, "%s: %s: %s\n", __func__, message, strerror(errno))

#endif
