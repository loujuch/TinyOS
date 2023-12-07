#ifndef _STDLIB_H__
#define _STDLIB_H__

#include "stdint.h"

int8_t abs8(int8_t i);
int16_t abs16(int16_t i);
int32_t abs32(int32_t i);

size_t parseDInt8(char *buf, int8_t i);
size_t parseDInt16(char *buf, int16_t i);
size_t parseDInt32(char *buf, int32_t i);

size_t parseUInt8(char *buf, uint8_t i);
size_t parseUInt16(char *buf, uint16_t i);
size_t parseUInt32(char *buf, uint32_t i);

size_t parseXInt8(char *buf, uint8_t i);
size_t parseXInt16(char *buf, uint16_t i);
size_t parseXInt32(char *buf, uint32_t i);

size_t parseOInt8(char *buf, uint8_t i);
size_t parseOInt16(char *buf, uint16_t i);
size_t parseOInt32(char *buf, uint32_t i);

#endif