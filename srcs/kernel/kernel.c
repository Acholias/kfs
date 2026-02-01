#include "types.h"
#include "colors.h"
#include "libstr.h"
#include "terminal.h"
#include "keyboard.h"
#include "printk.h"
#include "commands.h"

#define NB_TERMINALS 2

void	kernel_main(void)
{
	//Init keyboard
	t_keyboard	keyboard = keyboard_init();

	// Init terminals
	t_terminal	terminals[NB_TERMINALS];
	uint8_t		active_terminal_id = 0;

	terminals[0] = terminal_init(create_color(FG_WHITE, BG_BLACK), create_color(FG_WHITE, BG_DARK_GRAY), create_color(FG_WHITE, BG_BLACK), "> ");
	terminals[1] = terminal_init(create_color(FG_WHITE, BG_BLACK), create_color(FG_WHITE, BG_DARK_GRAY), create_color(FG_GREEN, BG_BLACK), "kfs@prompt: ");

	t_terminal	*active_terminal = &terminals[active_terminal_id];
	terminal_flush(active_terminal);

	// Infinite loop to keep the kernel running
	t_key_event	key_event;
	while (1)
	{
		if (keyboard_poll(&keyboard, &key_event))
		{
			if (key_event.type == KEY_PRESS)
			{
				// Write characters
				if (key_event.ascii != '\0')
				{
					if (keyboard.Lctrl && key_event.key == K_C)
					{
						terminal_putstring(active_terminal, "^C\n");
						if (active_terminal->mode == MODE_TERMINAL)
							terminal_write_prompt(active_terminal);
					}
					else if (keyboard.Lctrl && key_event.key == K_S)
						command_shutdown();
					else
						terminal_putchar(active_terminal, key_event.ascii);
				}
				else if (key_event.key == K_ENTER)
				{
					if (active_terminal->mode == MODE_TERMINAL)
					{
						terminal_cursor_end(active_terminal);
						parse_execute_command(active_terminal);
					}
					else
						terminal_putchar(active_terminal, '\n');
				}
				else if (key_event.key == K_TAB)
				{
					if (keyboard.Lctrl) // Switch terminal
					{
						active_terminal_id++;
						if (active_terminal_id == NB_TERMINALS)
							active_terminal_id = 0;
						active_terminal = &terminals[active_terminal_id];
						terminal_update_cursor(active_terminal);
					}
					else if (keyboard.Lshift) // Swicth mode
						active_terminal->mode = !active_terminal->mode;
					else
					{
						terminal_putchar(active_terminal, '\t');
					}
				}

				// Remove characters
				else if (key_event.key == K_BACKSPACE)
					terminal_remove_char(active_terminal);
				else if (key_event.key == K_DELETE)
				{
					if (keyboard.Lctrl)
					{
						terminal_clear(active_terminal);
						if (active_terminal->mode == MODE_TERMINAL)
							terminal_write_prompt(active_terminal);
					}
					else
						terminal_delete_char(active_terminal);
				}

				// Move cursor
				else if (key_event.key == K_LEFT)
					terminal_cursor_left(active_terminal);
				else if (key_event.key == K_RIGHT)
					terminal_cursor_right(active_terminal);
				else if (key_event.key == K_UP)
					terminal_cursor_up(active_terminal);
				else if (key_event.key == K_DOWN)
					terminal_cursor_down(active_terminal);
				else if (key_event.key == K_HOME)
				{
					if (keyboard.Lctrl)
						active_terminal->cursor.y = 1;
					terminal_cursor_start(active_terminal);
				}
				else if (key_event.key == K_END)
				{
					if (keyboard.Lctrl)
					{
						for (uint32_t y = active_terminal->cursor.y; y < TERMINAL_MAX_Y; y++)
							terminal_cursor_down(active_terminal);
					}
					terminal_cursor_end(active_terminal);
				}

				terminal_update_topbar(active_terminal);
				terminal_flush(active_terminal);
			}
		}
	}
}


