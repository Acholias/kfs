#include "terminal.h"
#include "libstr.h"

static void	_move_chars_left(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y);
static void	_move_chars_remove_newline(t_terminal *terminal, uint32_t pos_y);


void	terminal_remove_char(t_terminal *terminal)
{
	if (terminal->cursor.x == 0 && terminal->cursor.y == 1)
		return ;

	if (terminal->mode == MODE_TERMINAL
		&& terminal->cursor.x == terminal->prompt_end_pos.x
		&& terminal->cursor.y == terminal->prompt_end_pos.y)
		return ;

	if (terminal->cursor.x == 0)
	{
		terminal->cursor.y--;
		terminal->cursor.x = line_len(terminal->buffer_char[terminal->cursor.y]);
	}
	else
		terminal->cursor.x--;

	if (terminal->buffer_char[terminal->cursor.y][terminal->cursor.x] == '\n')
		_move_chars_remove_newline(terminal, terminal->cursor.y);
	else
		_move_chars_left(terminal, terminal->cursor.x, terminal->cursor.y);
}


void	terminal_delete_char(t_terminal *terminal)
{
	if (terminal->buffer_char[terminal->cursor.y][terminal->cursor.x] == '\n')
		_move_chars_remove_newline(terminal, terminal->cursor.y);
	else
		_move_chars_left(terminal, terminal->cursor.x, terminal->cursor.y);
}


static void	_move_chars_left(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y)
{
	if (pos_x >= TERMINAL_WIDTH || pos_y >= TERMINAL_HEIGHT)
		return ;

	// Compute lenght of the line
	uint32_t line_length = line_len(terminal->buffer_char[pos_y]);

	// Replace char on cursor pos by moving all chars to left
	for (uint32_t x = pos_x; x < line_length + 1; x++)
	{
		terminal->buffer_char[pos_y][x] = terminal->buffer_char[pos_y][x + 1];
		terminal->buffer_color[pos_y][x] = terminal->buffer_color[pos_y][x + 1];
	}

	// If line_length is smaller than terminal max width, there is an \n. Or if it's the last line, stop here
	if (pos_y == TERMINAL_MAX_Y || line_length != TERMINAL_WIDTH)
		return ;

	// Add the first char of the line at the upper one end
	terminal->buffer_char[pos_y][TERMINAL_MAX_X] = terminal->buffer_char[pos_y + 1][0];
	terminal->buffer_color[pos_y][TERMINAL_MAX_X] = terminal->buffer_color[pos_y + 1][0];

	pos_y++;

	while (pos_y < TERMINAL_MAX_Y)
	{
		// Add the first char of the line at the upper one end
		terminal->buffer_char[pos_y][TERMINAL_MAX_X] = terminal->buffer_char[pos_y + 1][0];
		terminal->buffer_color[pos_y][TERMINAL_MAX_X] = terminal->buffer_color[pos_y + 1][0];

		// Compute lenght of the line
		line_length = line_len(terminal->buffer_char[pos_y]);

		// Move all chars to left
		for (uint32_t x = 0; x < line_length + 1; x++)
		{
			terminal->buffer_char[pos_y][x] = terminal->buffer_char[pos_y][x + 1];
			terminal->buffer_color[pos_y][x] = terminal->buffer_color[pos_y][x + 1];
		}

		// If line_length is smaller than terminal max width, there is an \n. Or if it's the last line, stop here
		if (pos_y == TERMINAL_MAX_Y || line_length != TERMINAL_WIDTH)
			return ;

		pos_y++;
	}
}


static void	_move_chars_remove_newline(t_terminal *terminal, uint32_t pos_y)
{
	if (pos_y >= TERMINAL_MAX_Y) // Skip also if the \n is at the last line, this is not normal
		return ;

	// Compute lenght of the line
	uint32_t line_length = line_len(terminal->buffer_char[pos_y]);

	// Check if there is a \n at the line end
	if (terminal->buffer_char[pos_y][line_length] != '\n')
		return ;

	// Count how many char to move
	uint32_t	nb_char_to_move = TERMINAL_WIDTH - line_length;
	uint32_t	nb_chars_bot_line = line_len(terminal->buffer_char[pos_y + 1]);

	if (nb_chars_bot_line < TERMINAL_WIDTH)
		nb_chars_bot_line++;

	if (nb_char_to_move > nb_chars_bot_line)
		nb_char_to_move = nb_chars_bot_line;

	if (nb_char_to_move == 0) // If the bottom line is empty, stop here
		return ;

	// Move chars to current line
	for (uint32_t i = 0; i < nb_char_to_move; i++)
	{
		terminal->buffer_char[pos_y][line_length + i] = terminal->buffer_char[pos_y + 1][i];
		terminal->buffer_color[pos_y][line_length + i] = terminal->buffer_color[pos_y + 1][i];
	}

	// Move chars in bottom line
	pos_y++;
	for (uint32_t i = 0; i < TERMINAL_WIDTH - nb_char_to_move; i++)
	{
		terminal->buffer_char[pos_y][i] = terminal->buffer_char[pos_y][i + nb_char_to_move];
		terminal->buffer_color[pos_y][i] = terminal->buffer_color[pos_y][i + nb_char_to_move];
	}
	// And clear the rest of the line
	for (uint32_t i = TERMINAL_WIDTH - nb_char_to_move; i < TERMINAL_WIDTH; i++)
	{
		terminal->buffer_char[pos_y][i] = '\0';
		terminal->buffer_color[pos_y][i] = terminal->current_color;
	}

	line_length = line_len(terminal->buffer_char[pos_y]);
	while (pos_y < TERMINAL_MAX_Y && terminal->buffer_char[pos_y][line_length] != '\n')
	{
		// Move bottom line to this one
		nb_char_to_move = TERMINAL_WIDTH - line_length;
		nb_chars_bot_line = line_len(terminal->buffer_char[pos_y + 1]);

		if (nb_char_to_move > nb_chars_bot_line)
			nb_char_to_move = nb_chars_bot_line;

		if (nb_char_to_move == 0) // If the bottom line is empty, stop here
			return ;

		// Move chars to current line
		for (uint32_t i = 0; i < nb_char_to_move; i++)
		{
			terminal->buffer_char[pos_y][line_length + i] = terminal->buffer_char[pos_y + 1][i];
			terminal->buffer_color[pos_y][line_length + i] = terminal->buffer_color[pos_y + 1][i];
		}

		// Move chars in bottom line
		pos_y++;
		for (uint32_t i = 0; i < TERMINAL_WIDTH - nb_char_to_move; i++)
		{
			terminal->buffer_char[pos_y][i] = terminal->buffer_char[pos_y][i + nb_char_to_move];
			terminal->buffer_color[pos_y][i] = terminal->buffer_color[pos_y][i + nb_char_to_move];
		}
		// And clear the rest of the line
		for (uint32_t i = TERMINAL_WIDTH - nb_char_to_move; i < TERMINAL_WIDTH; i++)
		{
			terminal->buffer_char[pos_y][i] = '\0';
			terminal->buffer_color[pos_y][i] = terminal->current_color;
		}

		line_length = line_len(terminal->buffer_char[pos_y]);
	}
}

