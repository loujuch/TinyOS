#ifndef _MONITOR_H__
#define _MONITOR_H__

#include "stdint.h"

#define COLOR_BLACK     0
#define COLOR_BLUE      1
#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_FUCHSINE  5
#define COLOR_BROWN     6
#define COLOR_WHITE     7

#define COLOR_LIGHT_BLACK     8
#define COLOR_LIGHT_BLUE      9
#define COLOR_LIGHT_GREEN     10
#define COLOR_LIGHT_CYAN      11
#define COLOR_LIGHT_RED       12
#define COLOR_LIGHT_FUCHSINE  13
#define COLOR_LIGHT_BROWN     14
#define COLOR_LIGHT_WHITE     15

void monitor_scroll(uint8_t line);
void monitor_clear();

void monitor_put_char_with_color(char c, uint8_t color);

void set_cursor_position(uint16_t x, uint16_t y);
void move_cursor_position_to_next();
void new_line_cursor_position();

#endif