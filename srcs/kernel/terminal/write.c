#include "terminal.h"
#include "libstr.h"

static void	_putchar(t_terminal *terminal, const char c, const char color, bool insert);
static void	_move_chars_right(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y);
static void	_move_chars_after_newline(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y);



void	terminal_putchar(t_terminal *terminal, const char c)
{
	_putchar(terminal, c, terminal->current_color, false);
}


void	terminal_putstring(t_terminal *terminal, const char *str)
{
	uint32_t	len = strlen(str);

	for (uint32_t i = 0; i < len; i++)
		_putchar(terminal, str[i], terminal->current_color, false);
}


void	terminal_write_char(t_terminal *terminal, const char c, const char color, bool insert)
{
	_putchar(terminal, c, color, insert);
}


void	terminal_write_string(t_terminal *terminal, const char *str, const char color, bool insert)
{
	uint32_t	len = strlen(str);

	for (uint32_t i = 0; i < len; i++)
		_putchar(terminal, str[i], color, insert);
}


static void	_putchar(t_terminal *terminal, const char c, const char color, bool insert)
{
	// If not insert, move all char before
	if (!insert && c != '\n')
		_move_chars_right(terminal, terminal->cursor.x, terminal->cursor.y);

	if (c == '\n') // New line case
	{
		// Update cursor pos
		_move_chars_after_newline(terminal, terminal->cursor.x, terminal->cursor.y);
		terminal->cursor.x = 0;
		terminal->cursor.y++;
	}
	else if (c == '\t') // Tab case
	{
		uint32_t	nb_space = 4 - (terminal->cursor.x % 4);

		for (uint32_t i = 0; i < nb_space; i++)
			_putchar(terminal, ' ', color, insert);
	}
	else // Classic case
	{
		// Write char and it's color
		terminal->buffer_char[terminal->cursor.y][terminal->cursor.x] = c;
		terminal->buffer_color[terminal->cursor.y][terminal->cursor.x] = color;

		// Update cursor pos
		terminal->cursor.x++;
	}

	// Update cursor pos when end lines
	if (terminal->cursor.x == TERMINAL_WIDTH)
	{
		terminal->cursor.x = 0;
		terminal->cursor.y++;
	}
	if (terminal->cursor.y == TERMINAL_HEIGHT)
	{
		terminal->cursor.x = 0;
		terminal->cursor.y--;
		terminal_scroll_up(terminal);
	}
}


static void	_move_chars_right(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y)
{
	if (pos_x >= TERMINAL_WIDTH || pos_y >= TERMINAL_HEIGHT)
		return ;

	// Save the last char of the line that will be write over
	char	tmp_char = terminal->buffer_char[pos_y][TERMINAL_MAX_X];
	char	tmp_color = terminal->buffer_color[pos_y][TERMINAL_MAX_X];

	// Put an empty char on cursor pos by moving all chars to right
	for (uint32_t x = TERMINAL_MAX_X; x > pos_x; x--)
	{
		terminal->buffer_char[pos_y][x] = terminal->buffer_char[pos_y][x - 1];
		terminal->buffer_color[pos_y][x] = terminal->buffer_color[pos_y][x - 1];
	}

	terminal->buffer_char[pos_y][pos_x] = '\0';
	terminal->buffer_color[pos_y][pos_x] = terminal->current_color;

	pos_y++;
	while (pos_y < TERMINAL_HEIGHT && tmp_char != '\0' && tmp_char != '\n')
	{
		char	tmp_char2 = terminal->buffer_char[pos_y][TERMINAL_MAX_X];
		char	tmp_color2 = terminal->buffer_color[pos_y][TERMINAL_MAX_X];

		// Move to right all the line
		for (uint32_t x = TERMINAL_MAX_X; x > 0; x--)
		{
			terminal->buffer_char[pos_y][x] = terminal->buffer_char[pos_y][x - 1];
			terminal->buffer_color[pos_y][x] = terminal->buffer_color[pos_y][x - 1];
		}

		// Put the remain char on the other line
		terminal->buffer_char[pos_y][0] = tmp_char;
		terminal->buffer_color[pos_y][0] = tmp_color;

		// Update remain char
		tmp_char = tmp_char2;
		tmp_color = tmp_color2;

		// Update y
		pos_y++;
	}

	if (pos_y == TERMINAL_HEIGHT && tmp_char != '\0' && tmp_char != '\n')
	{
		// Put the remain char on the new line
		terminal_scroll_up(terminal);
		terminal->buffer_char[TERMINAL_MAX_Y][0] = tmp_char;
		terminal->buffer_color[TERMINAL_MAX_Y][0] = tmp_color;
		if (terminal->cursor.y > 0)
			terminal->cursor.y--;
	}
}


static void	_move_chars_after_newline(t_terminal *terminal, uint32_t pos_x, uint32_t pos_y)
{
	if (pos_x >= TERMINAL_WIDTH || pos_y >= TERMINAL_HEIGHT)
		return ;

	// Save the chars after the \n and remove then from char buffer
	char		tmp_chars[TERMINAL_WIDTH] = {0};
	char		tmp_colors[TERMINAL_WIDTH] = {0};
	uint32_t	line_length = line_len(terminal->buffer_char[pos_y]);
	uint32_t	i = 0;
	for (uint32_t x = pos_x; x < TERMINAL_WIDTH; x++)
	{
		if (terminal->buffer_char[pos_y][x] == '\0')
			break;
		tmp_chars[i] = terminal->buffer_char[pos_y][x];
		tmp_colors[i] = terminal->buffer_color[pos_y][x];
		terminal->buffer_char[pos_y][x] = '\0';
		terminal->buffer_color[pos_y][x] = terminal->current_color;
		i++;
	}
	terminal->buffer_char[pos_y][pos_x] = '\n';

	// Scroll terminal if needed
	if (pos_y == TERMINAL_MAX_Y || line_len(terminal->buffer_char[TERMINAL_MAX_Y]) > 0)
	{
		terminal_scroll_up(terminal);
		terminal->cursor.y--;
	}
	else
		pos_y++;

	// Move line under the \n
	for (uint32_t y = TERMINAL_MAX_Y; y > pos_y; y--)
	{
		for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
		{
			terminal->buffer_char[y][x] = terminal->buffer_char[y - 1][x];
			terminal->buffer_color[y][x] = terminal->buffer_color[y - 1][x];
		}
	}

	// Write tmp into new line
	i = 0;
	uint32_t len_tmp = strlen(tmp_chars);
	for (uint32_t x = 0; x < len_tmp; x++)
	{
		terminal->buffer_char[pos_y][x] = tmp_chars[i];
		terminal->buffer_color[pos_y][x] = tmp_colors[i];
		i++;
	}
	// Clear the remain part of the new line
	for (uint32_t x = len_tmp; x < TERMINAL_WIDTH; x++)
	{
		terminal->buffer_char[pos_y][x] = '\0';
		terminal->buffer_color[pos_y][x] = terminal->current_color;
	}

	// Move char to continue the line
	while (line_length == TERMINAL_WIDTH && pos_x != 0 && pos_y < TERMINAL_MAX_Y)
	{
		// Check if next line isn't empty
		line_length = line_len(terminal->buffer_char[pos_y + 1]);
		if (line_length == 0)
			return ;

		// Compute how many char to move
		uint32_t	nb_char_to_move = TERMINAL_WIDTH - len_tmp;
		if (nb_char_to_move > line_length)
			nb_char_to_move = line_length;

		// Move chars for next line to current one
		for (uint32_t j = 0; j < nb_char_to_move; j++)
		{
			terminal->buffer_char[pos_y][len_tmp + j] = terminal->buffer_char[pos_y + 1][j];
			terminal->buffer_color[pos_y][len_tmp + j] = terminal->buffer_color[pos_y + 1][j];
			terminal->buffer_char[pos_y + 1][j] = '\0';
			terminal->buffer_color[pos_y + 1][j] = terminal->current_color;
		}
		pos_y++;

		// Move chars of next line to the left
		for (uint32_t j = 0; j < nb_char_to_move; j++)
		{
			terminal->buffer_char[pos_y][j] = terminal->buffer_char[pos_y][j + nb_char_to_move];
			terminal->buffer_color[pos_y][j] = terminal->buffer_color[pos_y][j + nb_char_to_move];
		}
		// Clear the last part of the new line that where move
		for (uint32_t x = TERMINAL_WIDTH - nb_char_to_move; x < TERMINAL_WIDTH; x++)
		{
			terminal->buffer_char[pos_y][x] = '\0';
			terminal->buffer_color[pos_y][x] = terminal->current_color;
		}
	}
}
