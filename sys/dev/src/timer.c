#include "timer.h"

#include "io.h"
#include "stdio.h"

static uint32_t tick = 0;

void timer_callback(isr_params_t regs) {
	printf("Tick: %dd\n", tick++);
}

void init_timer(uint32_t frequency) {
	tick = 0;
	uint32_t divisor = 1193180 / frequency;

	outb(0x43, 0x36);

	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}