SYS_DIR=sys
OBJ_DIR=obj
BIN_DIR=bin
USER_DIR=user

SUB_DIRS = \
	lib/ \
	util/ \
	stabs/ \
	mem/ \
	dev/

OBJ_DIRS = $(addprefix ./${OBJ_DIR}/, ${SUB_DIRS})

OBJS_C = \
	$(OBJ_DIR)/main.o \
	$(OBJ_DIR)/lib/stdio.o \
	$(OBJ_DIR)/lib/string.o \
	$(OBJ_DIR)/lib/stdlib.o \
	$(OBJ_DIR)/lib/assert.o \
	$(OBJ_DIR)/util/io.o \
	$(OBJ_DIR)/util/monitor.o \
	$(OBJ_DIR)/stabs/gdt.o \
	$(OBJ_DIR)/stabs/load_gdt.o \
	$(OBJ_DIR)/stabs/idt.o \
	$(OBJ_DIR)/stabs/load_idt.o \
	$(OBJ_DIR)/mem/paging.o \
	$(OBJ_DIR)/mem/inline_mem.o \
	$(OBJ_DIR)/mem/bitmap.o \
	$(OBJ_DIR)/dev/timer.o

OBJS_ASM = \

CC=gcc -Werror
ASM=nasm
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -no-pie -fno-pic
IFLAGS=$(addprefix -I,$(addsuffix include, $(addprefix ./${SYS_DIR}/,$(SUB_DIRS))))
LDFLAGS=-m elf_i386 -Tlink.ld
ASFLAGS=-felf

all: image

prepare: ${SYS_DIR}/*
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p ${OBJ_DIRS}

image: prepare mbr loader kernel
	rm -rf os.img && bximage -hd -mode="flat" -size=3 -q os.img 1> /dev/null
	dd if=$(BIN_DIR)/mbr of=os.img bs=512 count=1 seek=0 conv=notrunc
	dd if=$(BIN_DIR)/loader of=os.img bs=512 count=8 seek=1 conv=notrunc
	dd if=$(BIN_DIR)/kernel of=os.img bs=512 count=2048 seek=9 conv=notrunc

mbr: $(SYS_DIR)/boot/mbr.s
	$(ASM) -I sys/boot/include -o $(BIN_DIR)/mbr $<

loader: $(SYS_DIR)/boot/loader.s
	$(ASM) -I sys/boot/include -o $(BIN_DIR)/loader $<

kernel: ${OBJS_C} ${OBJS_ASM} link.ld
	ld $(LDFLAGS) -o bin/kernel ${OBJS_C} ${OBJS_ASM}

$(OBJ_DIR)/%.o: $(SYS_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/lib/%.o: $(SYS_DIR)/lib/src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/util/%.o: $(SYS_DIR)/util/src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/stabs/%.o: $(SYS_DIR)/stabs/src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/stabs/%.o: $(SYS_DIR)/stabs/src/%.s
	$(ASM) $(ASFLAGS) $< -o $@

$(OBJ_DIR)/dev/%.o: $(SYS_DIR)/dev/src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(OBJ_DIR)/mem/%.o: $(SYS_DIR)/mem/src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -rf ${OBJ_DIR} ${BIN_DIR} os.img bochsout.txt kernel_dump.txt
