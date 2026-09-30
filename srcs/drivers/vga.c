/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vga.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:29:48 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 16:15:43 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/vga.h"
#include "../../includes/io.h"

u8	vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return (fg | bg << 4);
}

u16	vga_entry(unsigned char uc, u8 color)
{
	return ((u16)uc | (u16)color << 8);
}

u16	*vga_buffer_ptr(void)
{
	return ((u16 *)VGA_MEMORY);
}

void	vga_set_cursor(u16 row, u16 col)
{
	u16	pos = row * 80 + col;

	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);

	outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}
