#include "../includes/kernel.h"
#include "../includes/io.h"
#include "../includes/gdt.h"

static const char scancode_to_ascii[128] = {
    0,27,'1','2','3','4','5','6','7','8',
    '9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p',
    '[',']',0,0,
    'a','s','d','f','g','h','j','k','l',';',
    '\'','`',0,'\\','z','x','c','v','b','n',
    'm',',','.','/',0,'*',0,' ',0,0
};

static const char scancode_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*',
    '(', ')', '_', '+', '\b', '\t',
    'Q','W','E','R','T','Y','U','I','O','P',
    '{','}',0,0,
    'A','S','D','F','G','H','J','K','L',':',
    '"','~',0,'|','Z','X','C','V','B','N',
    'M','<','>','?',0,'*',0,' '
};


static inline u8 vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return (fg | bg << 4);
}

static inline u16	vga_entry(unsigned char uc, u8 color)
{
	return ((u16)uc | (u16)color << 8);
}

t_screen	*terminal_initialize()
{
	t_screen	*screen = NULL;

	screen->terminal_row = 0;
	screen->terminal_column = 0;
	screen->terminal_color = vga_entry_color(VGA_COLOR_LIGHT_RED2, VGA_COLOR_BLACK);
	screen->terminal_buffer = (u16*)VGA_MEMORY;
	screen->current_screen = 0;
	screen->input_end = PROMPT_LENGTH;
	screen->input_len = 0;

	screen->sta->caps_lock = false;
	screen->sta->shift_pressed = false;
	screen->sta->ctrl_pressed = false;
	screen->sta->alt_pressed = false;

	ft_memset(screen->screens, 0, sizeof(screen->screens));

	size_t	y = 0;
	while (y < VGA_HEIGHT)
	{
		size_t x = 0;
		while (x < VGA_WIDTH)
		{
			const size_t index = (y * VGA_WIDTH) + x;
			screen->terminal_buffer[index] = vga_entry(' ', screen->terminal_color);
			x++;
		}
		y++;
	}
	print_prompt(screen);
	for (size_t s = 0; s < NUM_SCREENS; ++s)
	{
		screen->screens[s].save_row = 0;
		screen->screens[s].save_column = 0;
		screen->screens[s].save_input_end = PROMPT_LENGTH;
		screen->screens[s].save_color = vga_entry_color(VGA_COLOR_LIGHT_RED2, VGA_COLOR_BLACK);	
		screen->screens[s].save_input_len = 0;
		ft_memcpy(screen->screens[s].save_buffer, (void *)screen->terminal_buffer, VGA_WIDTH * VGA_HEIGHT * sizeof(u16));
		ft_memcpy(screen->screens[s].save_input_buffer, (void *)screen->input_buffer, VGA_WIDTH * VGA_HEIGHT * sizeof(u16));
	}
	return (screen);
}

void	terminal_clear_screen(t_screen *screen)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            screen->terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', screen->terminal_color);

    screen->terminal_row = 0;
    screen->terminal_column = 0;
}

void	terminal_set_color(t_screen *screen, u8 color)
{
	screen->terminal_color = color;
}

void	set_cursor(u16 row, u16 col)
{
	u16	pos = row * 80 + col;

	outb(0x3D4, 0x0F);
	outb(0x3D5, pos & 0xFF);

	outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
}

void	terminal_putentry(t_screen *screen, char c, u8 color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	screen->terminal_buffer[index] = vga_entry(c, color);
}

void	terminal_scroll(t_screen *screen)
{
	size_t	bytes_copy = (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(u16);	
	ft_memcpy((void*)screen->terminal_buffer, (void*)(screen->terminal_buffer + VGA_WIDTH), bytes_copy);

	size_t	x = 0;
	while (x < VGA_WIDTH)
	{
		size_t	index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		screen->terminal_buffer[index] = vga_entry(' ', screen->terminal_color);
		++x;
	}
	screen->terminal_row = VGA_HEIGHT - 1;
	screen->terminal_column = 0;
}

void	terminal_putchar(t_screen *screen, char c)
{
	if (c == NEWLINE)
	{
		screen->terminal_row++;
		screen->terminal_column = 0;

		if (screen->terminal_row >= VGA_HEIGHT)
			terminal_scroll(screen);

		set_cursor(screen->terminal_row, screen->terminal_column);
	}
	else if (c == TAB)
	{
		screen->terminal_column += 4;
		printk(screen, "    ");
	}
	else
	{
		terminal_putentry(screen, c, screen->terminal_color, screen->terminal_column, screen->terminal_row);
		++screen->terminal_column;

		size_t max_col = (screen->terminal_row == 0) ? (VGA_WIDTH - 14) : VGA_WIDTH;
		
		if (screen->terminal_column >= max_col)
		{
			screen->terminal_column = 0;
			++screen->terminal_row;
			if (screen->terminal_row >= VGA_HEIGHT)
				terminal_scroll(screen);
		}
		set_cursor(screen->terminal_row, screen->terminal_column);
	}
}

void	terminal_write(t_screen *screen, const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(screen, data[i]);
}

void	clear_line(t_screen *screen)
{
	size_t	x = PROMPT_LENGTH;
	while (x < VGA_WIDTH)
	{
		terminal_putentry(screen, ' ', screen->terminal_color, x, screen->terminal_row);
		++x;
	}
	screen->terminal_column = PROMPT_LENGTH;
	set_cursor(screen->terminal_row, screen->terminal_column);
	print_prompt(screen);
}

void	handle_ctrl_c(t_screen *screen)
{
	printk(screen, "^C");
	screen->terminal_column = 0;
	screen->terminal_row++;
	
	if (screen->terminal_row >= VGA_HEIGHT)
		terminal_scroll(screen);
	
	print_prompt(screen);
	screen->input_end = PROMPT_LENGTH;
}

void	handle_backspace(t_screen *screen)
{
	if (screen->terminal_column > PROMPT_LENGTH)
	{
		--screen->input_len;
		screen->input_buffer[screen->input_len] = 0;

		--screen->terminal_column;
		terminal_putentry(screen, ' ', screen->terminal_color, screen->terminal_column, screen->terminal_row);
		set_cursor(screen->terminal_row, screen->terminal_column);
	
		if (screen->terminal_column < screen->input_end)
			screen->input_end = screen->terminal_column;
		
	}
}

void	handle_ctrl_l(t_screen *screen)
{
	terminal_clear_screen(screen);
	print_prompt(screen);
	screen->input_end = PROMPT_LENGTH;
}

void	handle_regular_char(t_screen *screen, char c)
{
	if (screen->sta->caps_lock && c >= 'a' && c <= 'z')
		c -= 32;

	screen->input_buffer[screen->input_len++] = c;
	screen->input_buffer[screen->input_len] = 0;

	terminal_putchar(screen, c);

	if (screen->terminal_column > screen->input_end)
		screen->input_end = screen->terminal_column;
}

void	handle_enter(t_screen *screen)
{
	terminal_putchar(screen, '\n');
	execute_command(screen, screen->input_buffer);
	ft_memset(screen->input_buffer, 0, sizeof(screen->input_buffer));
	screen->input_len = 0;
	print_prompt(screen);
	screen->input_end = PROMPT_LENGTH;
}

void	process_scancode(t_screen *screen, u8 scancode)
{
	char c;
	
	if (scancode == ENTER)
	{
		handle_enter(screen);
		return ;
	}
	
	if (screen->sta->shift_pressed)
		c = scancode_shift[scancode];
	else
		c = scancode_to_ascii[scancode];
	
	if (c == BACKSPACE)
		handle_backspace(screen);
	else if (c)
		handle_regular_char(screen, c);
}

void	handle_switch_terminal(t_screen *screen, u8 scancode)
{
	if (scancode == ALT_PRESS)
		screen->sta->alt_pressed = true;
	else if (scancode == ALT_RELEASE)
		screen->sta->alt_pressed = false;
	
	else if (screen->sta->alt_pressed && scancode == LEFT_ARROW)
	{
		size_t	new_screen = (screen->current_screen == 0) ? NUM_SCREENS - 1 : screen->current_screen - 1;
		switch_screen(screen, new_screen);
	}
	else if (screen->sta->alt_pressed && scancode == RIGHT_ARROW)
	{
		size_t	new_screen = (screen->current_screen + 1) % NUM_SCREENS;
		switch_screen(screen, new_screen);
	}
}

void	arrow_handler(t_screen *screen, u8 scancode)
{
	if (scancode == LEFT_ARROW)
	{
		if (screen->terminal_column > PROMPT_LENGTH)
		{
			--screen->terminal_column;
			set_cursor(screen->terminal_row, screen->terminal_column);
		}
	}
	else if (scancode == RIGHT_ARROW)
	{
		if (screen->terminal_column < screen->input_end && screen->terminal_column < VGA_WIDTH - 1)
		{
			++screen->terminal_column;
			set_cursor(screen->terminal_row, screen->terminal_column);
		}
	}
}

void	keyboard_handler_loop(t_screen *screen)
{
	while (1)
	{
		if (inb(0x64) & 1)
		{
			u8 scancode = inb(0x60);
			
			handle_switch_terminal(screen, scancode);
			if (!screen->sta->alt_pressed && (scancode == RIGHT_ARROW || scancode == LEFT_ARROW))
				arrow_handler(screen, scancode);
			else if (scancode == CTRL_PRESS)
				screen->sta->ctrl_pressed = true;
			else if (scancode == CTRL_RELEASE)
				screen->sta->ctrl_pressed = false;
			else if (screen->sta->ctrl_pressed && scancode == KEY_C)
				handle_ctrl_c(screen);
			else if (screen->sta->ctrl_pressed && scancode == KEY_L)
				handle_ctrl_l(screen);
			else if (scancode == SHIFT_LEFT || scancode == SHIFT_RIGHT)
				screen->sta->shift_pressed = true;
			else if (scancode == SHIFT_LEFT_R || scancode == SHIFT_RIGHT_R)
				screen->sta->shift_pressed = false;
			else if (scancode == CAPS_LOCK)
				screen->sta->caps_lock = !screen->sta->caps_lock;
			else if (scancode < 128 && !screen->sta->ctrl_pressed)
				process_scancode(screen, scancode);
		}
	}
}

void	terminal_write_string(t_screen *screen, const char *data)
{
	size_t i = 0;
	while (data[i])
	{
		if (data[i] == '\n')
			terminal_putchar(screen, '\n');
		else
			terminal_putchar(screen, data[i]);
		i++;
	}
}

void	print_prompt(t_screen *screen)
{
	u8 old_color = screen->terminal_color;
	terminal_set_color(screen, vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
	size_t i = 0;
	const char *prompt = "kfs -> ";
	while (prompt[i])
	{
		terminal_putchar(screen, prompt[i]);
		i++;
	}
	terminal_set_color(screen, old_color);
	draw_screen_index(screen);
	set_cursor(screen->terminal_row, PROMPT_LENGTH);
}

void	save_screen(t_screen *screen, size_t screen_id) 
{
	if (screen_id >= NUM_SCREENS)
		return ;

	ft_memcpy(screen->screens[screen_id].save_buffer, (void*)screen->terminal_buffer,
		   VGA_WIDTH * VGA_HEIGHT * sizeof(u16));

	screen->screens[screen_id].save_row = screen->terminal_row;
	screen->screens[screen_id].save_column = screen->terminal_column;
	screen->screens[screen_id].save_input_end = screen->input_end;
	screen->screens[screen_id].save_color = screen->terminal_color;
}

void	load_screen(t_screen *screen, size_t screen_id)
{
	if (screen_id >= NUM_SCREENS)
		return ;

	ft_memcpy((void*)screen->terminal_buffer, screen->screens[screen_id].save_buffer,
		   VGA_WIDTH * VGA_HEIGHT * sizeof(u16));

	screen->terminal_row = screen->screens[screen_id].save_row;
	screen->terminal_column = screen->screens[screen_id].save_column;
	screen->input_end = screen->screens[screen_id].save_input_end;
	screen->terminal_color = screen->screens[screen_id].save_color;
	if (screen->terminal_column == 0)
		screen->terminal_column = PROMPT_LENGTH;
	set_cursor(screen->terminal_row, screen->terminal_column);
}

void	switch_screen(t_screen *screen, size_t new_screen_id)
{
	if (new_screen_id >= NUM_SCREENS || new_screen_id == screen->current_screen)
		return ;
	
	save_screen(screen, screen->current_screen);
	screen->current_screen = new_screen_id;
	load_screen(screen, new_screen_id);

	draw_screen_index(screen);
}

void	draw_screen_index(t_screen *screen)
{
	const char	*text = "Screen  /  ";
	size_t		start_x = VGA_WIDTH - 13;
	u8			color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

	for (size_t	index = 0; text[index]; ++index)
	{
		if (index == 7)
			screen->terminal_buffer[start_x + index] = vga_entry('1' + screen->current_screen, color);
		else if (index == 9)
			screen->terminal_buffer[start_x + index] = vga_entry('0' + NUM_SCREENS, color);
		else
			screen->terminal_buffer[start_x + index] = vga_entry(text[index], color);
	}
}

void	need_help(t_screen *screen)
{
	terminal_set_color(screen, VGA_COLOR_LIGHT_BROWN);
	printk(screen, "If you don't know what to write, try 'help'\n");
	terminal_set_color(screen, VGA_COLOR_LIGHT_RED2);
}

void	kernel_main()
{
	gdt_init();

	t_screen	*screen = terminal_initialize();
	need_help(screen);
	print_prompt(screen);
	keyboard_handler_loop(screen);
}
