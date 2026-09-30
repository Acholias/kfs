/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screens.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:40:18 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 16:33:16 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/screens.h"
#include "../../includes/terminal.h"
#include "../../includes/keyboard.h"
#include "../../includes/kernel.h"

static t_screen	screens[NUM_SCREENS];
static size_t	current_screen = 0;

static void	save_screen(size_t screen_id)
{
	t_term_state	state;

	if (screen_id >= NUM_SCREENS)
		return ;
	state = terminal_get_state();
	ft_memcpy(screens[screen_id].save_buffer, vga_buffer_ptr(),
		VGA_WIDTH * VGA_HEIGHT * sizeof(u16));
	screens[screen_id].save_row = state.row;
	screens[screen_id].save_column = state.col;
	screens[screen_id].save_color = state.color;
	screens[screen_id].save_input_end = keyboard_get_input_end();
}

static void	load_screen(size_t screen_id)
{
	t_term_state	state;

	if (screen_id >= NUM_SCREENS)
		return ;
	ft_memcpy(vga_buffer_ptr(), screens[screen_id].save_buffer,
		VGA_WIDTH * VGA_HEIGHT * sizeof(u16));
	state.row = screens[screen_id].save_row;
	state.col = screens[screen_id].save_column;
	state.color = screens[screen_id].save_color;
	if (state.col == 0)
		state.col = PROMPT_LENGTH;
	terminal_set_state(state);
	keyboard_set_input_end(screens[screen_id].save_input_end);
	vga_set_cursor(state.row, state.col);
}

void	screens_init(void)
{
	t_term_state	state;

	state = terminal_get_state();
	current_screen = 0;
	for (size_t s = 0; s < NUM_SCREENS; ++s)
	{
		screens[s].save_row = 0;
		screens[s].save_column = 0;
		screens[s].save_input_end = PROMPT_LENGTH;
		screens[s].save_color = state.color;
		ft_memcpy(screens[s].save_buffer, vga_buffer_ptr(),
			VGA_WIDTH * VGA_HEIGHT * sizeof(u16));
	}
}

void	screens_switch(size_t new_screen_id)
{
	if (new_screen_id >= NUM_SCREENS || new_screen_id == current_screen)
		return ;
	save_screen(current_screen);
	current_screen = new_screen_id;
	load_screen(new_screen_id);
	screens_draw_index();
}

size_t	screens_current(void)
{
	return (current_screen);
}

void	screens_draw_index(void)
{
	const char	*text = "Screen  /  ";
	size_t		start_x = VGA_WIDTH - 13;
	u8			color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	u16			*buf = vga_buffer_ptr();

	for (size_t index = 0; text[index]; ++index)
	{
		if (index == 7)
			buf[start_x + index] = vga_entry('1' + current_screen, color);
		else if (index == 9)
			buf[start_x + index] = vga_entry('0' + NUM_SCREENS, color);
		else
			buf[start_x + index] = vga_entry(text[index], color);
	}
}
