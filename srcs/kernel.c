/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:11:04 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/12 13:34:03 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/gdt.h"
#include "../includes/pmm.h"
#include "../includes/paging.h"
#include "../includes/heap.h"

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

	display_prompt();
	keyboard_handler_loop();
}
