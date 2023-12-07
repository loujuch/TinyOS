%include "boot.inc"

SECTION loader vstart=LOADER_BASE_ADDR

loader_entry:
	jmp loader_setup

cursor_row: dd 0
cursor_col: dd 0

gdt_ptr:
	dw GDT_LIMIT
	dd GDT_BASE

times GDT_BASE_ADDR-LOADER_BASE_ADDR-($-$$) db 0

MAIN_MEMORY_SIZE:
	dw 0

GDT_BASE:
	dd 0x00000000
	dd 0x00000000

CODE_DESC:
	dd DESC_CODE_LOW_32
	dd DESC_CODE_HIGH_32

DATA_DESC:
	dd DESC_DATA_LOW_32
	dd DESC_DATA_HIGH_32

VIDEO_DESC:
	dd DESC_VIDEO_LOW_32
	dd DESC_VIDEO_HIGH_32

times 60 dq 0

GDT_SIZE  equ $ - GDT_BASE
GDT_LIMIT equ GDT_SIZE - 1

MEM_STRUCT:
	times (20*128) db 0
MEM_STRUCT_LENGTH:
	dw 0

loader_setup:
	call clear_screen
	call setup_memory_capacity
	call setup_protection_mode
	; 不会到达
	jmp $

setup_memory_capacity:
	mov di, MEM_STRUCT
	mov edx, 0x534d4150
	mov ebx, 0
.get_memory_struct_entry:
	mov eax, 0xe820
	mov ecx, 20
	int 0x15

	add di, cx
	inc word [MEM_STRUCT_LENGTH]
	cmp ebx, 0
	jne .get_memory_struct_entry

	mov edx, 0
	mov cx, [MEM_STRUCT_LENGTH]
	mov ebx, MEM_STRUCT
.find_main_memory_size:
	mov eax, [ebx]
	add eax, [ebx + 8]
	add ebx, 20

	cmp edx, eax
	jge .next_memory_struct
	mov edx, eax
.next_memory_struct:
	loop .find_main_memory_size

	mov [MAIN_MEMORY_SIZE], edx
	ret

setup_protection_mode:
	in al, 0x92
	or al, 0x02
	out 0x92, al

	lgdt [gdt_ptr]

	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp dword SELECTOR_CODE:protection_mode_entry

clear_screen:
	mov byte ah, 0x02
	mov byte dh, 0x00
	mov byte dl, 0x00
	mov byte bh, 0x00

	int 0x10

	mov byte ah, 0x06
	mov byte al, 0x00
	mov byte bh, 0x07

	mov byte cl, 0x00
	mov byte ch, 0x00

	mov byte dl, 0x4f
	mov byte dh, 0x18

	int 0x10
	ret

[bits 32]
protection_mode_entry:
	mov ax, SELECTOR_DATA
	mov ds, ax
	mov es, ax
	mov ss, ax

	mov ax, SELECTOR_VIDEO
	mov gs, ax

	call setup_page
	call init_kernel
	; 不会到达
	jmp $

setup_page:
	call init_page
	call enable_page
	ret

init_page:
	call clear_pde

	mov eax, (PAGE_DIR_PHYISCAL_ADDR + PAGE_SIZE) | PG_US_U | PG_RW_W | PG_P
	mov ebx, PAGE_DIR_PHYISCAL_ADDR
	mov ecx, 255
	mov esi, 0xC00
.create_pde:
	mov [ebx + esi], eax
	add eax, PAGE_SIZE
	add esi, 4
	loop .create_pde

	mov eax, (PAGE_DIR_PHYISCAL_ADDR + PAGE_SIZE) | PG_US_U | PG_RW_W | PG_P
	mov [PAGE_DIR_PHYISCAL_ADDR], eax

	mov eax, PAGE_DIR_PHYISCAL_ADDR | PG_US_U | PG_RW_W | PG_P
	mov [PAGE_DIR_PHYISCAL_ADDR + 1023 * 4], eax

	mov eax, PG_US_U | PG_RW_W | PG_P
	mov ecx, 256
	mov edx, PAGE_DIR_PHYISCAL_ADDR + PAGE_SIZE
.create_pte:
	mov [edx], eax
	add eax, PAGE_SIZE
	add edx, 4
	loop .create_pte
	ret

clear_pde:
	mov eax, PAGE_DIR_PHYISCAL_ADDR
	mov ecx, PAGE_SIZE
.clear_byte:
	mov byte [eax], 0
	inc eax
	loop .clear_byte
	ret

enable_page:
	sgdt [gdt_ptr]

	mov ebx, [gdt_ptr + 2]
	or dword [ebx + 0x18 + 4], 0xC0000000

	add dword [gdt_ptr + 2], 0xC0000000

	mov eax, [esp]
	add esp, 0xC0000000
	mov [esp], eax

	mov eax, PAGE_DIR_PHYISCAL_ADDR
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax

	lgdt [gdt_ptr]

	mov ax, SELECTOR_VIDEO
	mov gs, ax
	ret

init_kernel:
	call allocate_pages_for_kernel
	call load_hd_kernel_image
	call do_load_kernel

	finit

	mov esp, KERNEL_STACK_TOP - 16
	mov ebp, esp

	jmp eax
	ret

; 分配页表项
allocate_pages_for_kernel:
	push eax

	; 分配内核页表项，与系统栈内存连续，一同分配
	mov ecx, KERNEL_BIN_MAX_SIZE
	shr ecx, 12
	push ecx
	; 物理上栈放置在物理内存最顶部
	mov eax, [MAIN_MEMORY_SIZE]
	sub eax, KERNEL_BIN_MAX_SIZE
	push eax
	; push KERNEL_BIN_LOAD_PHYSICAL_ADDR
	push KERNEL_BIN_LOAD_VIRTUAL_ADDR
	call set_pages_mapping
	add esp, 12

	; 分配内核加载后存放内存
	mov ecx, KERNEL_SIZE_MAX
	shr ecx, 12
	push ecx
	push KERNEL_PHYSICAL_ADDR_START
	push KERNEL_VIRTUAL_ADDR_START
	call set_pages_mapping
	add esp, 12

	pop eax
	ret

set_pages_mapping:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx

	mov esi, [ebp + 8]
	mov edi, [ebp + 12]
	mov ecx, [ebp + 16]

	shr esi, 12
	or edi, PG_US_U | PG_RW_W | PG_P
.map_next_page:
	mov [PAGE_TABLES_VIRTUAL_ADDR_START + esi * 4], edi
	add esi, 1
	add edi, PAGE_SIZE
	loop .map_next_page

	pop ebx
	pop edi
	pop esi
	pop ebp
	ret

load_hd_kernel_image:
	mov eax, KERNEL_START_SECTOR
	mov ebx, KERNEL_BIN_LOAD_VIRTUAL_ADDR
	mov ecx, KERNEL_SECTORS
	call read_disk_32
	ret

do_load_kernel:
	xor eax, eax
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx

	mov dx, [KERNEL_BIN_LOAD_VIRTUAL_ADDR + 42]
	mov ebx, [KERNEL_BIN_LOAD_VIRTUAL_ADDR + 28]
	add ebx, KERNEL_BIN_LOAD_VIRTUAL_ADDR
	mov cx, [KERNEL_BIN_LOAD_VIRTUAL_ADDR + 44]

.load_each_segment:
	cmp byte [ebx + 0], 0
	je .next_prgoram_header

	push dword [ebx + 16]
	mov eax, [ebx + 4]
	add eax, KERNEL_BIN_LOAD_VIRTUAL_ADDR
	push eax
	push dword [ebx + 8]
	call memcpy
	add esp, 12

.next_prgoram_header:
	add ebx, edx
	loop .load_each_segment

	mov eax, [KERNEL_BIN_LOAD_VIRTUAL_ADDR + 24]
	ret

memcpy:
	cld
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ecx

	mov edi, [ebp + 8]
	mov esi, [ebp + 12]
	mov ecx, [ebp + 16]
	rep movsb

	pop ecx
	pop edi
	pop esi
	pop ebp
	ret

; EXC 为要求读取块数，每块512B
read_disk_32:
	mov esi, eax
	mov edi, ecx

	mov dx, 0x01f2
	mov al, cl
	out dx, al

	mov eax, esi

	mov dx, 0x1f3
	out dx, al

	shr eax, 8
	mov dx, 0x1f4
	out dx, al

	shr eax, 8
	mov dx, 0x1f5
	out dx, al

	shr eax, 8
	and al, 0x0f

	or al, 0xe0
	mov dx, 0x1f6
	out dx, al

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al
.hd_not_ready:
	nop
	in al, dx
	and al, 0x88
	cmp al, 0x08
	jnz .hd_not_ready

	mov eax, edi
	mov edx, 256
	mul edx
	mov ecx, eax

	mov dx, 0x1f0

.go_on_read_data:
	in ax, dx
	mov [ebx], ax
	add ebx, 2
	loop .go_on_read_data

	ret