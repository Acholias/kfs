#include "commands.h"
#include "io.h"

void	command_shutdown(void)
{
	outw(0x604, 0x2000);
}
