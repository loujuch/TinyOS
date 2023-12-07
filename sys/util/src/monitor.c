#include "monitor.h"

#include "stdint.h"
#include "io.h"

#define NULL_FILL (COLOR_BLACK << 12) | (COLOR_BLACK << 8) | '\n'

uint16_t cursor_x = 0;
uint16_t cursor_y = 0;

uint16_t *video_memory = (uint16_t *)0xC00B8000;

static uint16_t cursor_position() {
	return cursor_y * 80 + cursor_x;;
}

static void flush_cursor_position() {
	uint16_t cursorLocation = cursor_position();
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}

void monitor_scroll(uint8_t line) {
	if(line == 0) {
		return;
	}
	if(line >= 25) {
		monitor_clear();
		return;
	}
	int index = 0;
	for(int i = 80 * line;i < 80 * 25;++i) {
		video_memory[index++] = video_memory[i];
	}
	for(;index < 80 * 25;++index) {
		video_memory[index] = NULL_FILL;
	}
	cursor_y -= line;
	flush_cursor_position();
}

void monitor_clear() {
	for(int i = 0;i < 80 * 25;++i) {
		video_memory[i] = NULL_FILL;
	}
	cursor_x = 0;
	cursor_y = 0;
	flush_cursor_position();
}

void monitor_put_char_with_color(char c, uint8_t color) {
	if(c == '\n') {
		new_line_cursor_position();
		return;
	}
	uint16_t video_ceil = (((uint16_t)color) << 8) | c;
	video_memory[cursor_position()] = video_ceil;
	move_cursor_position_to_next();
}

void set_cursor_position(uint16_t x, uint16_t y) {
	cursor_x = x >= 80 ? 79 : x;
	cursor_y = y >= 25 ? 24 : y;
	flush_cursor_position();
}

void move_cursor_position_to_next() {
	if(cursor_x == 79 && cursor_y == 24) {
		monitor_scroll(1);
		cursor_x = 0;
		cursor_y = 24;
	} else if(cursor_x == 79) {
		cursor_x = 0;
		++cursor_y;
	} else {
		++cursor_x;
	}
	flush_cursor_position();
}

void new_line_cursor_position() {
	if(cursor_y >= 24) {
		monitor_scroll(1);
		cursor_y = 24;
	} else {
		++cursor_y;
	}
	cursor_x = 0;
	flush_cursor_position();
}