#ifndef KERNEL_H
# define KERNEL_H

# include "types.h"

# define VGA_WIDTH		80
# define VGA_HEIGHT		25
# define VGA_MEMORY		0xB8000

# define INPUT_MAX		256

# define CTRL_PRESS		0x1D
# define CTRL_RELEASE	0x9D
# define KEY_C			0x2E
# define KEY_L			0x26
# define SHIFT_LEFT		0x2A
# define SHIFT_RIGHT	0x36
# define SHIFT_LEFT_R	0xAA
# define SHIFT_RIGHT_R	0xB6
# define CAPS_LOCK		0x3A
# define ALT_PRESS		0x38
# define ALT_RELEASE	0xB8
# define LEFT_ARROW		0x4B
# define RIGHT_ARROW	0x4D
# define NUM_SCREENS	2
# define NEWLINE		'\n'
# define BACKSPACE		'\b'
# define ENTER			0x1C

enum vga_color
{
	VGA_COLOR_BLACK,
	VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_MAGENTA,
	VGA_COLOR_BROWN,
	VGA_COLOR_LIGHT_GREY,
	VGA_COLOR_DARK_GREY,
	VGA_COLOR_LIGHT_BLUE,
	VGA_COLOR_LIGHT_GREEN,
	VGA_COLOR_LIGHT_CYAN,
	VGA_COLOR_LIGHT_RED2,
	VGA_COLOR_LIGHT_MAGENTA,
	VGA_COLOR_LIGHT_BROWN,
	VGA_COLOR_WHITE,
};

// Struct for screen data
typedef struct	s_screen
{
	size_t	terminal_row;
	size_t	terminal_column;
	size_t	prompt_length;
	size_t	input_end;
	u8		terminal_color;
	u16		*terminal_buffer;
}	t_screen;

// Struct for save screen data (for switch terminal)
typedef struct	s_save_screen
{
	size_t		save_row;
	size_t		save_column;
	size_t		save_input_end;
	u8			save_color;
	u16			save_buffer[VGA_WIDTH * VGA_HEIGHT];
}	t_save_screen;

// LIBC functions in ASM
extern	size_t		ft_strlen(const char *str);
extern	void		*ft_memcpy(void *dest, const void *src, size_t n);
extern	void		ft_memset(void *s, int c, size_t n);	

// printk.c
int		printk(t_screen *screen, const char *str, ...);

// kernel.c
t_screen	*terminal_initialize(void);
void		terminal_set_color(t_screen *screen, u8 color);
void		set_cursor(u16 row, u16 col);
void		terminal_putentry(t_screen *screen, char c, u8 color, size_t x, size_t y);
void		terminal_clear_screen(t_screen *screen);
void		terminal_scroll(t_screen *screen);
void		terminal_putchar(t_screen *screen, char c);
void		terminal_write(t_screen *screen, const char *data, size_t size);
void		clear_line(t_screen *screen);
void		handle_ctrl_c(t_screen *screen);
void		handle_backspace(t_screen *screen);
void		handle_ctrl_l(t_screen *screen);
void		handle_regular_char(t_screen *screen, char c);
void		process_scancode(t_screen *screen, u8 scancode);
void		handle_switch_terminal(t_screen *screen, u8 scancode);
void		arrow_handler(t_screen *screen, u8 scancode);
void		keyboard_handler_loop(t_screen *screen);
void		terminal_write_string(t_screen *screen, const char *data);
void		print_prompt(t_screen *screen);
void		save_screen(t_screen *screen, size_t screen_id); 
void		load_screen(t_screen *screen, size_t screen_id);
void		switch_screen(t_screen *screen, size_t new_screen_id);
void		draw_screen_index(t_screen *screen);

// shell.c
void		execute_command(t_screen *screen, const char *cmd);

#endif
