/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 21:57:24 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 12:10:58 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"

static const char	*color_names[] = {
	"BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA", "BROWN",
	"LIGHT_GREY", "DARK_GREY", "LIGHT_BLUE", "LIGHT_GREEN",
	"LIGHT_CYAN", "LIGHT_RED2", "LIGHT_MAGENTA", "LIGHT_BROWN", "WHITE"
};

void	display_color_panel(void)
{
	int	index;

	index = 0;
	while (index < COLOR_COUNT)
	{
		printk("%d-\t\t%s\n", index + 1, color_names[index]);
		index++;
	}
	printk("\nSelect number for apply color in terminal:  \n");
	shell_set_awaiting_color(true);
}

void	color_command(const char *num)
{
	int	choice;

	shell_set_awaiting_color(false);
	if (!num || !*num)
	{
		printk("Soliacha: Invalid input !");
		return ;
	}
	choice = ft_atoi(num);
	if (choice < 1 || choice > COLOR_COUNT)
	{
		printk("Soliacha: Invalid color number (1-%d)\n", COLOR_COUNT);
		return ;
	}
	terminal_set_color(vga_entry_color((enum vga_color)(choice - 1), VGA_COLOR_BLACK));
	printk("Color set to:  %s\n", color_names[choice - 1]);
}
