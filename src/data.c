#define _GNU_SOURCE

#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "data.h"

uint16_t convertBigEndianUInt16(uint16_t value)
{
	union { uint16_t value; unsigned char data[2]; } aux = { 0x4142 };

	if (aux.data[0] == 0x41)
	{
		return value;
	}

	aux.data[0] = (value >> 8) & 0xff;
	aux.data[1] = value & 0xff;

	return aux.value;
}

uint32_t convertBigEndianUInt32(uint32_t value)
{
	union { uint32_t value; unsigned char data[4]; } aux = { 0x41424344 };

	if (aux.data[0] == 0x41)
	{
		return value;
	}

	aux.data[0] = (value >> 24) & 0xff;
	aux.data[1] = (value >> 16) & 0xff;
	aux.data[2] = (value >> 8) & 0xff;
	aux.data[3] = value & 0xff;

	return aux.value;
}

bool readPackedLink(FILE *stream, struct IPConfigLink *link)
{
	char **address = &link->address;
	uint32_t *prefix = &link->prefix;

	if (!readPackedString(stream, address))
	{
		return false;
	}

	if (!readPackedUInt32(stream, prefix))
	{
		return false;
	}

	return true;
}

bool readPackedRoute(FILE *stream, struct IPConfigRoute *route)
{
	char **destinationAddress = &route->destination.address;
	uint32_t *destinationPrefix = &route->destination.prefix;
	char **nextHop = &route->nextHop;

	uint32_t haveDestination = 0;
	uint32_t haveNextHop = 0;

	if (!readPackedUInt32(stream, &haveDestination))
	{
		return false;
	}

	if (haveDestination)
	{
		if (!readPackedString(stream, destinationAddress))
		{
			return false;
		}

		if (!readPackedUInt32(stream, destinationPrefix))
		{
			return false;
		}
	}

	if (!readPackedUInt32(stream, &haveNextHop))
	{
		return false;
	}

	if (haveNextHop)
	{
		if (!readPackedString(stream, nextHop))
		{
			return false;
		}
	}

	return true;
}

bool readPackedString(FILE *stream, char **string)
{
	uint16_t length = 0;

	if (!readPackedUInt16(stream, &length))
	{
		return false;
	}

	while (length == 0)
	{
		if (fseek(stream, sizeof(uint16_t), SEEK_CUR) == -1)
		{
			return false;
		}

		if (!readPackedUInt16(stream, &length))
		{
			return false;
		}
	}

	*string = calloc(1, length + 1);

	if (!*string)
	{
		return false;
	}

	if (fread(*string, length, 1, stream) != 1)
	{
		if (!feof(stream))
		{
			free(*string);
			return false;
		}
	}

	return true;
}

bool readPackedUInt16(FILE *stream, uint16_t *value)
{
	uint16_t buffer = 0; 

	if (fread(&buffer, sizeof buffer, 1, stream) != 1)
	{
		return false;
	}

	*value = convertBigEndianUInt16(buffer);
	return true;
}

bool readPackedUInt32(FILE *stream, uint32_t *value)
{
	uint32_t buffer = 0; 

	if (fread(&buffer, sizeof buffer, 1, stream) != 1)
	{
		return false;
	}

	*value = convertBigEndianUInt32(buffer);
	return true;
}

bool writePackedRoute(struct IPConfigRoute *route, FILE *stream)
{
	struct IPConfigLink *destination = &route->destination;

	if (destination->address && destination->prefix)
	{
		if (!writePackedUInt32(1, stream))
		{
			return false;
		}

		if (!writePackedString(destination->address, stream))
		{
			return false;
		}

		if (!writePackedUInt32(destination->prefix, stream))
		{
			return false;
		}
	}

	else
	{
		if (!writePackedUInt32(0, stream))
		{
			return false;
		}
	}

	if (route->nextHop)
	{
		if (!writePackedUInt32(1, stream))
		{
			return false;
		}

		if (!writePackedString(route->nextHop, stream))
		{
			return false;
		}
	}

	else
	{
		if (!writePackedUInt32(0, stream))
		{
			return false;
		}
	}

	return true;
}

bool writePackedLink(struct IPConfigLink *link, FILE *stream)
{
	if (!writePackedString(link->address, stream))
	{
		return false;
	}

	if (!writePackedUInt32(link->prefix, stream))
	{
		return false;
	}

	return true;
}

bool writePackedString(char *string, FILE *stream)
{
	size_t stringLength = strlen(string);

	if (!writePackedUInt16(stringLength, stream))
	{
		return false;
	}

	return fwrite(string, stringLength, 1, stream) == 1;
}

bool writePackedUInt16(uint16_t value, FILE *stream)
{
	uint16_t buffer = convertBigEndianUInt16(value);
	return fwrite(&buffer, sizeof buffer, 1, stream) == 1;
}

bool writePackedUInt32(uint32_t value, FILE *stream)
{
	uint32_t buffer = convertBigEndianUInt32(value);
	return fwrite(&buffer, sizeof buffer, 1, stream) == 1;
}

bool readUnpackedLine(FILE *stream, char **line)
{
	char *cursor = NULL;

	*line = calloc(1, BUFSIZ);

	if (!*line)
	{
		return false;
	}
	
	cursor = *line;

	while (cursor - *line < BUFSIZ)
	{
		int character = fgetc(stream);

		if (character == EOF || character == '\n')
		{
			break;
		}

		*cursor++ = character;
	}

	return true;
}

bool parseUnpackedPair(char *line, char **key, char **value)
{
	char *next = index(line, ':');

	if (!next)
	{
		return false;
	}

	*key = strndup(line, next - line);

	if (!*key)
	{
		return false;
	}

	while (*++next && isspace(*next));
	*value = strdup(next);

	if (!*value)
	{
		free(*key);
		return false;
	}

	return true;
}

bool parseUnpackedRoute(char *string, struct IPConfigRoute *route)
{
	char *next = index(string, ' ');

	if (next)
	{
		*next = 0;

		if (!parseUnpackedLink(string, &route->destination))
		{
			return false;
		}

		string = ++next;
	}

	route->nextHop = strdup(string);

	if (!route->nextHop)
	{
		return false;
	}

	return true;
}

bool parseUnpackedLink(char *string, struct IPConfigLink *link)
{
	char *next = index(string, '/');

	if (!next)
	{
		return false;
	}

	link->address = strndup(string, next - string);

	if (!link->address)
	{
		return false;
	}

	if (!parseUnpackedUInt32(++next, &link->prefix))
	{
		free(link->address);
		return false;
	}

	return true;
}

bool parseUnpackedUInt32(char *string, uint32_t *integer)
{
	char *terminator = NULL;
	unsigned long value = strtoul(string, &terminator, 10);

	if (*terminator)
	{
		return false;
	}

	*integer = value % UINT32_MAX;
	return true;
}
