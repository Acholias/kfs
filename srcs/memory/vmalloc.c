/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 10:57:09 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 11:02:12 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/vmalloc.h"
#include "../../includes/panic.h"


static bool	is_virtual(t_heap *heap)
{
	if (heap && heap->is_virtual)
		return (true);
	kpanic(PANIC_WARNING, "vmalloc: not a virtual heap");
	return (false);
}

void	*vmalloc(t_heap *vheap, u32 size)
{
	if (!is_virtual(vheap))
		return (NULL);
	return (kmalloc(vheap, size));
}

void	vfree(t_heap *vheap, void *ptr)
{
	if (!is_virtual(vheap))
		return ;
	return (kfree(vheap, ptr));
}

u32	vsize(void *ptr)
{
	return (ksize(ptr));
}

void	*vbreak(t_heap *vheap, int ict)
{
	if (!is_virtual(vheap))
		return (NULL);
	return (kbreak(vheap, ict));
}
