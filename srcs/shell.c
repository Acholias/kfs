/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:13:07 by lumugot           #+#    #+#             */
/*   Updated: 2026/02/08 14:18:53 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/gdt.h"

int		ft_strncmp(const char *s1, const char *s2, size_t len)
{
	size_t	index = 0;
	
	while (index < len && s1[index] && s2[index] && s1[index] == s2[index])
		index++;
	if (index == len)
		return (0);
	return ((unsigned char)s1[index] - (unsigned char)s2[index]);
}

size_t	get_cmd(const char *cmd)
{
	size_t	index = 0;

	while (cmd[index] && cmd[index] != ' ')
		index++;
	return (index);
}

void	execute_command(t_screen *screen, const char *cmd)
{
	size_t	len;

	if (!cmd || !*cmd)
		return ;
		
	len = get_cmd(cmd);

	if (len == 4 && ft_strncmp(cmd,	"help", 4) == 0)
	{
		printk(screen, "Commands:\n");
		printk(screen, "help         - show this message\n");
		printk(screen, "clear        - clear screen\n");
		printk(screen, "reboot       - reboot machine\n");
		printk(screen, "halt         - stop cpu\n");
		printk(screen, "exit         - exit kernel\n");
		printk(screen, "stack        - print stack\n");
		printk(screen, "gdt          - print gdt\n");
		printk(screen, "Hello there  - print easter egg\n");
	}
	
	else if (len == 5 && ft_strncmp(cmd, "clear", 5) == 0)
		terminal_clear_screen(screen);

	else if (len == 6 && ft_strncmp(cmd, "reboot", 6) == 0)
		outb(0x64, 0xFE);

	else if (len == 4 && ft_strncmp(cmd, "halt", 4) == 0)
		asm volatile ("cli; hlt");
	
	else if (len == 4 && ft_strncmp(cmd, "exit", 4) == 0)
		outw(0x604, 0x2000);

	else if (len == 3 && ft_strncmp(cmd, "gdt", 3) == 0)
		print_gdt(screen);

	else if (len == 5 && ft_strncmp(cmd, "stack", 5) == 0)
		print_stack(screen);

	else if (ft_strncmp(cmd, "Hello there", 11) == 0)
		printk(screen, "General Kenobi\n");
	else
	{
		print_prompt(screen);
		printk(screen, "Command not found\n");
	}
}
