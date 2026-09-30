/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:10:49 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/30 16:08:55 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

# include "types.h"
# include "vga.h"
# include "terminal.h"
# include "screens.h"
# include "keyboard.h"
# include "shell.h"

#if defined(__LINUX__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

extern	size_t	ft_strlen(const char *str);
extern	void	*ft_memcpy(void *dest, const void *src, size_t n);
extern	void	ft_memset(void *s, int c, size_t n);

int		printk(const char *str, ...);
void	need_help(void);
void	kernel_main(void);

#endif
