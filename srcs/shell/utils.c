/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 21:55:11 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 12:44:51 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"

int		ft_strncmp(const char *s1, const char *s2, size_t len)
{
	size_t	index = 0;
	
	while (index < len && s1[index] && s2[index] && s1[index] == s2[index])
		index++;
	if (index == len)
		return (0);
	return ((unsigned char)s1[index] - (unsigned char)s2[index]);
}

int	ft_atoi(const char *str)
{
	int	res;
	int	index;

	res = 0;
	index = 0;
	while (str[index] >= '0' && str[index] <= '9')
	{
		res = res * 10 + str[index] - '0';
		index++;
	}
	return (res);
}

void	print_helper(void)
{
	t_term_state	state;
	u8				old_color;

	state = terminal_get_state();
	old_color = state.color;

	terminal_set_color(VGA_COLOR_LIGHT_BROWN);
	printk("Commands:\n");
	printk("--help\t\t- show this message\n");
	printk("--color\t\t- change terminal color\n");
	printk("clear\t\t- clear screen\n");
	printk("reboot\t\t- reboot machine\n");
	printk("halt\t\t- stop cpu\n");
	printk("exit\t\t- exit kernel\n");
	printk("stack\t\t- print stack\n");
	printk("gdt\t\t\t- print gdt\n");
	terminal_set_color(old_color);
}

void	invalid_command(const char *cmd)
{
	t_term_state	state;
	u8				old_color;

	state = terminal_get_state();
	old_color = state.color;

	terminal_set_color(VGA_COLOR_DARK_GREY);
	printk("Soliacha: Command not found: %s\n", cmd);
	terminal_set_color(old_color);
}
