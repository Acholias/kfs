#include "commands.h"
#include "printk.h"
#include "libstr.h"

void	command_gdt(t_terminal *terminal, const char *command_buffer)
{
	uint32_t	i = 0;
	uint32_t	command_buffer_length = strlen(command_buffer);

	while (i < command_buffer_length && is_whitespace(command_buffer[i]))
	i++;

	if (i != command_buffer_length)
	{
		printk(terminal, "gdt take no argument\n", command_buffer);
		return ;
	}

	// Get gdt table info
	// extern uint8_t	gdt_start;
	// uint8_t			*gdt_addr = &gdt_start;
	// uint8_t			*gdt_start_part = &gdt_addr[0];
	// uint8_t			*gdt_sys_code = &gdt_addr[8];
	// uint8_t			*gdt_sys_data = &gdt_addr[16];
	// uint8_t			*gdt_sys_stack = &gdt_addr[24];
	// uint8_t			*gdt_user_code = &gdt_addr[32];
	// uint8_t			*gdt_user_data = &gdt_addr[40];
	// uint8_t			*gdt_user_stack = &gdt_addr[48];

	// printk(terminal, "GDT (%p) :\n", gdt_addr);
	// printk(terminal, "  start      : %h %h %h %h\n", gdt_start_part[0], gdt_start_part[1], gdt_start_part[2], gdt_start_part[3]);
	// printk(terminal, "  sys code   : %h %h %h %h\n", gdt_sys_code[0], gdt_sys_code[1], gdt_sys_code[2], gdt_sys_code[3]);
	// printk(terminal, "  sys data   : %h %h %h %h\n", gdt_sys_data[0], gdt_sys_data[1], gdt_sys_data[2], gdt_sys_data[3]);
	// printk(terminal, "  sys stack  : %h %h %h %h\n", gdt_sys_stack[0], gdt_sys_stack[1], gdt_sys_stack[2], gdt_sys_stack[3]);
	// printk(terminal, "  user code  : %h %h %h %h\n", gdt_user_code[0], gdt_user_code[1], gdt_user_code[2], gdt_user_code[3]);
	// printk(terminal, "  user data  : %h %h %h %h\n", gdt_user_data[0], gdt_user_data[1], gdt_user_data[2], gdt_user_data[3]);
	// printk(terminal, "  user stack : %h %h %h %h\n", gdt_user_stack[0], gdt_user_stack[1], gdt_user_stack[2], gdt_user_stack[3]);
}
