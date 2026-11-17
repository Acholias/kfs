/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/17 12:27:40 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/17 12:28:56 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PANIC_H
# define PANIC_H

typedef enum e_panic_level
{
	PANIC_WARNING,
	PANIC_FATAL,
}	t_panic_level;

void	kpanic(t_panic_level level, const char *message);

#endif
