#include "stdlib.h"

#include "stdint.h"
#include "string.h"
#include "assert.h"

int8_t abs8(int8_t i) {
	return i < 0 ? -i : i;
}

int16_t abs16(int16_t i) {
	return i < 0 ? -i : i;
}

int32_t abs32(int32_t i) {
	return i < 0 ? -i : i;
}

size_t parseDInt8(char *buf, int8_t i) {
	assert(buf != NULL);
	return parseDInt32(buf, (int32_t)i);
}

size_t parseDInt16(char *buf, int16_t i) {
	assert(buf != NULL);
	return parseDInt32(buf, (int32_t)i);
}

size_t parseDInt32(char *buf, int32_t i) {
	assert(buf != NULL);
	char *base = buf;
	*buf++ = i < 0 ? '-' : '+';
	while(i) {
		*buf++ = abs8(i % 10) + '0';
		i /= 10;
	}
	if(base + 1 == buf) {
		*buf++ = '0';
	}
	*buf = '\0';
	size_t res = buf - base - 1;
	strnrev(base + 1, res);
	return res;
}

size_t parseUInt8(char *buf, uint8_t i) {
	assert(buf != NULL);
	return parseUInt32(buf, (uint32_t)i);
}

size_t parseUInt16(char *buf, uint16_t i) {
	assert(buf != NULL);
	return parseUInt32(buf, (uint32_t)i);
}

size_t parseUInt32(char *buf, uint32_t i) {
	assert(buf != NULL);
	char *base = buf;
	while(i) {
		*buf++ = abs8(i % 10) + '0';
		i /= 10;
	}
	if(base == buf) {
		*buf++ = '0';
	}
	*buf = '\0';
	size_t res = buf - base;
	strnrev(base, res);
	return res;
}

static char numberChar(uint8_t i) {
	return i < 10 ? (i + '0') : (i - 10 + 'A');
}

size_t parseXInt8(char *buf, uint8_t i) {
	assert(buf != NULL);
	return parseXInt32(buf, (uint32_t)i);
}

size_t parseXInt16(char *buf, uint16_t i) {
	assert(buf != NULL);
	return parseXInt32(buf, (uint32_t)i);
}

size_t parseXInt32(char *buf, uint32_t i) {
	assert(buf != NULL);
	char *base = buf;
	while(i) {
		*buf++ = numberChar(i & 0xF);
		i >>= 4;
	}
	if(buf == base) {
		*buf++ = '0';
	}
	*buf = '\0';
	size_t res = buf - base;
	strnrev(base, res);
	return res;
}

size_t parseOInt8(char *buf, uint8_t i) {
	assert(buf != NULL);
	return parseOInt32(buf, (uint32_t)i);
}

size_t parseOInt16(char *buf, uint16_t i) {
	assert(buf != NULL);
	return parseOInt32(buf, (uint32_t)i);
}

size_t parseOInt32(char *buf, uint32_t i) {
	assert(buf != NULL);
	char *base = buf;
	while(i) {
		*buf++ = numberChar(i & 0x7);
		i >>= 3;
	}
	if(buf == base) {
		*buf++ = '0';
	}
	*buf = '\0';
	size_t res = buf - base;
	strnrev(base, res);
	return res;
}