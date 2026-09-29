/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 10:07:39 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/10 10:40:46 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/pmm.h"
# include "../includes/kernel.h"
# include "../includes/bool.h"

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

// Toutes les fonctions vraiment chiantes pour le scan de la RAM

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
