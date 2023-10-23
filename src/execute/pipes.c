/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:50:25 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:50:32 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

void	open_pipes(void)
{
	t_command	*curr;

	curr = g_data.command_head;
	while (curr && curr->next != NULL)
	{
		pipe(curr->pipe_fds);
		curr = curr->next;
	}
}

void	close_pipes(void)
{
	t_command	*curr;

	curr = g_data.command_head;
	while (curr->next != NULL)
	{
		close(curr->pipe_fds[0]);
		close(curr->pipe_fds[1]);
		curr = curr->next;
	}
}
