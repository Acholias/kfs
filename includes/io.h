#ifndef IO_H
# define IO_H

# include "types.h"

void	outb(uint16_t port, uint8_t val);
uint8_t	inb(uint16_t port);
void	outw(uint16_t port, uint16_t value);

#endif
