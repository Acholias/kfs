/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 20:29:35 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/09 21:12:14 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "types.h"
# include "bool.h"
# include "pmm.h"
# include "kernel.h"

# define HEAP_MAX_GROW (FRAME_SIZE * 256)

typedef struct s_block_header
{
	u32						size;
	bool					free;
	struct s_block_header	*next;
}	t_block_header;

typedef struct s_heap
{
	t_pmm			*pmm;
	u32				start;
	u32				break_addr;
	u32				mapped_end;
	t_block_header	*first_block;
}	t_heap;

void	heap_init(t_heap *heap, t_pmm *pmm, u32 start);
void	*kbreak(t_heap *heap, int ict);
void	*kmalloc(t_heap *heap, u32 size);
void	kfree(t_heap *heap, void *ptr);
u32		ksize(void *ptr);
void	print_heap(t_heap *heap);
t_heap	*get_heap(void);

#endif
