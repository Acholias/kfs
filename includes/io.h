#ifndef IO_H
# define IO_H

#include "types.h"

__inline	void	outb(u16 port, u8 val)
{
    __asm__
	("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

#endif
