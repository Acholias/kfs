/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vargs.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 20:10:58 by lumugot           #+#    #+#             */
/*   Updated: 2026/07/24 21:10:54 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARGS_H
# define VARGS_H

typedef __builtin_va_list	va_list;

# define	va_start(ap, last)	__builtin_va_start(ap, last)
# define	va_arg(ap, type)	__builtin_va_arg(ap, type)
# define	va_copy(dest, src)	__builtin_va_copy(dest, src)
# define	va_end(ap)			__builtin_va_end(ap)

#endif
