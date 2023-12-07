#include "gdt.h"

#include "stdint.h"

#define FLAG_G_4K  (1 << 3)
#define FLAG_D_32  (1 << 2)

#define DESC_P     (1 << 7)

#define DESC_DPL_0   (0 << 5)
#define DESC_DPL_1   (1 << 5)
#define DESC_DPL_2   (2 << 5)
#define DESC_DPL_3   (3 << 5)

#define DESC_S_CODE   (1 << 4)
#define DESC_S_DATA   (1 << 4)
#define DESC_S_SYS    (0 << 4)

#define DESC_TYPE_CODE  0x8
#define DESC_TYPE_DATA  0x2
#define DESC_TYPE_TSS   0x9

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr gdt_ptr_t;

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  attributes;
	uint8_t  base_high;
} __attribute__((packed));
typedef struct gdt_entry gdt_entry_t;

static gdt_entry_t gdt[8];

extern void load_gdt(gdt_ptr_t *gdt);

static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit,
	uint8_t access, uint8_t flags) {
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].access = access;
	gdt[num].attributes = (limit >> 16) & 0x0F;
	gdt[num].attributes |= ((flags << 4) & 0xF0);
	gdt[num].base_high = (base >> 24) & 0xFF;
}

static void refresh_gdt(gdt_ptr_t *gdt) {
	load_gdt(gdt);
}

void init_gdt() {
	gdt_ptr_t gdt_ptr;
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 8) - 1;
	gdt_ptr.base = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFF, DESC_P | DESC_DPL_0 | DESC_S_CODE | DESC_TYPE_CODE,
		FLAG_G_4K | FLAG_D_32);
	gdt_set_gate(2, 0, 0xFFFFF, DESC_P | DESC_DPL_0 | DESC_S_DATA | DESC_TYPE_DATA,
		FLAG_G_4K | FLAG_D_32);
	gdt_set_gate(3, 0, 7, DESC_P | DESC_DPL_0 | DESC_S_DATA | DESC_TYPE_DATA,
		FLAG_G_4K | FLAG_D_32);
	// gdt_set_gate(4, 0, 0xBFFFF, DESC_P | DESC_DPL_3 | DESC_S_CODE | DESC_TYPE_CODE,
		// FLAG_G_4K | FLAG_D_32);
	// gdt_set_gate(5, 0, 0xBFFFF, DESC_P | DESC_DPL_3 | DESC_S_DATA | DESC_TYPE_DATA,
		// FLAG_G_4K | FLAG_D_32);
	gdt_set_gate(4, 0, 0, 0, 0);
	gdt_set_gate(5, 0, 0, 0, 0);
	gdt_set_gate(6, 0, 0, 0, 0);
	gdt_set_gate(7, 0, 0, 0, 0);

	refresh_gdt(&gdt_ptr);
}
