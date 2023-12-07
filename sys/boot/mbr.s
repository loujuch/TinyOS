%include "boot.inc"

SECTION mbr vstart=MBR_BASE_ADDR

mbr_entry:
	; 初始化段寄存器
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	; 初始化栈
	mov sp, MBR_LOADER_STACK_ADDR

	call load_loader_img

	jmp LOADER_BASE_ADDR

load_loader_img:
	mov eax, LOADER_START_SECTOR
	mov bx, LOADER_BASE_ADDR
	mov cx, LOADER_SIZE

	call read_disk

	ret

read_disk:
	mov esi, eax
	mov di, cx

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
	out dx,al

	shr eax, 8
	and al, 0x0f

	or al, 0xe0
	mov dx, 0x1f6
	out dx, al

	mov dx, 0x1f7
	mov al, 0x20
	out dx, al

.not_ready:
	nop
	in al, dx
	and al, 0x88
	cmp al, 0x08
	jnz .not_ready

	mov ax, di
	mov dx, 256
	mul dx
	mov cx, ax

	mov dx, 0x1f0

.go_on_read:
	in ax, dx
	mov [bx], ax
	add bx, 2
	loop .go_on_read
	ret

times 510-($-$$) db 0
db 0x55, 0xaa