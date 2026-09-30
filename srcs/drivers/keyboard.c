/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:38:23 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 18:42:51 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/keyboard.h"
#include "../../includes/terminal.h"
#include "../../includes/screens.h"
#include "../../includes/kernel.h"
#include "../../includes/io.h"

static const char	qwerty_normal[128] = {
	0,27,'1','2','3','4','5','6','7','8',
	'9','0','-','=','\b','\t',
	'q','w','e','r','t','y','u','i','o','p',
	'[',']',0,0,
	'a','s','d','f','g','h','j','k','l',';',
	'\'','`',0,'\\','z','x','c','v','b','n',
	'm',',','.','/',0,'*',0,' ',0,0
};

static const char	qwerty_shift[128] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+', '\b', '\t',
	'Q','W','E','R','T','Y','U','I','O','P',
	'{','}',0,0,
	'A','S','D','F','G','H','J','K','L',':',
	'"','~',0,'|','Z','X','C','V','B','N',
	'M','<','>','?',0,'*',0,' '
};

static const char	azerty_normal[128] = {
	0, 27, '&', '\x82', '"', '\'', '(', '-', '\x8A', '_',
	'\x87', '\x85', ')', '=', '\b', '\t',
	'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'^', '$', 0, 0,
	'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
	'\x97', '\xFD', 0, '*', 'w', 'x', 'c', 'v', 'b', 'n',
	',', ';', ':', '!', 0, '*', 0, ' ', 0, 0
};

static const char	azerty_shift[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '\xF8', '+', '\b', '\t',
	'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'^', '$', 0, 0,
	'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M',
	'%', '\xFD', 0, '*', 'W', 'X', 'C', 'V', 'B', 'N',
	'?', '.', '/', '\xA7', 0, '*', 0, ' '
};

static const t_keymap	keymaps[LAYOUT_COUNT] = {
	[LAYOUT_QWERTY] = {.tables = {qwerty_normal, qwerty_shift, NULL, NULL}},
	[LAYOUT_AZERTY] = {.tables = {azerty_normal, azerty_shift, NULL, NULL}},
};

static t_kb_state	kb = {
	.layout = LAYOUT_AZERTY,
	.input_end = PROMPT_LENGTH
};

size_t	keyboard_get_input_end(void)
{
	return (kb.input_end);
}

void	keyboard_set_input_end(size_t val)
{
	kb.input_end = val;
}

void	keyboard_reset_input(void)
{
	ft_memset(kb.input_buffer, 0, sizeof(kb.input_buffer));
	kb.input_len = 0;
	kb.input_end = PROMPT_LENGTH;
}

const char	*keyboard_get_input_buffer(void)
{
	return (kb.input_buffer);
}

void	keyboard_toggle_layout(void)
{
	kb.layout = (kb.layout == LAYOUT_QWERTY) ? LAYOUT_AZERTY : LAYOUT_QWERTY;
	
	terminal_set_color(VGA_COLOR_LIGHT_BLUE);
	printk("\n\n[Keyboard Language] -> %s \n\n", kb.layout == LAYOUT_QWERTY ? "QWERTY" : "AZERTY");
	terminal_set_color(VGA_COLOR_LIGHT_RED2);

	keyboard_reset_input();
	print_prompt();
}

static char	scancode_to_char(u8 scancode)
{
	t_kbmod		mod;
	const char	*table;

	mod = kb.shift ? KB_SHIFT : KB_NORMAL;
	table = keymaps[kb.layout].tables[mod];
	if (!table)
		table = keymaps[kb.layout].tables[KB_NORMAL];
	return (table[scancode]);
}

static void	handle_backspace(void)
{
	t_term_state	state;
	
	state = terminal_get_state();
	if (state.col > PROMPT_LENGTH)
	{
		--kb.input_len;
		kb.input_buffer[kb.input_len] = 0;
		--state.col;
		terminal_putentry(' ', state.color, state.col, state.row);
		terminal_set_state(state);
		vga_set_cursor(state.row, state.col);
		if (state.col < kb.input_end)
			kb.input_end = state.col;
	}
}

static void	handle_regular_char(char c)
{
	t_term_state	state;

	if (kb.caps && c >= 'a' && c <= 'z')
		c -= 32;
	kb.input_buffer[kb.input_len++] = c;
	kb.input_buffer[kb.input_len] = 0;
	terminal_putchar(c);
	state = terminal_get_state();
	if (state.col > kb.input_end)
		kb.input_end = state.col;
}

static void	handle_ctrl_c(void)
{
	t_term_state	state; 
	u8				old_color;

	state = terminal_get_state();
	old_color = state.color;
	
	state.color = vga_entry_color(VGA_COLOR_LIGHT_RED2, VGA_COLOR_BLACK);
	terminal_set_state(state);
	terminal_putentry('^', state.color, state.col, state.row);
	state.col++;
	
	terminal_putentry('C', state.color, state.col, state.row);
	
	state.col++;
	state.color = old_color;
	state.col = 0;
	state.row++;
	terminal_set_state(state);
	
	if (state.row >= VGA_HEIGHT)
		terminal_scroll();
	keyboard_reset_input();
	print_prompt();
}

static void	handle_ctrl_l(void)
{
	terminal_clear_screen();
	print_prompt();
	keyboard_reset_input();
}

static void	handle_enter(void)
{
	terminal_putchar('\n');
	execute_command(kb.input_buffer);
	keyboard_reset_input();
	print_prompt();
}

static void	process_scancode(u8 scancode)
{
	char	c;

	if (scancode == ENTER)
	{
		handle_enter();
		return ;
	}
	c = scancode_to_char(scancode);
	if (c == BACKSPACE)
		handle_backspace();
	else if (c)
		handle_regular_char(c);
}

static void	arrow_handler(u8 scancode)
{
	t_term_state	state;
	
	state = terminal_get_state();
	if (scancode == LEFT_ARROW && state.col > PROMPT_LENGTH)
	{
		state.col--;
		terminal_set_state(state);
		vga_set_cursor(state.row, state.col);
	}
	else if (scancode == RIGHT_ARROW && state.col < kb.input_end && state.col < VGA_WIDTH - 1)
	{
		state.col++;
		terminal_set_state(state);
		vga_set_cursor(state.row, state.col);
	}
}

void	keyboard_handler_loop(void)
{
	u8		scancode;
	size_t	new_screen;

	while (1)
	{
		if (!(inb(0x64) & 1))
			continue ;
		scancode = inb(0x60);
		if (scancode == ALT_PRESS)
			kb.alt = true;
		else if (scancode == ALT_RELEASE)
			kb.alt = false;
		else if (kb.alt && scancode == LEFT_ARROW)
		{
			new_screen = (screens_current() == 0) ? NUM_SCREENS - 1 : screens_current() - 1;
			screens_switch(new_screen);
		}
		else if (kb.alt && scancode == RIGHT_ARROW)
		{
			new_screen = (screens_current() + 1) % NUM_SCREENS;
			screens_switch(new_screen);
		}
		else if (!kb.alt && (scancode == RIGHT_ARROW || scancode == LEFT_ARROW))
			arrow_handler(scancode);
		else if (scancode == CTRL_PRESS)
			kb.ctrl = true;
		else if (scancode == CTRL_RELEASE)
			kb.ctrl = false;
		else if (kb.shift && scancode == KEY_TAB)
			keyboard_toggle_layout();
		else if (kb.ctrl && scancode == KEY_C)
			handle_ctrl_c();
		else if (kb.ctrl && scancode == KEY_L)
			handle_ctrl_l();
		else if (scancode == SHIFT_LEFT || scancode == SHIFT_RIGHT)
			kb.shift = true;
		else if (scancode == SHIFT_LEFT_R || scancode == SHIFT_RIGHT_R)
			kb.shift = false;
		else if (scancode == CAPS_LOCK)
			kb.caps = !kb.caps;
		else if (scancode < 128 && !kb.ctrl)
			process_scancode(scancode);
	}
}
