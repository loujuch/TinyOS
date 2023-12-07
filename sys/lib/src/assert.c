#include "assert.h"

#include "stdio.h"

void panic_spin(char *filename, int line, const char *func, const char *condition) {
	puts("---------ERROR---------\n");
	printf("File: %s  Line: %du  Func: %s  Expr: %s\n", filename, line, func, condition);
	while(1);
}