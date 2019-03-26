#ifndef IPCONFIG_H
#define IPCONFIG_H

#include <stdbool.h>
#include <inttypes.h>

enum IPConfigAttributeType
{
	InvalidIPConfigAttributeType = -1,
	TerminalIPConfigAttributeType = 0,
	IntegerIPConfigAttributeType = 1,
	StringIPConfigAttributeType = 2,
	LinkIPConfigAttributeType = 3,
	RouteIPConfigAttributeType = 4
};

struct IPConfigAttributeKey
{
	char *key;
	enum IPConfigAttributeType type;
};

struct IPConfigLink
{
	char *address;
	uint32_t prefix;
};

struct IPConfigRoute
{
	struct IPConfigLink destination;
	char *nextHop;
};

union IPConfigValue
{
	uint32_t integer;
	char *string;
	struct IPConfigLink link;
	struct IPConfigRoute route;
};

struct IPConfigAttribute
{
	enum IPConfigAttributeType type;
	char *key;
	union IPConfigValue value;
	struct IPConfigAttribute *next;
};

struct IPConfig
{
	uint32_t version;
	struct IPConfigAttribute *attributes;
};

bool readPackedIPConfig(FILE *stream, struct IPConfig *config);
bool readUnpackedIPConfig(FILE *stream, struct IPConfig *config);
bool writePackedIPConfig(struct IPConfig *config, FILE *stream);
bool writeUnpackedIPConfig(struct IPConfig *config, FILE *stream);

void deinitializeIPConfig(struct IPConfig *config);

#endif
