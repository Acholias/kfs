/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiboot.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 09:45:15 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/10 10:40:57 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MULTIBOOT_H
# define MULTIBOOT_H

# include "types.h"

# define MULTIBOOT_MAGIC		0x2BADB002
# define MULTIBOOT_MMAP_FLAG	(1 << 6)
# define MMAP_TYPE_AVAILABLE	1

typedef struct s_multiboot_info
{
	u32	flags;
	u32	mem_lower;
	u32	mem_upper;
	u32	boot_device;
	u32	cmdline;
	u32	mods_count;
	u32	mods_addr;
	u32	syms[4];
	u32	mmap_length;
	u32	mmap_addr;
}	t_multiboot_info;

typedef struct s_mmap_entry
{
	u32	size;
	u64	addr;
	u64	len;
	u32	type;
}	__attribute__((packed)) t_mmap_entry;

#endif
