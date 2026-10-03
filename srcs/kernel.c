/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:11:04 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 21:50:33 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/gdt.h"

void	need_help(void)
{
	terminal_set_color(VGA_COLOR_LIGHT_BROWN);
	printk("If you don't know what to write, try 'help'\n");
	terminal_set_color(VGA_COLOR_LIGHT_RED2);
}

void	kernel_main(void)
{
	gdt_init();
	
	terminal_initialize();
	screens_init();
	
	need_help();
	display_prompt();
	
	keyboard_handler_loop();
}
