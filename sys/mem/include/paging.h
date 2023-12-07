#ifndef _PAGING_H__
#define _PAGING_H__

#include "idt.h"

void init_paging();

void page_fault_handler(isr_params_t params);

#endif