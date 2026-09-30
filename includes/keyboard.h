/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 16:01:39 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 16:31:04 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include "types.h"
# include "bool.h"

# define INPUT_MAX		256

# define PROMPT_LENGTH	12

# define CTRL_PRESS		0x1D
# define CTRL_RELEASE	0x9D
# define KEY_C			0x2E
# define KEY_L			0x26
# define KEY_TAB		0x0F
# define SHIFT_LEFT		0x2A
# define SHIFT_RIGHT	0x36
# define SHIFT_LEFT_R	0xAA
# define SHIFT_RIGHT_R	0xB6
# define CAPS_LOCK		0x3A
# define ALT_PRESS		0x38
# define ALT_RELEASE	0xB8
# define LEFT_ARROW		0x4B
# define RIGHT_ARROW	0x4D
# define BACKSPACE		'\b'
# define ENTER			0x1C

typedef enum e_layout
{
	LAYOUT_QWERTY,
	LAYOUT_AZERTY,
	LAYOUT_COUNT
}	t_layout;

typedef enum e_kbmod
{
	KB_NORMAL,
	KB_SHIFT,
	KB_ALTGR,
	KB_CTRL,
	KB_MOD_COUNT
}	t_kbmod;

typedef struct s_keymap
{
	const char	*tables[KB_MOD_COUNT];
}	t_keymap;

typedef struct s_kb_state
{
	t_layout	layout;
	bool		shift;
	bool		caps;
	bool		ctrl;
	bool		alt;
	char		input_buffer[INPUT_MAX];
	size_t		input_len;
	size_t		input_end;
}	t_kb_state;

void		keyboard_handler_loop(void);
void		keyboard_toggle_layout(void);
size_t		keyboard_get_input_end(void);
void		keyboard_set_input_end(size_t val);
void		keyboard_reset_input(void);
const char	*keyboard_get_input_buffer(void);

#endif
