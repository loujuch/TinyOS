#include "paging.h"

#include "stdint.h"
#include "bitmap.h"
#include "stdio.h"
#include "string.h"
#include "inline_mem.h"
#include "assert.h"

#define PAGE_SIZE (4 * 1024)
#define KERNEL_PAGE_DIR_PHY 0x100000

struct page_table_entry {
	uint32_t present : 1;
	uint32_t rw : 1;
	uint32_t user : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t unused : 7;
	uint32_t frame : 20;
};
typedef struct page_table_entry pte_t;
typedef pte_t pde_t;

typedef pte_t *pt_t;
typedef pde_t *pd_t;

static uintptr_t physical_mem_size;
static bitmap physical_mem_map;

static pd_t kernel_page_directory = NULL;
static pd_t current_page_directory = NULL;

static uintptr_t allocate_physical() {
	uint32_t phy = allocate_a_bit(&physical_mem_map);
	return (uintptr_t)(phy * PAGE_SIZE);
}

static void reload_page_directory(pd_t dir) {
	current_page_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(current_page_directory));
}

static void map_page_with_frame(uintptr_t virtual) {

}

void init_paging() {
	physical_mem_size = *((uintptr_t *)0xC0000600);

	physical_mem_map.size = physical_mem_size / PAGE_SIZE / 8;
	physical_mem_map.bits = (uint32_t *)alloc_inline_mem(physical_mem_map.size);

	assert(physical_mem_map.bits != NULL);

	memset(physical_mem_map.bits, 0, physical_mem_map.size * sizeof(uint32_t));

	// 已分配物理空间：(0-1MB)系统保留，(1MB-2MB)页表，(2MB-3MB)内核，(MAX-1MB, MAX)栈
	for(size_t i = 0;i < (3 * 1024 * 1024) / PAGE_SIZE / 8;++i) {
		physical_mem_map.bits[i] = 0xFFFFFFFF;
	}
	for(size_t i = (physical_mem_size - 1024 * 1024) / PAGE_SIZE / 8;
		i < physical_mem_map.size;++i) {
		physical_mem_map.bits[i] = 0xFFFFFFFF;
	}

	kernel_page_directory = (pd_t)KERNEL_PAGE_DIR_PHY;
	current_page_directory = kernel_page_directory;
}

void page_fault_handler(isr_params_t params) {
	uintptr_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	map_page_with_frame(faulting_address);
}