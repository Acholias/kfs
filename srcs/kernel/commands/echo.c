#include "commands.h"
#include "printk.h"
#include "libstr.h"

void	command_echo(t_terminal *terminal, const char *command_buffer)
{
	bool		newline = true;
	bool		option = true;
	bool		print_before = false;
	uint32_t	i = 0;
	uint32_t	j = 0;
	uint32_t	command_buffer_length = strlen(command_buffer);

	while (i < command_buffer_length)
	{
		if (is_whitespace(command_buffer[i]))
		{
			i++;
			continue;
		}

		if (option && !print_before)
		{
			if (command_buffer[i] != '-' || i == command_buffer_length - 1 || command_buffer[i + 1] != 'n')
			{
				option = false;
				continue;
			}

			j = i + 2;
			while (j < command_buffer_length && command_buffer[j] == 'n')
				j++;

			if (j != command_buffer_length && !is_whitespace(command_buffer[j]))
			{
				option = false;
				continue;
			}

			newline = false;
			i = j;
		}
		else
		{
			if (print_before)
				terminal_putchar(terminal, ' ');
			else
				print_before = true;

			while (i < command_buffer_length && !is_whitespace(command_buffer[i]))
			{
				terminal_putchar(terminal, command_buffer[i]);
				i++;
			}
		}

		i++;
	}

	if (newline)
		terminal_putchar(terminal, '\n');
}
