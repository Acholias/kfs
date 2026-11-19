/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 10:16:19 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/19 10:22:47 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDT_H
# define IDT_H

# include "types.h"

# define IDT_ENTRIES		256
# define IDT_EXEPTIONS		32
# define IDT_KERNEL_CODE	0x08
# define IDT_GATE_INT32		0x8E

typedef struct s_idt_ptr
{
	u16	limit;
	u32	base;
}	__attribute__((packed)) t_idt_ptr;

void	idt_init(void);
void	idt_flush(u32 idt_ptr_adr);

#endif
