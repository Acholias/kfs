/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 13:14:29 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 13:35:53 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"
#include "../../includes/paging.h"

void	paging_init(t_paging *pg, t_pmm *pmm)
{
	u32	mem_bytes;
	u32	dir_phys;
	u32	table_phys;
	u32	t;
	u32	i;

	mem_bytes = pmm_total_frames(pmm) * FRAME_SIZE;
	pg->n_tables = (mem_bytes + TABLE_COVERAGE - 1) / TABLE_COVERAGE;

	dir_phys = pmm_alloc_frame(pmm);
	pg->directory = (u32 *)dir_phys;
	ft_memset(pg->directory, 0, PD_ENTRIES * sizeof(u32));

	t = 0;
	while (t < pg->n_tables)
	{
		table_phys = pmm_alloc_frame(pmm);
		ft_memset((void *)table_phys, 0, PT_ENTRIES * sizeof(u32));

		i = 0;
		while (i < PT_ENTRIES)
		{
			((u32 *)table_phys)[i] = (t * TABLE_COVERAGE + i * FRAME_SIZE)
				| PAGE_PRESENT | PAGE_RW;
			i++;
		}
		pg->directory[t] = table_phys | PAGE_PRESENT | PAGE_RW;
		t++;
	}
}

void	paging_enable(t_paging *pg)
{
	asm volatile (
		"mov %0, %%cr3\n"
		"mov %%cr0, %%eax\n"
		"or $0x80000000, %%eax\n"
		"mov %%eax, %%cr0\n"
		:
		: "r"((u32)pg->directory)
		: "eax"
	);
}

bool	paging_is_enabled(void)
{
	u32	cr0;

	asm volatile ("mov %%cr0, %0" : "=r"(cr0));
	return ((cr0 & 0x80000000) != 0);
}

void	print_paging(t_paging *pg)
{
	printk("[Paging] %s\n", paging_is_enabled() ? "ENABLED" : "disabled");
	printk("Directory phys: 0x%x\n", (u32)pg->directory);
	printk("Tables: %d (identity-mapped 0x0 -> 0x%x)\n",
		pg->n_tables, pg->n_tables * TABLE_COVERAGE);
}
