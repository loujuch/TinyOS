#ifndef _INLINE_MEM_H__
#define _INLINE_MEM_H__

#include "stdint.h"

// 分配[0x500, 0x9FC00)区域内存，分配后永不归还。取出内存容量后才可分配
void *alloc_inline_mem(size_t size);
size_t inline_mem();

#endif