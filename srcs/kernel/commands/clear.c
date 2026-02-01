#include "commands.h"
#include "printk.h"
#include "libstr.h"

void	command_clear(t_terminal *terminal, const char *command_buffer)
{
	uint32_t	i = 0;
	uint32_t	command_buffer_length = strlen(command_buffer);

	while (i < command_buffer_length && is_whitespace(command_buffer[i]))
		i++;

	if (i != command_buffer_length)
		printk(terminal, "clear take no argument\n", command_buffer);
	else
		terminal_clear(terminal);
}
