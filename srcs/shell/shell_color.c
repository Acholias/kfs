/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumugot <lumugot@42angouleme.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/03 21:57:24 by lumugot           #+#    #+#             */
/*   Updated: 2026/10/03 22:38:55 by lumugot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/kernel.h"

void	display_color_panel()
{
	printk("1-		BLACK");
	printk("2-		BLUE");
	printk("3-		GREEN");
	printk("5-		CYAN");
	printk("6-		RED");
	printk("7-		MAGENTA");
	printk("8-		BROWN");
	printk("9-		LIGHT_GREY");
	printk("10-		DARK_GREY");
	printk("11-		LIGHT_BLUE");
	printk("12-		LIGHT_GREEN");
	printk("13-		LIGHT_CYAN");
	printk("14-		LIGHT_RED2");
	printk("15-		LIGHT_MAGENTA");
	printk("16-		LIGHT_BROWN");
	printk("17-		WHITE");

	printk("/n/n Select number for apply color in terminal :  ");
}

void	color_command()
{

}
