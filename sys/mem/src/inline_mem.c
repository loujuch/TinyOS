#include "inline_mem.h"

static size_t top = 0xC0000500;

void *alloc_inline_mem(size_t size) {
	void *p = NULL;
	if(top + size <= 0xC009FC00) {
		p = (void *)top;
		top += size;
	}
	return p;
}

size_t inline_mem() {
	return 0xC009FC00 - top;
}