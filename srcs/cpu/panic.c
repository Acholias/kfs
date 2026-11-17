/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:30:20 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/17 12:43:21 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/panic.h"
#include "../../includes/kernel.h"

void	kpanic(t_panic_level level, const char *msg)
{
	t_term_state	state;
	u8				old_color;

	state = terminal_get_state();
	old_color = state.color;
	if (level == PANIC_WARNING)
	{
		terminal_set_color(VGA_COLOR_LIGHT_BROWN);
		printk("[WARNING] %s\n", msg);
		terminal_set_color(old_color);
		return ;
	}
	terminal_set_color(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
	printk("\n[KERNEL PANIC] %s\n", msg);
	printk("System halted.\n");
	asm volatile ("cli");
	while (1)
		asm volatile ("hlt");
}
