#ifndef _BIT_MAP_H__
#define _BIT_MAP_H__

#include "stdint.h"

struct bitmap {
	uint32_t *bits;
	size_t size;
};

typedef struct bitmap bitmap;

void set_bitmap_bit(bitmap *bm, size_t n);
uint32_t test_bitmap(bitmap *bm, size_t n);

uint32_t allocate_a_bit(bitmap *bm);

#endif