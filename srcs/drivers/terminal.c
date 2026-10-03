/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:32:50 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 21:50:37 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/terminal.h"
#include "../../includes/screens.h"
#include "../../includes/kernel.h"

static t_term_state	term = {0, 0, 0};

t_term_state	terminal_get_state(void)
{
	return (term);
}

void	terminal_set_state(t_term_state state)
{
	term = state;
}

void	terminal_initialize(void)
{
	u16		*buf;
	size_t	y;
	size_t	x;

	term.row = 0;
	term.col = 0;
	term.color = vga_entry_color(VGA_COLOR_LIGHT_RED2, VGA_COLOR_BLACK);

	buf = vga_buffer_ptr();
	y = 0;
	while (y < VGA_HEIGHT)
	{
		x = 0;
		while (x < VGA_WIDTH)
		{
			buf[y * VGA_WIDTH + x] = vga_entry(' ', term.color);
			x++;
		}
		y++;
	}
	display_prompt();
}

void	terminal_clear_screen(void)
{
	u16	*buffer;

	buffer = vga_buffer_ptr();
	for (size_t y = 0; y < VGA_HEIGHT; y++)
		for (size_t x = 0; x < VGA_WIDTH; x++)
			buffer[y * VGA_WIDTH + x] = vga_entry(' ', term.color);

	term.row = 0;
	term.col = 0;
	term.color = vga_entry_color(VGA_COLOR_LIGHT_RED2, VGA_COLOR_BLACK);
}

void	terminal_set_color(u8 color)
{
	term.color = color;
}

void	terminal_putentry(char c, u8 color, size_t x, size_t y)
{
	vga_buffer_ptr()[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void	terminal_scroll(void)
{
	u16		*buffer;
	size_t	bytes_copy;
	size_t	x;
	
	buffer = vga_buffer_ptr();
	bytes_copy = (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(u16);
	ft_memcpy((void *)buffer, (void *)(buffer + VGA_WIDTH), bytes_copy);
	x = 0;
	while (x < VGA_WIDTH)
	{
		buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', term.color);
		++x;
	}
	term.row = VGA_HEIGHT - 1;
	term.col = 0;
}

void	terminal_putchar(char c)
{
	size_t	max_col;

	if (c == NEWLINE)
	{
		term.row++;
		term.col = 0;
		if (term.row >= VGA_HEIGHT)
			terminal_scroll();
		vga_set_cursor(term.row, term.col);
	}
	else
	{
		terminal_putentry(c, term.color, term.col, term.row);
		++term.col;
		max_col = (term.row == 0) ? (VGA_WIDTH - 14) : VGA_WIDTH;
		if (term.col >= max_col)
		{
			term.col = 0;
			++term.row;
			if (term.row >= VGA_HEIGHT)
				terminal_scroll();
		}
		vga_set_cursor(term.row, term.col);
	}
}

void	terminal_write(const char *data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void	terminal_write_string(const char *data)
{
	size_t	i = 0;

	while (data[i])
	{
		terminal_putchar(data[i]);
		i++;
	}
}

void	clear_line(void)
{
	size_t	x;

	x = PROMPT_LENGTH;
	while (x < VGA_WIDTH)
	{
		terminal_putentry(' ', term.color, x, term.row);
		++x;
	}
	term.col = PROMPT_LENGTH;
	vga_set_cursor(term.row, term.col);
	display_prompt();
}

void	display_prompt(void)
{
	u8			old_color;
	size_t		i;
	const char	*prompt;

	old_color = term.color;
	i = 0;
	prompt = "Soliacha -> ";
	term.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	while (prompt[i])
	{
		terminal_putchar(prompt[i]);
		i++;
	}
	term.color = old_color;
	screens_draw_index();
	vga_set_cursor(term.row, PROMPT_LENGTH);
}
