/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paging.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 13:12:05 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/09 21:10:35 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PADING_H
# define PADING_H

# include "types.h"
# include "bool.h"
# include "pmm.h"

# define PAGE_PRESENT		0x1
# define PAGE_RW			0x2
# define PAGE_USER			0x4

# define PD_ENTRIES			1024
# define PT_ENTRIES			1024
# define TABLE_COVERAGE		(PT_ENTRIES * FRAME_SIZE)

typedef struct	s_paging
{
	u32	*directory;
	u32	n_tables;
}	t_paging;

void		paging_init(t_paging *pg, t_pmm *pmm);
void		paging_enable(t_paging *pg);
bool		paging_is_enabled(void);
void		print_paging(t_paging *pg);
t_paging	*get_paging(void);

#endif
