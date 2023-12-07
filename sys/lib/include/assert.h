#ifndef _ASSERT_H__
#define _ASSERT_H__

void panic_spin(char *filename, int line, const char *func, const char *condition);

#define PANIC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NODEBUG
#define assert(expr)
#else
#define assert(expr) if(!(expr))PANIC(#expr)
#endif

#endif