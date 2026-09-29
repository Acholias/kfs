/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 09:58:30 by lumugot           #+#    #+#             */
/*   Updated: 2026/09/10 10:22:46 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMM_H
# define PMM_H

# include "types.h"
# include "multiboot.h"

# define FRAME_SIZE	4096

typedef struct s_pmm
{
	u8	*bitmap;
	u32	total_frames;
	u32	bitmap_size;
}	t_pmm;

void	pmm_init(t_pmm *pmm, t_multiboot_info *mbi, u32 kernel_end);
u32		pmm_alloc_frame(t_pmm *pmm);
void	pmm_free_frame(t_pmm *pmm, u32 frame_addr);
void	pmm_mark_used(t_pmm *pmm, u32 addr, u32 size);
void	pmm_mark_free(t_pmm *pmm, u32 addr, u32 size);
void	print_pmm(t_pmm *pmm);

#endif
