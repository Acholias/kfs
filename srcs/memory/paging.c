/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 13:14:29 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 11:30:05 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"
#include "../../includes/paging.h"
#include "../../includes/memlayout.h"

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

t_paging	*get_paging(void)
{
	static t_paging	paging;

	return (&paging);
}

static u32	fix_flags(u32 virt, u32 flags)
{
	flags &= 0xFFF;
	if (memory_space_of(virt) == SPACE_USER)
		return (flags | PAGE_USER);
	return (flags & ~PAGE_USER);
}

static void	flush_tlb(void)
{
	asm volatile ("mov %%cr3, %%eax\n\t"
		"mov %%eax, %%cr3" : : : "eax", "memory");
}

int	paging_map(t_paging *pg, t_pmm *pmm, u32 virt, u32 phys, u32 flags)
{
	u32	pd_idx;
	u32	pt_idx;
	u32	table_phys;
	u32	*table;

	if ((virt % FRAME_SIZE) || (phys % FRAME_SIZE))
		return (-1);
	flags = fix_flags(virt, flags);
	pd_idx = virt >> 22;
	pt_idx = (virt >> 12) & 0x3FF;
	if (!(pg->directory[pd_idx] & PAGE_PRESENT))
	{
		table_phys = pmm_alloc_frame(pmm);
		if (table_phys == 0)
			return (-2);
		ft_memset((void *)table_phys, 0, FRAME_SIZE);
		pg->directory[pd_idx] = table_phys | PAGE_PRESENT | PAGE_RW
			| (flags & PAGE_USER);
	}
	table = (u32 *)(pg->directory[pd_idx] & ~0xFFF);
	if (table[pt_idx] & PAGE_PRESENT)
		return (-3);
	table[pt_idx] = phys | flags | PAGE_PRESENT;
	flush_tlb();
	return (0);
}

int	paging_unmap(t_paging *pg, u32 virt)
{
	u32	*table;
	u32	pt_idx;

	if (!(pg->directory[virt >> 22] & PAGE_PRESENT))
		return (-1);
	table = (u32 *)(pg->directory[virt >> 22] & ~0xFFF);
	pt_idx = (virt >> 12) & 0x3FF;
	if (!(table[pt_idx] & PAGE_PRESENT))
		return (-1);
	table[pt_idx] = 0;
	flush_tlb();
	return (0);
}

bool	paging_get_page(t_paging *pg, u32 virt, u32 *phys, u32 *flags)
{
	u32	pde;
	u32	pte;

	pde = pg->directory[virt >> 22];
	if (!(pde & PAGE_PRESENT))
		return (false);
	pte = ((u32 *)(pde & ~0xFFF))[(virt >> 12) & 0x3FF];
	if (!(pte & PAGE_PRESENT))
		return (false);
	if (phys)
		*phys = (pte & ~0xFFF) | (virt & 0xFFF);
	if (flags)
		*flags = pte & 0xFFF;
	return (true);
}

void	print_page_info(t_paging *pg, u32 virt)
{
	u32	phys;
	u32	flags;

	if (!paging_get_page(pg, virt, &phys, &flags))
	{
		printk("0x%x -> not mapped\n", virt);
		return ;
	}
	printk("0x%x -> 0x%x  %s %s\n", virt, phys,
		(flags & PAGE_RW) ? "RW" : "RO",
		(flags & PAGE_USER) ? "user" : "kernel");
}
