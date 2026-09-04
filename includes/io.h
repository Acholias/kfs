/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 21:10:47 by lumugot           #+#    #+#             */
/*   Updated: 2026/07/24 21:11:03 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_H
# define IO_H

#include "types.h"

static __inline__
void	outb(u16 port, u8 val)
{
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static __inline__
u8	inb(u16 port)
{
	u8	ret;

    __asm__ volatile ( "inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");	

	return (ret);
}

static __inline__
void	outw(u16 port, u16 value)
{
	__asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

#endif
