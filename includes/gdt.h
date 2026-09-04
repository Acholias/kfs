/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 21:45:17 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/04 20:21:10 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

# include "kernel.h"

typedef struct s_gdt_entry
{
	u16	limit_low;
	u16	base_low;
	u8	base_middle;
	u8	access_byte;
	u8	flags_limit_hight;
	u8	base_high;
}	__attribute__((packed)) t_gdt_entry;

typedef struct s_gdt_ptr
{
	u16	limit;
	u32	base;
}	__attribute__((packed)) t_gdt_ptr;

# define GDT_NULL_SEGMENT			0
# define GDT_KERNEL_CODE_SEGMENT	1
# define GDT_KERNEL_DATA_SEGMENT	2
# define GDT_KERNEL_STACK_SEGMENT	3
# define GDT_USER_CODE_SEGMENT		4
# define GDT_USER_DATA_SEGMENT		5
# define GDT_USER_STACK_SEGMENT		6

# define GDT_ENTRIES_COUNT			7

# define GDT_BASE_ADRESS			0x00000800

# define GDT_PRESENT				(1 << 7)

# define GDT_RING_0					(0 << 5) // Kernel mode
# define GDT_RING_1					(1 << 5) 
# define GDT_RING_2					(2 << 5)
# define GDT_RING_3					(3 << 5) // User mode

# define GDT_SYSTEM_SEGMENT			(0 << 4) // Segment système (TSS, LD, ...)
# define GDT_CODE_DATA_SEGMENT		(1 << 4) // Segment code/data

# define GDT_DATA_SEGMENT			(0 << 3) // Segment de données
# define GDT_CODE_SEGMENT			(1 << 3) // Segment de code

# define GDT_GROW_UP				(0 << 2)  // Data: grandit vers le haut
# define GDT_GROW_DOWN				(1 << 2)  // Data: grandit vers le bas (stack)
# define GDT_NON_CONFORMING			(0 << 2)  // Code: non-conforming
# define GDT_CONFORMING				(1 << 2)  // Code: conforming

# define GDT_READ_ONLY				(0 << 1)  // Code: non-readable / Data: read-only
# define GDT_READABLE				(1 << 1)  // Code: readable
# define GDT_WRITABLE				(1 << 1)  // Data: writable

# define GDT_NOT_ACCESSED			(0 << 0)
# define GDT_ACCESSED				(1 << 0)

# define GDT_GRANULARITY_4KB		(1 << 7)  // Limite en pages de 4KB

# define GDT_16BIT					(0 << 6)  // Segment 16 bits
# define GDT_32BIT					(1 << 6)  // Segment 32 bits

# define GDT_RESERVED				(0 << 4)

# define GDT_KERNEL_CODE_ACCESS		(GDT_PRESENT | GDT_RING_0 | GDT_CODE_DATA_SEGMENT | GDT_CODE_SEGMENT | GDT_READABLE)

# define GDT_KERNEL_DATA_ACCESS		(GDT_PRESENT | GDT_RING_0 | GDT_CODE_DATA_SEGMENT | GDT_DATA_SEGMENT | GDT_WRITABLE)

# define GDT_USER_CODE_ACCESS		(GDT_PRESENT | GDT_RING_3 | GDT_CODE_DATA_SEGMENT | GDT_CODE_SEGMENT | GDT_READABLE)

# define GDT_USER_DATA_ACCESS		(GDT_PRESENT | GDT_RING_3 | GDT_CODE_DATA_SEGMENT | GDT_DATA_SEGMENT | GDT_WRITABLE)

# define GDT_FLAGS_32BIT				(GDT_GRANULARITY_4KB | GDT_32BIT)

void	gdt_set_gate(u32 num, u32 base, u32 limit, u8 access_byte, u8 flags);
void	gdt_init(void);
void	print_gdt(void);
void	print_stack(void);

#endif
