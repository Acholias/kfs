/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:13:07 by lumugot           #+#    #+#             */
/*   Updated: 2026/01/30 19:27:34 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kernel.h"
#include "../includes/io.h"
#include "../includes/gdt.h"
#include "../includes/stdbool.h"

int	ft_strncmp(const char *s1, const char *s2, size_t len)
{
	size_t	index;

	index = 0;
	while (index < len && s1[index] && s2[index] && s1[index] == s2[index])
		index++;
	if (index == len)
		return (0);
	return ((unsigned char)s1[index] - (unsigned char)s2[index]);
}

size_t	get_cmd(const char *cmd)
{
	size_t	index;

	index = 0;
	while (cmd[index] && cmd[index] != ' ')
		index++;
	return (index);
}

static void	terminal_color_helper(t_screen *screen)
{
	u8	old_color;

	old_color = screen->terminal_color;
	terminal_clear_screen(screen);
	terminal_set_color(screen, VGA_COLOR_LIGHT_BROWN);
	printk(screen, "Available colors:\n\n");
	printk(screen, "  0  - BLACK\n");
	printk(screen, "  1  - BLUE\n");
	printk(screen, "  2  - GREEN\n");
	printk(screen, "  3  - CYAN\n");
	printk(screen, "  4  - RED\n");
	printk(screen, "  5  - MAGENTA\n");
	printk(screen, "  6  - BROWN\n");
	printk(screen, "  7  - LIGHT_GREY\n");
	printk(screen, "  8  - DARK_GREY\n");
	printk(screen, "  9  - LIGHT_BLUE\n");
	printk(screen, "  10 - LIGHT_GREEN\n");
	printk(screen, "  11 - LIGHT_CYAN\n");
	printk(screen, "  12 - LIGHT_RED\n");
	printk(screen, "  13 - LIGHT_MAGENTA\n");
	printk(screen, "  14 - LIGHT_BROWN\n");
	printk(screen, "  15 - WHITE\n\n");
	terminal_set_color(screen, old_color);
}

static int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static bool	is_valid_number(const char *str)
{
	int		i;
	bool	has_digit;

	i = 0;
	has_digit = false;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			has_digit = true;
			i++;
		}
		else if (str[i] == ' ' || str[i] == '\t' || str[i] == '\0')
			break ;
		else
			return (false);
	}
	return (has_digit);
}

static void	terminal_color_manager(t_screen *screen, const char *arg)
{
	int	color;

	if (!arg || arg[0] == '\0')
	{
		terminal_color_helper(screen);
		printk(screen, "Select number for new color !\n");
		printk(screen, "Usage: color <num>\n");
		return ;
	}
	if (!is_valid_number(arg))
	{
		terminal_color_helper(screen);
		printk(screen, "Error: Invalid color format!\n");
		printk(screen, "Please enter a number between 0 and 15.\n");
		return ;
	}
	color = ft_atoi(arg);
	if (color < 0 || color > 15)
	{
		terminal_color_helper(screen);
		printk(screen, "Error: Color must be between 0 and 15!\n");
		printk(screen, "Type 'color' to see available colors.\n");
		return ;
	}
	terminal_set_color(screen, (u8)color);
	printk(screen, "Terminal color changed to %d\n", color);
}

static char	*get_argument(const char *cmd)
{
	int	index;

	index = 0;
	while (cmd[index] && cmd[index] != ' ')
		index++;
	if (cmd[index] == ' ')
	{
		index++;
		while (cmd[index] == ' ')
			index++;
		if (cmd[index])
			return ((char *)&cmd[index]);
	}
	return (NULL);
}

void	execute_command(t_screen *screen, const char *cmd)
{
	size_t	len;
	char	*arg;

	if (!cmd || !*cmd)
		return ;
	len = get_cmd(cmd);
	arg = get_argument(cmd);
	if (len == 4 && ft_strncmp(cmd, "help", 4) == 0)
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
		printk(screen, "color <num>  - change text color (0-15)\n");
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
	else if (len == 5 && ft_strncmp(cmd, "color", 5) == 0)
		terminal_color_manager(screen, arg);
	else
	{
		printk(screen, "Command not found: %s\n", cmd);
		printk(screen, "Type 'help' for available commands.\n");
	}
}
