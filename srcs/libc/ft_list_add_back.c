/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_add_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 20:15:31 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 20:19:14 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libc.h"

void	ft_list_add_back(t_list **list, t_list *node)
{
	t_list	*temp;

	if (!list)
		return ;
	if (!*list)
	{
		*list = node;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = node;
}
