#include "bitmap.h"

void set_bitmap_bit(bitmap *bm, size_t n) {
	bm->bits[n / 32] = bm->bits[n / 32] | (0x1 << (n % 32));
}

uint32_t test_bitmap(bitmap *bm, size_t n) {
	return bm->bits[n / 32] & ~((0x1 << (n % 32)));
}

uint32_t allocate_a_bit(bitmap *bm) {
	for(size_t i = 0;i < bm->size;++i) {
		uint32_t buf = bm->bits[i];
		if(buf != 0xFFFFFFFF) {
			uint32_t p = i * 32;
			for(int j = 0;j < 32;++j) {
				if((buf & (0x1 << j)) != 0) {
					continue;
				}
				buf |= (0x1 << j);
				p += j;
				break;
			}
			return p;
		}
	}
	return 0;
}