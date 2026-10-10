/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_size.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 20:09:05 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 20:11:36 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libc.h"

int	ft_list_size(t_list *list)
{
	int size;

	size = 0;
	while (list)
	{
		size++;
		list = list->next;
	}
	return (size);
}
