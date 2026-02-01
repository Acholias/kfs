#include "commands.h"
#include "printk.h"
#include "libstr.h"

static t_position	get_begin_command_input(t_terminal *terminal);
static void			get_command_input(t_terminal *terminal, char *command_buffer);
static uint32_t		get_command_name(const char *command_buffer, char *command_name);

void	parse_execute_command(t_terminal *terminal)
{
	// Get the command input
	char		command_buffer[TERMINAL_WIDTH * TERMINAL_HEIGHT] = {0};
	char		command_name[TERMINAL_WIDTH + 1] = {0};
	uint32_t	command_name_length = 0;

	get_command_input(terminal, command_buffer);
	command_name_length = get_command_name(command_buffer, command_name);

	// Empty enter
	if (command_name_length == 0 && strlen(command_buffer) == 0)
	{
		terminal_putchar(terminal, '\n');
		terminal_write_prompt(terminal);
		return ;
	}

	// Go to newline
	terminal_putchar(terminal, '\n');

	// Execute command
	if (is_str_equals("clear", command_name))
		command_clear(terminal, command_buffer + command_name_length);
	else if (is_str_equals("echo", command_name))
		command_echo(terminal, command_buffer + command_name_length);
	else if (is_str_equals("gdt", command_name))
		command_gdt(terminal, command_buffer + command_name_length);
	else if (is_str_equals("shutdown", command_name))
		command_shutdown();
	else
		printk(terminal, "kfs: command '%s' is not found\n", command_name);

	// Reprint the prompt
	terminal_write_prompt(terminal);
}


static t_position	get_begin_command_input(t_terminal *terminal)
{
	t_position	pos;
	pos.x = terminal->prompt_end_pos.x;
	pos.y = terminal->prompt_end_pos.y;

	while (pos.y < TERMINAL_MAX_Y)
	{
		while (pos.x < TERMINAL_MAX_X)
		{
			if (!is_whitespace(terminal->buffer_char[pos.y][pos.x]))
				return (pos);

			pos.x++;
		}
		pos.y++;
		pos.x = 0;
	}

	return (pos);
}


static void	get_command_input(t_terminal *terminal, char *command_buffer)
{
	t_position	begin_pos = get_begin_command_input(terminal);

	uint32_t	x = begin_pos.x;
	uint32_t	y = begin_pos.y;
	uint32_t	i = 0;

	// Get the command input
	while (y < TERMINAL_HEIGHT)
	{
		while (x < TERMINAL_WIDTH)
		{
			command_buffer[i] = terminal->buffer_char[y][x];
			x++;
			i++;
		}
		y++;
		x = 0;
	}
}


static uint32_t	get_command_name(const char *command_buffer, char *command_name)
{
	uint32_t	i = 0;

	while (i < TERMINAL_WIDTH && !is_whitespace(command_buffer[i]) && command_buffer[i])
	{
		command_name[i] = command_buffer[i];
		i++;
	}

	return (i);
}
