/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:11:04 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/18 11:04:34 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/gdt.h"
#include "../includes/pmm.h"
#include "../includes/paging.h"
#include "../includes/heap.h"
#include "../includes/memlayout.h"

void	need_help(void)
{
	terminal_set_color(VGA_COLOR_LIGHT_BROWN);
	printk("If you don't know what to write, try '--help'\n");
	terminal_set_color(VGA_COLOR_LIGHT_RED2);
}

void	kernel_main(u32 magic, t_multiboot_info *mbi)
{
	t_pmm	*pmm;

	if (magic != MULTIBOOT_MAGIC)
		asm volatile ("cli; hlt");
	
	gdt_init();
	terminal_initialize();
	screens_init();
	need_help();

	pmm = get_pmm();
	pmm_init(pmm, mbi, (u32)&_kernel_end);

	paging_init(get_paging(), pmm);
	paging_enable(get_paging());
	printk("[Paging] Activation OK, toujours en vie.\n");

	heap_init(get_heap(), pmm, HEAP_START);
	printk("[Heap] Initialized at 0x%x\n", HEAP_START);

	heap_init_virtual(get_vheap(), pmm, get_paging(), VMALLOC_START, VMALLOC_END);
	printk("[VHeap] Initialized at 0x%x\n", VMALLOC_START);

	display_prompt();
	keyboard_handler_loop();
}
