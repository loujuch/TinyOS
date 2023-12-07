#include "stdio.h"

#include "stdarg.h"
#include "monitor.h"
#include "stdlib.h"
#include "assert.h"

char putchar(char c) {
	monitor_put_char_with_color(c, (COLOR_BLACK << 4) | COLOR_WHITE);
	return c;
}

int puts(const char *s) {
	assert(s != NULL);
	int n = 0;
	while(*s) {
		putchar(*s);
		++n;
		++s;
	}
	return n;
}

int vprintf(const char *format, va_list arg) {
	int n = 0;
	int32_t status = 0;
	char type = '\0';
	while(*format) {
		char fc = *format++;
		if(status == 0) {
			if(fc == '%') {
				status = 1;
			} else {
				putchar(fc);
			}
		} else if(status == 1) {
			if(fc == '%') {
				putchar('%');
				status = 0;
			} else if(fc == 'b' || fc == 'w' || fc == 'd' || fc == 'q') {
				type = fc;
				status = 2;
			} else if(fc == 's') {
				char *s = va_arg(arg, char *);
				puts(s);
				status = 0;
			} else if(fc == 'c') {
				char c = va_arg(arg, uint32_t);
				putchar(c);
				status = 0;
			} else if(fc == 'p') {
				uintptr_t ptr = va_arg(arg, uintptr_t);
				char buf[9];
				parseXInt32(buf, ptr);
			} else {
				putchar('%');
				putchar(fc);
				status = 0;
			}
		} else if(status == 2) {
			if(fc == 'o' || fc == 'd' || fc == 'u' || fc == 'x') {
				if(type == 'b') {
					uint8_t i = va_arg(arg, uint32_t);
					char buf[4];
					switch(fc) {
					case 'o':
						parseOInt8(buf, i);
						break;
					case 'd':
						parseDInt8(buf, i);
						break;
					case 'u':
						parseUInt8(buf, i);
						break;
					case 'x':
						parseXInt8(buf, i);
						break;
					default:
						break;
					}
					puts(buf);
				} else if(type == 'w') {
					uint16_t i = va_arg(arg, uint32_t);
					char buf[7];
					switch(fc) {
					case 'o':
						parseOInt16(buf, i);
						break;
					case 'd':
						parseDInt16(buf, i);
						break;
					case 'u':
						parseUInt16(buf, i);
						break;
					case 'x':
						parseXInt16(buf, i);
						break;
					default:
						break;
					}
					puts(buf);
				} else if(type == 'd') {
					uint32_t i = va_arg(arg, uint32_t);
					char buf[12];
					switch(fc) {
					case 'o':
						parseOInt32(buf, i);
						break;
					case 'd':
						parseDInt32(buf, i);
						break;
					case 'u':
						parseUInt32(buf, i);
						break;
					case 'x':
						parseXInt32(buf, i);
						break;
					default:
						break;
					}
					puts(buf);
				} else {
					puts("Unknown data type: ");
					putchar(type);
					putchar('\n');
					assert(0);
				}
			} else {
				putchar('%');
				putchar(type);
				putchar(fc);
			}
			status = 0;
		} else {
			puts("Unknown status: ");
			char buf[12];
			parseDInt32(buf, status);
			puts(buf);
			putchar('\n');
			assert(0);
		}
	}
	return n;
}

int printf(const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	int n = vprintf(format, arg);
	va_end(arg);
	return n;
}