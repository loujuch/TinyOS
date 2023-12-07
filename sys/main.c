#include "stdio.h"
#include "assert.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "paging.h"

int main(int argc, char *argv[]) {
	init_paging();

	init_gdt();
	init_idt();

	// init_timer(100);

	register_interrupt_handler(IRQ0_INT_NUM, timer_callback);
	register_interrupt_handler(14, page_fault_handler);

	init_paging();

	enable_interrupt();

	while(1);
	return 0;
}