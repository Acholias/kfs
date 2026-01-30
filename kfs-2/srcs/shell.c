/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:13:07 by lumugot           #+#    #+#             */
/*   Updated: 2026/01/30 14:27:10 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/io.h"
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

static void	terminal_color_helper(t_screen *screen)
{
	u8	old_color = screen->terminal_color;

	terminal_clear_screen(screen);
	terminal_set_color(screen, VGA_COLOR_LIGHT_BROWN);
	printk(screen, "All colors:\n");
	printk(screen, "BLUE          = 0\n");
	printk(screen, "GREEN         = 1\n");
	printk(screen, "CYAN          = 2\n");
	printk(screen, "RED           = 3\n");
	printk(screen, "MAGENTA       = 4\n");
	printk(screen, "BROWN         = 5\n");
	printk(screen, "LIGHT_GREY    = 6\n");
	printk(screen, "DARK_GREY     = 7\n");
	printk(screen, "LIGHT_BLUE    = 8\n");
	printk(screen, "LIGHT_GREEN   = 9\n");
	printk(screen, "LIGHT_CYAN    = 10\n");
	printk(screen, "LIGHT_RED2    = 11\n");
	printk(screen, "LIGHT_MAGENTA = 12\n");
	printk(screen, "LIGHT_BROW    = 13\n");
	printk(screen, "WHITE         = 14\n");
	terminal_set_color(screen, old_color);
}

static void	terminal_color_manager(t_screen *screen)
{
	char	input[10];
	int		color;
	u32		index;

	terminal_color_helper(screen);
	printk(screen, "\nEnter color number: ");

	index = 0;
	while (index < 9)
	{
		input[index] = inb(0x60);
		if (input[index] == '\n' || input[index] == '\r')
			break ;
		index++;
	}
	input[index] = '\0';
	

	index = 0;
	color = 0;
	while (input[index] >= '0' && input[index] <= 14)
	{
		color = color * 10 + (input[index] - '0');
		index++;
	}
	if (color >= 0 && color <= 14)
	{
		terminal_set_color(screen, color);
		printk(screen, "Terminal color change !");
	}
	else
		printk(screen, "Invalid color number !\n");
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
		printk(screen, "reboot       - reboot kernel\n");
		printk(screen, "halt         - stop cpu\n");
		printk(screen, "exit         - exit kernel\n");
		printk(screen, "stack        - print stack\n");
		printk(screen, "gdt          - print gdt\n");
		printk(screen, "Hello there  - print easter egg\n");
		printk(screen, "color        - Change text color\n");
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
	{
		terminal_set_color(screen, VGA_COLOR_WHITE);
		print_gdt(screen);
		terminal_set_color(screen, VGA_COLOR_LIGHT_RED2);
	}

	else if (len == 5 && ft_strncmp(cmd, "stack", 5) == 0)
	{
		terminal_set_color(screen, VGA_COLOR_WHITE);
		print_stack(screen);
		terminal_set_color(screen, VGA_COLOR_LIGHT_RED2);
	}

	else if (ft_strncmp(cmd, "Hello there", 11) == 0)
		printk(screen, "General Kenobi\n");

	else if (ft_strncmp(cmd, "color", 5) == 0)
		terminal_color_manager(screen);
}
