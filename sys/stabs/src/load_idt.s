[GLOBAL reload_idt]
; [GLOBAL interrupt_exit]

reload_idt:
	mov eax, [esp + 4]
	lidt [eax]
	ret

[EXTERN isr_handler]

%macro DEFINE_ISR 2
[GLOBAL isr%1]
isr%1:
	cli
	%2
	push byte %1

	pusha
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; 返回EOI
	mov al, 0x20
	out 0xa0, al
	out 0x20, al

	call isr_handler

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8

	sti
	iret
%endmacro

%define ERROR nop
%define NOERROR push 0

DEFINE_ISR  0,  NOERROR
DEFINE_ISR  1,  NOERROR
DEFINE_ISR  2,  NOERROR
DEFINE_ISR  3,  NOERROR
DEFINE_ISR  4,  NOERROR
DEFINE_ISR  5,  NOERROR
DEFINE_ISR  6,  NOERROR
DEFINE_ISR  7,  NOERROR
DEFINE_ISR  8,  ERROR
DEFINE_ISR  9,  NOERROR
DEFINE_ISR  10, ERROR
DEFINE_ISR  11, ERROR
DEFINE_ISR  12, ERROR
DEFINE_ISR  13, ERROR
DEFINE_ISR  14, ERROR
DEFINE_ISR  15, NOERROR
DEFINE_ISR  16, NOERROR
DEFINE_ISR  17, NOERROR
DEFINE_ISR  18, NOERROR
DEFINE_ISR  19, NOERROR
DEFINE_ISR  20, NOERROR
DEFINE_ISR  21, NOERROR
DEFINE_ISR  22, NOERROR
DEFINE_ISR  23, NOERROR
DEFINE_ISR  24, NOERROR
DEFINE_ISR  25, NOERROR
DEFINE_ISR  26, NOERROR
DEFINE_ISR  27, NOERROR
DEFINE_ISR  28, NOERROR
DEFINE_ISR  29, NOERROR
DEFINE_ISR  30, NOERROR
DEFINE_ISR  31, NOERROR

DEFINE_ISR  32, NOERROR
DEFINE_ISR  33, NOERROR
DEFINE_ISR  34, NOERROR
DEFINE_ISR  35, NOERROR
DEFINE_ISR  36, NOERROR
DEFINE_ISR  37, NOERROR
DEFINE_ISR  38, NOERROR
DEFINE_ISR  39, NOERROR
DEFINE_ISR  40, NOERROR
DEFINE_ISR  41, NOERROR
DEFINE_ISR  42, NOERROR
DEFINE_ISR  43, NOERROR
DEFINE_ISR  44, NOERROR
DEFINE_ISR  45, NOERROR
DEFINE_ISR  46, NOERROR
DEFINE_ISR  47, NOERROR
DEFINE_ISR  48, NOERROR