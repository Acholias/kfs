/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_add_front.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 20:12:08 by lumugot           #+#    #+#             */
/*   Updated: 2026/11/19 09:22:37 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libk.h"

void	ft_list_add_front(t_list **list, t_list *node)
{
	if (!list || !*list)
		return ;
	node->next = *list;
	*list = node;
}
