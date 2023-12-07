#ifndef _STRING_H__
#define _STRING_H__

#include "stdint.h"

void *memchr(const void *str, char c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *str, char c, size_t n);

char *strrev(char *str);
char *strnrev(char *str, size_t n);

char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);

size_t strlen(const char *str);

#endif