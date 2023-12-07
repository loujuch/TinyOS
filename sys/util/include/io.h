#ifndef _IO_H__
#define _IO_H__

#include "stdint.h"

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

#endif