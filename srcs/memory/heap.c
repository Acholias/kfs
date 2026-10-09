/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 20:29:01 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/09 21:29:43 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/heap.h"

void	heap_init(t_heap *heap, t_pmm *pmm, u32 start)
{
	heap->pmm = pmm;
	heap->start = start;
	heap->break_addr = start;
	heap->mapped_end = start;
	heap->first_block = NULL;
}

void	*kbreak(t_heap *heap, int ict)
{
	u32	old_break;
	u32	new_break;
	u32	frame;

	old_break = heap->break_addr;
	new_break = old_break + ict;
	if (ict > 0)
	{
		if ((u32)ict > HEAP_MAX_GROW)
			return (NULL);
		while (heap->mapped_end < new_break)
		{
			frame = pmm_alloc_frame(heap->pmm);
			if (frame == 0)
				return (NULL);
			heap->mapped_end += FRAME_SIZE;
		}
	}
	heap->break_addr = new_break;
	return ((void *)old_break);
}

static t_block_header	*find_free_block(t_heap *heap, u32 size)
{
	t_block_header	*current;

	current = heap->first_block;
	while (current)
	{
		if (current->free && current->size >= size)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static t_block_header	*extend_heap(t_heap *heap, u32 size)
{
	t_block_header	*block;
	t_block_header	*last;
	u32				total;

	total = sizeof(t_block_header) + size;
	block = (t_block_header *)kbreak(heap, total);
	if (!block)
		return (NULL);
	block->size = size;
	block->free = false;
	block->next = NULL;

	if (!heap->first_block)
		heap->first_block = block;
	else
	{
		last = heap->first_block;
		while (last->next)
			last = last->next;
		last->next = block;
	}
	return (block);
}

void	*kmalloc(t_heap *heap, u32 size)
{
	t_block_header	*block;

	if (size == 0)
		return (NULL);

	block = find_free_block(heap, size);
	if (block)
		block->free = false;
	else
		block = extend_heap(heap, size);
	if (!block)
		return (NULL);
	return ((void *)((u32)block + sizeof(t_block_header)));
}

static void	coalesce(t_heap *heap)
{
	t_block_header	*current;
	u32				current_end;

	current = heap->first_block;
	while (current && current->next)
	{
		current_end = (u32)current + sizeof(t_block_header) + current->size;
		if (current->free && current->next->free && current_end == (u32)current->next)
		{
			current->size += sizeof(t_block_header) + current->next->size;
			current->next = current->next->next;
		}
		else
			current = current->next;
	}
}

void	kfree(t_heap *heap, void *ptr)
{
	t_block_header	*block;

	if (!ptr)
		return ;
	block = (t_block_header *)((u32)ptr - sizeof(t_block_header));
	block->free = true;
	coalesce(heap);
}

u32	ksize(void *ptr)
{
	t_block_header	*block;

	if (!ptr)
		return (0);
	block = (t_block_header *)((u32)ptr - sizeof(t_block_header));
	return (block->size);
}

void	print_heap(t_heap *heap)
{
	t_block_header	*current;
	u32				total_used;
	u32				total_free;
	u32				blocks;

	current = heap->first_block;
	total_used = 0;
	total_free = 0;
	blocks = 0;
	while (current)
	{
		if (current->free)
			total_free += current->size;
		else
			total_used += current->size;
		blocks++;
		current = current->next;
	}
	printk("[Heap] start=0x%x break=0x%x mapped=0x%x\n",
		heap->start, heap->break_addr, heap->mapped_end);
	printk("Blocks: %d | used: %d o | free: %d o\n", blocks, total_used, total_free);
}

t_heap	*get_heap(void)
{
	static	t_heap	heap;

	return (&heap);
}
