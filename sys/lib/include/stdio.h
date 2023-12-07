#ifndef _STDIO_H__
#define _STDIO_H__

#include "stdint.h"
#include "stdarg.h"

char putchar(char c);
int puts(const char *s);

int vprintf(const char *format, va_list arg);
int printf(const char *format, ...);

#endif