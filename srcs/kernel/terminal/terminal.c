#include "terminal.h"
#include "libstr.h"
#include "printk.h"

static const char	*modes_texts[2] = {"terminal", "text"};

t_terminal	terminal_init(
				const char base_color,
				const char topbar_color,
				const char prompt_color,
				const char* prompt)
{
	t_terminal	terminal;

	// Set terminal address
	terminal.address = TERMINAL_ADDR;

	// Init buffers
	for (uint32_t x = 0; x < TERMINAL_WIDTH + 1; x++)
	{
		terminal.buffer_char[0][x] = '\0';
		terminal.buffer_color[0][x] = topbar_color;
	}
	for (uint32_t y = 1; y < TERMINAL_HEIGHT; y++)
	{
		for (uint32_t x = 0; x < TERMINAL_WIDTH + 1; x++)
		{
			terminal.buffer_char[y][x] = '\0';
			terminal.buffer_color[y][x] = base_color;
		}
	}

	// Set colors
	terminal.current_color = base_color;
	terminal.topbar_color = topbar_color;
	terminal.prompt_color = prompt_color;

	// Init prompt
	terminal.prompt_length = strlen(prompt);

	for (uint32_t i = 0; i < terminal.prompt_length; i++)
		terminal.prompt[i] = prompt[i];
	for (uint32_t i = terminal.prompt_length; i < TERMINAL_WIDTH; i++)
		terminal.prompt[i] = '\0';

	// Init positions
	terminal.cursor.x = 0;
	terminal.cursor.y = 1;
	terminal.prompt_end_pos.x = 0;
	terminal.prompt_end_pos.y = 1;

	// Set mode
	terminal.mode = MODE_TERMINAL;

	// Write base of terminal
	terminal_update_topbar(&terminal);
	terminal_write_prompt(&terminal);

	return (terminal);
}


void	terminal_clear(t_terminal *terminal)
{
	for (uint32_t y = 1; y < TERMINAL_HEIGHT; y++)
	{
		for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
		{
			terminal->buffer_char[y][x] = '\0';
			terminal->buffer_color[y][x] = terminal->current_color;
		}
	}
	terminal->cursor.x = 0;
	terminal->cursor.y = 1;
	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_flush(t_terminal *terminal)
{
	uint32_t	char_addr = 0;
	char		char_tmp = ' ';

	for (uint32_t y = 0; y < TERMINAL_HEIGHT; y++)
	{
		for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
		{
			// Compute char addr
			char_addr = (x + y * TERMINAL_WIDTH) * 2;

			// Get char to write
			char_tmp = terminal->buffer_char[y][x];

			// Case of non displayable char remplace it by space
			if (char_tmp < 32)
				char_tmp = ' ';

			// Write char and color data
			terminal->address[char_addr] = char_tmp;
			terminal->address[char_addr + 1] = terminal->buffer_color[y][x];
		}
	}
	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_scroll_up(t_terminal *terminal)
{
	for (uint32_t y = 1; y < TERMINAL_HEIGHT - 1; y++)
	{
		for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
		{
			terminal->buffer_char[y][x] = terminal->buffer_char[y + 1][x];
			terminal->buffer_color[y][x] = terminal->buffer_color[y + 1][x];
		}
	}
	for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
	{
		terminal->buffer_char[TERMINAL_HEIGHT - 1][x] = '\0';
		terminal->buffer_color[TERMINAL_HEIGHT - 1][x] = terminal->current_color;
	}
}


void	terminal_update_topbar(t_terminal *terminal)
{
	// Save current cursor position
	uint32_t	cursor_x = terminal->cursor.x;
	uint32_t	cursor_y = terminal->cursor.y;
	char		current_color = terminal->current_color;

	// Move cursor to terminal topbar
	terminal->cursor.x = 0;
	terminal->cursor.y = 0;
	terminal->current_color = terminal->topbar_color;

	// Clear topbar
	for (uint32_t x = 0; x < TERMINAL_WIDTH; x++)
	{
		terminal->buffer_char[0][x] = '\0';
		terminal->buffer_color[0][x] = terminal->topbar_color;
	}

	// Write terminal topbar
	printk(terminal, "42 kfs ! | Cursor:(%u,%u) | Mode %s", cursor_x, cursor_y, modes_texts[terminal->mode]);

	// Relace cursor to previous position
	terminal->cursor.x = cursor_x;
	terminal->cursor.y = cursor_y;
	terminal->current_color = current_color;
	_update_cursor_pos(terminal->cursor.x, terminal->cursor.y);
}


void	terminal_write_prompt(t_terminal *terminal)
{
	terminal_write_string(terminal, terminal->prompt, terminal->prompt_color, false);
	terminal->prompt_end_pos.x = terminal->cursor.x;
	terminal->prompt_end_pos.y = terminal->cursor.y;
}
