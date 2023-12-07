#include "idt.h"

#include "io.h"
#include "gdt.h"
#include "string.h"
#include "assert.h"
#include "stdio.h"

#define IDT_GATE_P     1
#define IDT_GATE_DPL0  0
#define IDT_GATE_DPL3  3
#define IDT_GATE_32_TYPE  0xE

#define IDT_GATE_ATTR_DPL0 ((IDT_GATE_P << 7) + (IDT_GATE_DPL0 << 5) + IDT_GATE_32_TYPE)
#define IDT_GATE_ATTR_DPL3 ((IDT_GATE_P << 7) + (IDT_GATE_DPL3 << 5) + IDT_GATE_32_TYPE)

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr48();

struct idt_ptr_t {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr_t idt_ptr_t;

extern void reload_idt(idt_ptr_t *);

struct idt_entry_t {
	uint16_t handler_addr_low;
	uint16_t sel;
	uint8_t always0;
	uint8_t attrs;
	uint16_t handler_addr_high;
} __attribute__((packed));
typedef struct idt_entry_t idt_entry_t;

static idt_entry_t idt[256];
static isr_t isr[256];

static void set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t attrs) {
	idt[num].handler_addr_low = base & 0xFFFF;
	idt[num].handler_addr_high = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].attrs = attrs;
}

static void init_pic() {
	// master
	outb(0x20, 0x11);
	outb(0x21, 0x20);
	outb(0x21, 0x04);
	outb(0x21, 0x01);
	// slave
	outb(0xA0, 0x11);
	outb(0xA1, 0x28);
	outb(0xA1, 0x02);
	outb(0xA1, 0x01);
	// unmask all irqs
	outb(0x21, 0xfe);
	outb(0xA1, 0xff);
	// outb(0x21, 0x0);
	// outb(0xA1, 0x0);
}

void isr_handler(isr_params_t params) {
	uint32_t int_num = params.int_num;

	if(isr[int_num] != NULL) {
		isr[int_num](params);
	} else {
		printf("Interrupt is not register: %dd\n", int_num);
		uint32_t errno = params.err_code;
		if(errno == 0) {
			printf("  No Error Code\n");
		} else {
			if((errno & 0x1) == 0) {
				printf("  EXT: true ");
			} else {
				printf("  EXT: false ");
			}
			if(((errno >> 1) & 0x3) == 1) {
				printf("IDT ");
			} else {
				if(errno & (0x1 << 3)) {
					printf("LDT ");
				} else {
					printf("GDT ");
				}
			}
			printf("Selector Index: %dx\n", errno >> 4);
		}
		printf("  eax: %dx, ebx: %dx, ecx: %dx, edx: %dx\n",
			params.eax, params.ebx, params.ecx, params.edx);
		printf("  esp: %dx, ebp: %dx, esi: %dx, edi: %dx\n",
			params.esp, params.ebp, params.esi, params.esi);
		printf("  cs: %dx, eip: %dx, ds: %dx\n", params.cs, params.eip, params.ds);
		assert(0);
	}
}

void init_idt() {
	idt_ptr_t idt_ptr;
	idt_ptr.base = (uint32_t)idt;
	idt_ptr.limit = sizeof(idt) - 1;

	memset(idt, 0, sizeof(idt));
	memset(isr, 0, sizeof(isr));

	set_idt_gate(0, (uint32_t)isr0, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(1, (uint32_t)isr1, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(2, (uint32_t)isr2, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(3, (uint32_t)isr3, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(4, (uint32_t)isr4, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(5, (uint32_t)isr5, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(6, (uint32_t)isr6, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(7, (uint32_t)isr7, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(8, (uint32_t)isr8, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(9, (uint32_t)isr9, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(10, (uint32_t)isr10, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(11, (uint32_t)isr11, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(12, (uint32_t)isr12, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(13, (uint32_t)isr13, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(14, (uint32_t)isr14, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(15, (uint32_t)isr15, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(16, (uint32_t)isr16, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(17, (uint32_t)isr17, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(18, (uint32_t)isr18, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(19, (uint32_t)isr19, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(20, (uint32_t)isr20, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(21, (uint32_t)isr21, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(22, (uint32_t)isr22, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(23, (uint32_t)isr23, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(24, (uint32_t)isr24, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(25, (uint32_t)isr25, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(26, (uint32_t)isr26, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(27, (uint32_t)isr27, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(28, (uint32_t)isr28, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(29, (uint32_t)isr29, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(30, (uint32_t)isr30, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(31, (uint32_t)isr31, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);

	set_idt_gate(32, (uint32_t)isr32, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(33, (uint32_t)isr33, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(34, (uint32_t)isr34, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(35, (uint32_t)isr35, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(36, (uint32_t)isr36, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(37, (uint32_t)isr37, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(38, (uint32_t)isr38, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(39, (uint32_t)isr39, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(40, (uint32_t)isr40, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(41, (uint32_t)isr41, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(42, (uint32_t)isr42, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(43, (uint32_t)isr43, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(44, (uint32_t)isr44, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(45, (uint32_t)isr45, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(46, (uint32_t)isr46, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);
	set_idt_gate(47, (uint32_t)isr47, SELECTOR_K_CODE, IDT_GATE_ATTR_DPL3);

	init_pic();
	reload_idt(&idt_ptr);
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
	isr[n] = handler;
}

void enable_interrupt() {
	asm volatile ("sti");
}

void disable_interrupt() {
	asm volatile ("cli");
}