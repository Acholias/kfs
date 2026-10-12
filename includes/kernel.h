/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:10:49 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/12 13:33:10 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

#include "multiboot.h"
# include "types.h"
# include "vga.h"
# include "terminal.h"
# include "screens.h"
# include "keyboard.h"
# include "shell.h"
# include "libc.h"

#if defined(__LINUX__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

# define HEAP_START	0x1000000

extern u32	_kernel_end;

void	need_help(void);
void	kernel_main(u32 magic, t_multiboot_info *mbi);

#endif
