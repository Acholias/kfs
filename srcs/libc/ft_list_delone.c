/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_list_delone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 20:19:33 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 20:25:56 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libc.h"

void	ft_list_delone(t_heap *heap, t_list *node, t_del_fn del)
{
	if (!node)
		return ;
	if (del)
		del(heap, node->content);
	kfree(heap, node);
}
