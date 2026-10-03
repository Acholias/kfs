/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:54:36 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 21:49:22 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# include "types.h"
# include "vga.h"

# define NEWLINE	'\n'

typedef struct	s_term_state
{
	size_t	row;
	size_t	col;
	u8		color;
}	t_term_state;

void			terminal_initialize(void);
void			terminal_clear_screen(void);
void			terminal_set_color(u8 color);
void			terminal_scroll(void);
void			terminal_putentry(char c, u8 color, size_t x, size_t y);
void			terminal_putchar(char c);
void			terminal_write(const char *data, size_t size);
void			terminal_write_string(const char *data);
void			clear_line(void);
void			display_prompt(void);
t_term_state	terminal_get_state(void);
void			terminal_set_state(t_term_state state);

#endif
