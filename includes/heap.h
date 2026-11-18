/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 20:29:35 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 10:54:38 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "types.h"
# include "bool.h"
# include "pmm.h"
# include "paging.h"

# define HEAP_MAX_GROW (FRAME_SIZE * 256)
# define MIN_SPLIT	8

typedef struct s_block_header
{
	u32						size;
	bool					free;
	struct s_block_header	*next;
}	t_block_header;

typedef struct s_heap
{
	t_pmm			*pmm;
	t_paging		*paging;
	u32				start;
	u32				break_addr;
	u32				mapped_end;
	u32				limit;
	bool			is_virtual;
	t_block_header	*first_block;
}	t_heap;

void	heap_init(t_heap *heap, t_pmm *pmm, u32 start);
void	heap_init_virtual(t_heap *heap, t_pmm *pmm, t_paging *pg, u32 start, u32 limit);
void	*kbreak(t_heap *heap, int ict);
void	*kmalloc(t_heap *heap, u32 size);
void	kfree(t_heap *heap, void *ptr);
u32		ksize(void *ptr);
void	print_heap(t_heap *heap);
t_heap	*get_heap(void);
t_heap	*get_vheap(void);

#endif
