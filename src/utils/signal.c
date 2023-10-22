/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:30:33 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:30:34 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

void	sigint_handler(int signum)
{
	(void)signum;
	g_data.sig = 1;
	write(1, "\033[A", 3);
	ioctl(0, TIOCSTI, "\n");
}

void	sigquit_handler(int signum)
{
	(void)signum;
	if (g_data.quit_flag == 1)
	{
		write(1, "\033[2D", 4);
		write(1, "  ", 2);
		write(1, "\033[2D", 4);
		ioctl(-1, TIOCSTI, "\n");
		g_data.quit_flag = 0;
	}
	g_data.quit_flag = 1;
}
