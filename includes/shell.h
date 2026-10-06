/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 21:53:17 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/06 11:57:12 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "bool.h"

# define COLOR_COUNT	16

// utils.c
int		ft_strncmp(const char *s1, const char *s2, size_t len);
int		ft_atoi(const char *str);
void	print_helper(void);
void	invalid_command(const char *cmd);

//shell_color.c
bool	shell_is_awaiting_color(void);
void	shell_set_awaiting_color(bool value);
void	display_color_panel(void);
void	color_command(const char *num);

//shell.c
size_t	get_cmd(const char *cmd);
void	execute_command(const char *cmd);
