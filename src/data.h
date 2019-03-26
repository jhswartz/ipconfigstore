#ifndef IPCONFIG_DATA_H
#define IPCONFIG_DATA_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "ipconfig.h"

uint16_t convertBigEndianUInt16(uint16_t value);
uint32_t convertBigEndianUInt32(uint32_t value);

bool readPackedRoute(FILE *stream, struct IPConfigRoute *route);
bool readPackedLink(FILE *stream, struct IPConfigLink *link);
bool readPackedString(FILE *stream, char **string);
bool readPackedUInt16(FILE *stream, uint16_t *value);
bool readPackedUInt32(FILE *stream, uint32_t *value);

bool writePackedRoute(struct IPConfigRoute *route, FILE *stream);
bool writePackedLink(struct IPConfigLink *link, FILE *stream);
bool writePackedString(char *string, FILE *stream);
bool writePackedUInt16(uint16_t value, FILE *stream);
bool writePackedUInt32(uint32_t value, FILE *stream);

bool readUnpackedLine(FILE *stream, char **line);
bool parseUnpackedPair(char *line, char **key, char **value);
bool parseUnpackedRoute(char *string, struct IPConfigRoute *route);
bool parseUnpackedLink(char *string, struct IPConfigLink *link);
bool parseUnpackedUInt32(char *string, uint32_t *integer);

#endif
