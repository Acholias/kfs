#include "terminal.h"
#include "io.h"
#include "libstr.h"

void	terminal_set_cursor(t_terminal *terminal, uint32_t x, uint32_t y)
{
	if (x >= TERMINAL_WIDTH || y >= TERMINAL_HEIGHT)
		return ;

	terminal->cursor.x = x;
	terminal->cursor.y = y;

	_update_cursor_pos(x, y);
}


void	terminal_update_cursor(t_terminal *terminal)
{
	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_left(t_terminal *terminal)
{
	if (terminal->cursor.x == 0 && terminal->cursor.y == 1)
		return ;

	if (terminal->mode == MODE_TERMINAL
		&& terminal->cursor.x == terminal->prompt_end_pos.x
		&& terminal->cursor.y == terminal->prompt_end_pos.y)
		return ;

	if (terminal->cursor.x == 0)
	{
		uint32_t previous_line_length = line_len(terminal->buffer_char[terminal->cursor.y - 1]);

		if (terminal->mode == MODE_TERMINAL && previous_line_length < TERMINAL_WIDTH)
			return ;

		terminal->cursor.x = 0;
		terminal->cursor.y--;
		terminal->cursor.x = previous_line_length - 1;
	}
	else
		terminal->cursor.x--;

	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_right(t_terminal *terminal)
{
	if (terminal->cursor.x == TERMINAL_WIDTH - 1 && terminal->cursor.y == TERMINAL_HEIGHT - 1)
		return ;

	terminal->cursor.x++;
	uint32_t line_length = line_len(terminal->buffer_char[terminal->cursor.y]);

	if (terminal->cursor.x > line_length)
	{
		// Check if the current line is full, or contains an \n
		uint32_t line_length = line_len(terminal->buffer_char[terminal->cursor.y]);
		if (line_length < TERMINAL_WIDTH && terminal->buffer_char[terminal->cursor.y][line_length] != '\n')
		{
			terminal->cursor.x--;
			return ;
		}

		terminal->cursor.y++;
		terminal->cursor.x = 0;
	}

	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_up(t_terminal *terminal)
{
	if (terminal->cursor.y == 1)
		return ;

	uint32_t line_length = line_len(terminal->buffer_char[terminal->cursor.y - 1]);

	if (terminal->mode == MODE_TERMINAL && line_length < TERMINAL_WIDTH)
		return ;

	terminal->cursor.y--;

	if (terminal->cursor.x > line_length)
		terminal->cursor.x = line_length;

	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_down(t_terminal *terminal)
{
	if (terminal->cursor.y == TERMINAL_HEIGHT - 1)
		return ;

	// Check if the current line is full, or contains an \n
	uint32_t line_length = line_len(terminal->buffer_char[terminal->cursor.y]);
	if (line_length < TERMINAL_WIDTH && terminal->buffer_char[terminal->cursor.y][line_length] != '\n')
		return ;

	terminal->cursor.y++;
	line_length = line_len(terminal->buffer_char[terminal->cursor.y]);

	if (terminal->cursor.x > line_length)
		terminal->cursor.x = line_length;

	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_start(t_terminal *terminal)
{
	terminal->cursor.x = 0;
	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_cursor_end(t_terminal *terminal)
{
	terminal->cursor.x = line_len(terminal->buffer_char[terminal->cursor.y]);

	if (terminal->cursor.x == TERMINAL_WIDTH)
		terminal->cursor.x--;

	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	_update_cursor_pos(uint32_t x, uint32_t y)
{
	uint32_t pos = x + y * TERMINAL_WIDTH;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, ((pos >> 8) & 0xFF));
}
