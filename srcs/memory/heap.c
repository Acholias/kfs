/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/09 20:29:01 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 11:30:24 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/heap.h"
#include "../../includes/libc.h"
#include "../../includes/bool.h"
#include "../../includes/panic.h"
#include "../../includes/paging.h"

void	heap_init(t_heap *heap, t_pmm *pmm, u32 start)
{
	heap->pmm = pmm;
	heap->paging = NULL;
	heap->start = start;
	heap->break_addr = start;
	heap->mapped_end = start;
	heap->limit = 0xFFFFFFFF;
	heap->is_virtual = false;
	heap->first_block = NULL;
}

void	heap_init_virtual(t_heap *heap, t_pmm *pmm, t_paging *pg, u32 start, u32 limit)
{
	heap_init(heap, pmm, start);
	heap->is_virtual = true;
	heap->paging = pg;
	heap->limit = limit;
}

static bool	heap_grow_page(t_heap *heap)
{
	u32	frame;

	if (!heap->is_virtual)
		return (pmm_alloc_frame_at(heap->pmm, heap->mapped_end));
	frame = pmm_alloc_frame(heap->pmm);
	if (frame == 0)
		return (false);
	if (paging_map(heap->paging, heap->pmm, heap->mapped_end, frame, PAGE_RW) != 0)
	{
		pmm_free_frame(heap->pmm, frame);
		return (false);
	}
	return (true);
}

void	*kbreak(t_heap *heap, int ict)
{
	u32	old_break;
	u32	new_break;

	old_break = heap->break_addr;
	new_break = old_break + ict;
	if (ict > 0)
	{
		if ((u32)ict > HEAP_MAX_GROW || new_break < old_break
			|| new_break > heap->limit)
			return (NULL);
		while (heap->mapped_end < new_break)
		{
			if (!heap_grow_page(heap))
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

static void	split_block(t_block_header *block, u32 size)
{
	t_block_header	*rest;

	if (block->size < size + sizeof(t_block_header) + MIN_SPLIT)
		return ;
	rest = (t_block_header *)((u32)block + sizeof(t_block_header) + size);
	rest->size = block->size - size - sizeof(t_block_header);
	rest->free = true;
	rest->next = block->next;
	block->size = size;
	block->next = rest;
}

static void	heap_check(t_heap *heap)
{
	t_block_header	*cur;
	u32				end;

	cur = heap->first_block;
	while (cur)
	{
		end = (u32)cur + sizeof(t_block_header) + cur->size;
		if ((u32)cur < heap->start || end > heap->break_addr)
			kpanic(PANIC_FATAL, "heap corrupted: block out of bounds");
		if (cur->next && (u32)cur->next != end)
			kpanic(PANIC_FATAL, "heap corrupted: broken block chain");
		cur = cur->next;
	}
}

void	*kmalloc(t_heap *heap, u32 size)
{
	t_block_header	*block;

	if (size == 0)
		return (NULL);
	heap_check(heap);
	block = find_free_block(heap, size);
	if (block)
	{
		split_block(block, size);
		block->free = false;
	}
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

static t_block_header	*find_block(t_heap *heap, void *ptr)
{
	t_block_header	*cur;

	cur = heap->first_block;
	while (cur)
	{
		if ((u32)cur + sizeof(t_block_header) == (u32)ptr)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

void	kfree(t_heap *heap, void *ptr)
{
	t_block_header	*block;

	if (!ptr)
		return ;
	heap_check(heap);
	block = find_block(heap, ptr);
	if (!block)
	{
		printk("  ptr: 0x%x\n", (u32)ptr);
		kpanic(PANIC_WARNING, "kfree: invalid pointer");
		return ;
	}
	if (block->free)
	{
		printk("  ptr: 0x%x\n", (u32)ptr);
		kpanic(PANIC_WARNING, "kfree: double free");
		return ;
	}
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

t_heap	*get_vheap(void)
{
	static t_heap	vheap;

	return (&vheap);
}
