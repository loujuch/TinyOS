#ifndef _IDT_H__
#define _IDT_H__

#include "stdint.h"

#define RPL0 0
#define RPL1 1
#define RPL2 2
#define RPL3 3

#define TI_GDT 0
#define TI_LDT 1

#define SELECTOR_K_CODE   ((1 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_DATA   ((2 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_STACK  SELECTOR_K_DATA
#define SELECTOR_K_GS     ((3 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_U_CODE   ((4 << 3) + (TI_GDT << 2) + RPL3)
#define SELECTOR_U_DATA   ((5 << 3) + (TI_GDT << 2) + RPL3)

#define IRQ0_INT_NUM 32
#define IRQ1_INT_NUM 33
#define IRQ2_INT_NUM 34
#define IRQ3_INT_NUM 35
#define IRQ4_INT_NUM 36
#define IRQ5_INT_NUM 37
#define IRQ6_INT_NUM 38
#define IRQ7_INT_NUM 39
#define IRQ8_INT_NUM 40
#define IRQ9_INT_NUM 41
#define IRQ10_INT_NUM 42
#define IRQ11_INT_NUM 43
#define IRQ12_INT_NUM 44
#define IRQ13_INT_NUM 45
#define IRQ14_INT_NUM 46
#define IRQ15_INT_NUM 47

#define SYSCALL_INT_NUM 0x80

struct isr_params_t {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_num;
	uint32_t err_code;
	uint32_t eip, cs, eflags, user_esp, user_ss;
};

typedef struct isr_params_t isr_params_t;
typedef void (*isr_t)(isr_params_t);

void init_idt();

void register_interrupt_handler(uint8_t n, isr_t handler);

void enable_interrupt();
void disable_interrupt();

#endif