#ifndef _STDDEF_H__
#define _STDDEF_H__

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef uintptr_t size_t;

#define NULL 0
#define EOF ((char)-1)

#endif