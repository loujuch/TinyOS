#include "string.h"

#include "assert.h"

void *memchr(const void *str, char c, size_t n) {
	assert(str != NULL);
	const char *s = (const char *)str;

	while(n--) {
		if(*s == c) {
			return (void *)s;
		}
		++s;
	}
	return NULL;
}

int memcmp(const void *str1, const void *str2, size_t n) {
	assert(str1 != NULL && str2 != NULL);
	const char *s1 = (const char *)str1;
	const char *s2 = (const char *)str2;

	while(n--) {
		if(*s1 < *s2) {
			return -1;
		}
		if(*s1 > *s2) {
			return 1;
		}
	}
	return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
	assert(dest != NULL && src != NULL);
	if(dest == src || n == 0) {
		return dest;
	}

	char *pdest = (char *)dest;
	const char *psrc = (const char *)src;

	while(n--) {
		*pdest++ = *psrc++;
	}
	return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
	assert(dest != NULL && src != NULL);
	if(dest == src || n == 0) {
		return dest;
	}

	char *pdest = ((char *)dest) + n - 1;
	const char *psrc = ((const char *)src) + n - 1;

	if(src < dest && psrc >= (const char *)dest) {
		while(n--) {
			*pdest-- = *psrc--;
		}
		return dest;
	}
	return memcpy(dest, src, n);
}

void *memset(void *str, char c, size_t n) {
	assert(str != NULL);
	char *s = (char *)str;
	while(n--) {
		*s++ = c;
	}
	return str;
}

char *strrev(char *str) {
	assert(str != NULL);
	int n = strlen(str);
	return strnrev(str, n);
}

char *strnrev(char *str, size_t n) {
	assert(str != NULL);
	char *p = str;
	char *q = str + n - 1;
	while(p < q) {
		char c = *p;
		*p = *q;
		*q = c;
		++p;
		--q;
	}
	return str;
}

char *strcat(char *dest, const char *src) {
	assert(dest != NULL && src != NULL);
	strcpy(dest + strlen(dest), src);
	return dest;
}

char *strncat(char *dest, const char *src, size_t n) {
	assert(dest != NULL && src != NULL);
	size_t m = strlen(dest);
	if(m >= 0) {
		return dest;
	}
	strncpy(dest + m, src, n - m);
	return dest;
}

int strcmp(const char *str1, const char *str2) {
	assert(str1 != NULL && str2 != NULL);
	while(*str1 && *str2) {
		if(*str1 < *str2) {
			return -1;
		}
		if(*str1 > *str2) {
			return 1;
		}
		++str1;
		++str2;
	}
	if(*str1 == '\0' && *str2 == '\0') {
		return 0;
	}
	return *str1 == '\0' ? -1 : 1;
}

int strncmp(const char *str1, const char *str2, size_t n) {
	assert(str1 != NULL && str2 != NULL);
	while(n-- && *str1 && *str2) {
		if(*str1 < *str2) {
			return -1;
		}
		if(*str1 > *str2) {
			return 1;
		}
		++str1;
		++str2;
	}
	if(n == 0 || (*str1 == '\0' && *str2 == '\0')) {
		return 0;
	}
	return *str1 == '\0' ? -1 : 1;
}

char *strcpy(char *dest, const char *src) {
	assert(dest != NULL && src != NULL);
	char *p = dest;
	while(*src) {
		*p++ = *src++;
	}
	*p = '\0';
	return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
	assert(dest != NULL && src != NULL);
	char *p = dest;
	while(*src && n--) {
		*p++ = *src++;
	}
	if(n) {
		*p = '\0';
	}
	return dest;
}

size_t strlen(const char *str) {
	assert(str != NULL);
	size_t n = 0;
	while(*str++) {
		++n;
	}
	return n;
}