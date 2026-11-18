/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memlayout.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 10:36:40 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 10:40:45 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMLAYOUT_H
# define MEMLAYOUT_H

# include "types.h"

# define KERNEL_SPACE_START	0x00000000
# define KERNEL_SPACE_END	0x40000000	// exclu : 1 Go, identity map, kmalloc
# define USER_SPACE_START	0x40000000
# define USER_SPACE_END		0xC0000000	// exclu : 2 Go, pages avec PAGE_USER
# define VMALLOC_START		0xC0000000	// kernel, mémoire virtuelle (vmalloc)
# define VMALLOC_END		0xD0000000	// exclu : 256 Mo

typedef	enum e_space
{
	SPACE_KERNEL,
	SPACE_USER,
}	t_space;

static inline t_space	memory_space_of(u32 address)
{
	if (address >= USER_SPACE_START && address < USER_SPACE_END)
		return (SPACE_USER);
	return (SPACE_KERNEL);
}

#endif
