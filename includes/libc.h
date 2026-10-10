/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 19:32:13 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/10 20:31:22 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBC_H
# define LIBC_H

# include "types.h"
# include "heap.h"

typedef struct	s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef void	(t_del_fn)(t_heap *heap, void *content);

// All list function
t_list	*ft_list_new(t_heap *heap, void *content);
int		ft_list_size(t_list *list);
void	ft_list_add_back(t_list **list, t_list *node);
void	ft_list_add_front(t_list **list, t_list *node);
void	ft_list_delone(t_heap *heap, t_list *node, t_del_fn del);
void	ft_list_clear(t_heap *heap, t_list **list, t_del_fn del);

void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t);
size_t	ft_strlen(const char *str);
int		printk(const char *str, ...);

#endif
