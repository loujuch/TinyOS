#ifndef _TIMER_H__
#define _TIMER_H__

#include "stdint.h"
#include "idt.h"

void init_timer(uint32_t frequency);
void timer_callback(isr_params_t regs);

#endif