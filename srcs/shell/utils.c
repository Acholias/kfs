/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 21:55:11 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 22:21:11 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"

void	print_helper(void)
{
	terminal_set_color(VGA_COLOR_LIGHT_BROWN);
	printk("Commands:\n");
	printk("help         - show this message\n");
	printk("clear        - clear screen\n");
	printk("reboot       - reboot machine\n");
	printk("halt         - stop cpu\n");
	printk("exit         - exit kernel\n");
	printk("stack        - print stack\n");
	printk("gdt          - print gdt\n");
	terminal_set_color(VGA_COLOR_LIGHT_RED2);
}

void	invalid_command(const char *cmd)
{
	terminal_set_color(VGA_COLOR_DARK_GREY);
	printk("Soliacha: Command not found: %s\n", cmd);
	terminal_set_color(VGA_COLOR_LIGHT_RED2);
}
