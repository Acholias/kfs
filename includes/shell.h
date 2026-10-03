/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 21:53:17 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 22:39:23 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "bool.h"

// utils.c
void	print_helper(void);
void	invalid_command(const char *cmd);

void	execute_command(const char *cmd);
