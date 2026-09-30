/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screens.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 15:57:40 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 16:12:09 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCREENS_H
# define SCREENS_H

# include "types.h"
# include "vga.h"

# define NUM_SCREENS	3

typedef struct	s_screen
{
	size_t	save_row;
	size_t	save_column;
	size_t	save_input_end;
	u8		save_color;
	u16		save_buffer[VGA_WIDTH * VGA_HEIGHT];
}	t_screen;

void	screens_init(void);
void	screens_switch(size_t new_screen_id);
void	screens_draw_index(void);
size_t	screens_current(void);

#endif
