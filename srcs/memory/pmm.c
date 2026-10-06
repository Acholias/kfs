/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 10:07:39 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 13:10:50 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/pmm.h"
# include "../../includes/kernel.h"
# include "../../includes/bool.h"

static void	set_frame(t_pmm *pmm, u32 frame)
{
	pmm->bitmap[frame / 8] |= (1 << (frame % 8));
}

static void	clear_frame(t_pmm *pmm, u32 frame)
{
	pmm->bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static bool	test_frame(t_pmm *pmm, u32 frame)
{
	return ((pmm->bitmap[frame / 8] & (1 << (frame % 8))) != 0);
}

void	pmm_mark_used(t_pmm *pmm, u32 addr, u32 size)
{
	u32	start = addr / FRAME_SIZE;
	u32	end = (addr + size + FRAME_SIZE - 1) / FRAME_SIZE;


	for (u32 f = start; f < end && f < pmm->total_frames; f++)
		set_frame(pmm, f);
}

void	pmm_mark_free(t_pmm *pmm, u32 addr, u32 size)
{
	u32	start = addr / FRAME_SIZE;
	u32 end	= (addr + size) / FRAME_SIZE;

	for (u32 f = start; f < end && f < pmm->total_frames; f++)
		clear_frame(pmm, f);
}

static void	scan_mmap(t_pmm *pmm, t_multiboot_info *mbi)
{
	t_mmap_entry	*entry;
	u32				end;

	entry = (t_mmap_entry *)mbi->mmap_addr;
	end = mbi->mmap_addr + mbi->mmap_length;
	while ((u32)entry < end)
	{
		if (entry->type == MMAP_TYPE_AVAILABLE)
			pmm_mark_free(pmm, (u32)entry->addr, (u32)entry->len);
		entry = (t_mmap_entry *)((u32)entry + entry->size + 4);
	}
}

void	pmm_init(t_pmm *pmm, t_multiboot_info *mbi, u32 kernel_end)
{
	u32	mem_bytes;

	if (mbi->flags & (1 << 0))
		mem_bytes = (mbi->mem_lower + mbi->mem_upper) * 1024;
	else
		mem_bytes = 128 * 1024 * 1024;

	pmm->total_frames = mem_bytes / FRAME_SIZE;
	pmm->bitmap_size = (pmm->total_frames + 7) / 8;
	pmm->bitmap = (u8 *)((kernel_end + FRAME_SIZE - 1) & ~(FRAME_SIZE - 1));

	ft_memset(pmm->bitmap, 0xFF, pmm->bitmap_size);

	if (mbi->flags & MULTIBOOT_MMAP_FLAG)
		scan_mmap(pmm, mbi);
	else
		pmm_mark_free(pmm, 0x100000, mem_bytes - 0x100000);

	pmm_mark_used(pmm, 0, 0x100000);
	pmm_mark_used(pmm, 0x100000, kernel_end - 0x100000);
	pmm_mark_used(pmm, (u32)pmm->bitmap, pmm->bitmap_size);
}

u32	pmm_total_frames(t_pmm *pmm)
{
	return (pmm->total_frames);
}

u32	pmm_alloc_frame(t_pmm *pmm)
{
	for (u32 f = 0; f < pmm->total_frames; f++)
	{
		if (!test_frame(pmm, f))
		{
			set_frame(pmm, f);
			return (f * FRAME_SIZE);
		}
	}
	return (0);
}

void	pmm_free_frame(t_pmm *pmm, u32 frame_addr)
{
	clear_frame(pmm, frame_addr / FRAME_SIZE);
}

void	print_pmm(t_pmm *pmm)
{
	u32	used;

	used = 0;
	for (u32 f = 0; f <pmm->total_frames; f++)
	{
		if (test_frame(pmm, f))
			used++;
	}
	printk("[PMM] %d/%d frames used (%d ko libres)\n", used, pmm->total_frames, (pmm->total_frames - used) * FRAME_SIZE / 1024);
}
