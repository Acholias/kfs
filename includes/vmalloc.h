/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vmalloc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/18 10:54:56 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 10:56:53 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VMALLOC_H
# define VMALLOC_H

# include "heap.h"

void	*vmalloc(t_heap *vheap, u32 size);
void	vfree(t_heap *vheap, void *ptr);
u32		vsize(void *ptr);
void	*vbreak(t_heap *vheap, int ict);

#endif
