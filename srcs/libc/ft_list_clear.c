/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_clear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 20:26:00 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 20:28:23 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libc.h"

void	list_clear(t_heap *heap, t_list **list, t_del_fn del)
{
	t_list	*next;

	if (!list)
		return ;
	while (*list)
	{
		next = (*list)->next;
		ft_list_delone(heap, *list, del);
		*list = next;
	}
}
