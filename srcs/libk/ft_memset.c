/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 19:28:19 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 19:30:52 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/types.h"

 void	*ft_memset(void *s, int c, size_t n)
 {
	size_t	index;
	char	*str;

	index = 0;
	str = (char *)s;
	while (index < n)
	{
 		str[index] = c;
		index++;
	}
	return (s);
}
