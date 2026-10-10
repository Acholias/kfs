/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 19:27:15 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 19:43:03 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/types.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			index;
	unsigned char	*s1;
	unsigned char	*s2;

	index = 0;
	s1 = (unsigned char *)dest;
	s2 = (unsigned char *)src;
	if (s1 == NULL || s2 == NULL)
		return (dest);
	while (index < n)
	{
		s1[index] = s2[index];
		index++;
	}
	return (dest);
}
