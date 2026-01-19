/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 22:12:41 by lumugot           #+#    #+#             */
/*   Updated: 2026/01/19 22:28:00 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/gdt.h"

t_gdt_entry	*gdt = (t_gdt_entry *)GDT_BASE_ADRESS;
t_gdt_entry	gdt_ptr;

extern void	gdt_flush(u32 gdt_ptr_addr);

void	gdt_set_gate(u32 num, u32 base, u32 limit, u8 access_byte, u8 flags)
{
	if (num >= GDT_ENTRIES_COUNT)
		return ;

	// Base du segment (adress 32 bits divisé en 3 parties)
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0XFF;

	// Setup la limite du sgment (20 bits divisé en 2 parties)
	gdt[num].limit_low = (limit & 0xFFFF);

	// Combiner les 4 bits hauts de la limite + les 4 bits de flags
	gdt[num].flags_limit_hight = ((limit >> 16) & 0xFF) | (flags & 0xF0);

	// configurer l'access byte
	gdt[num].access_byte = access_byte;
}

void	gdt_init(void)
{
	// Configuration du pointeur gdt
	gdt_ptr.limit = (sizeof(t_gdt_entry) * GDT_ENTRIES_COUNT) - 1;
	gdt_ptr.base  = (u32)gdt;

	gdt_set_gate(GDT_NULL_SEGMENT, 0, 0, 0 ,0);
}
