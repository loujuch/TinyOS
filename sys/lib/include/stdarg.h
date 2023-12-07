#ifndef _STDARG_H__
#define _STDARG_H__

#include "stdint.h"

typedef void *va_list;

#define va_start(ap, param) (ap = sizeof(param) + ((char*)&param))
#define va_end(ap)          (ap = NULL)
#define va_arg(ap, type)    (ap = ((type *)ap) + 1, *(((type *)ap)-1))

#endif