/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:13:07 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 12:06:35 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"
#include "../../includes/io.h"
#include "../../includes/gdt.h"

static bool	awaiting_color = false;

bool	shell_is_awaiting_color(void)
{
	return (awaiting_color);
}

void	shell_set_awaiting_color(bool value)
{
	awaiting_color = value;
}

size_t	get_cmd(const char *cmd)
{
	size_t	index = 0;

	while (cmd[index] && cmd[index] != ' ')
		index++;
	return (index);
}

void	execute_command(const char *cmd)
{
	t_term_state	state = terminal_get_state();
	u8				old_color = state.color;
	size_t			len;

	if (!cmd || !*cmd)
		return ;
		
	len = get_cmd(cmd);
	if (len == 6 && ft_strncmp(cmd,	"--help", 6) == 0)
		print_helper();

	else if ((len == 5 && ft_strncmp(cmd, "clear", 5) == 0) || (len == 1 && ft_strncmp(cmd, "c", len) == 0))
		terminal_clear_screen();

	else if (len == 6 && ft_strncmp(cmd, "reboot", 6) == 0)
		outb(0x64, 0xFE);

	else if (len == 4 && ft_strncmp(cmd, "halt", 4) == 0)
		asm volatile ("cli; hlt");
	
	else if (len == 4 && ft_strncmp(cmd, "exit", 4) == 0)
	    outw(0x604, 0x2000);

	else if (len == 3 && ft_strncmp(cmd, "gdt", 3) == 0)
	{
		terminal_set_color(VGA_COLOR_WHITE);
		print_gdt();
		terminal_set_color(old_color);
	}

	else if (len == 5 && ft_strncmp(cmd, "stack", 5) == 0)
	{
		terminal_set_color(VGA_COLOR_WHITE);
		print_stack();
		terminal_set_color(old_color);
	}

	else if (len == 7 && ft_strncmp(cmd, "--color", len) == 0)
	{
		if (cmd[len] == ' ')
			color_command(cmd + len + 1);
		else
			display_color_panel();
	}

	else
		invalid_command(cmd);
}
