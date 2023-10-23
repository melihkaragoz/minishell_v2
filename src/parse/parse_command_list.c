/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:03:55 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:04:02 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static void	init_redirections(t_command *command)
{
	command->redirection_heads[0] = NULL;
	command->redirection_heads[1] = NULL;
	command->redirection_heads[2] = NULL;
	command->redirection_heads[3] = NULL;
	command->redirection_tails[0] = NULL;
	command->redirection_tails[1] = NULL;
	command->redirection_tails[2] = NULL;
	command->redirection_tails[3] = NULL;
}

t_command	*create_command_node(void)
{
	t_command	*new;

	new = malloc(sizeof(t_command));
	new->infile = NULL;
	new->outfile = NULL;
	new->next = NULL;
	new->prev = NULL;
	init_redirections(new);
	return (new);
}

void	add_command_node(t_command *new)
{
	if (g_data.command_head == NULL)
	{
		new->prev = NULL;
		g_data.command_head = new;
		g_data.command_tail = new;
	}
	else
	{
		new->prev = g_data.command_tail;
		g_data.command_tail->next = new;
		g_data.command_tail = new;
	}
}

void	cmd_clear(t_redirection **redir, t_redirection **freeable_redir,
					t_command **freeable, int i)
{
	*redir = (*freeable)->redirection_heads[i];
	while (*redir != NULL)
	{
		*freeable_redir = *redir;
		*redir = (*redir)->next;
		smart_free((*freeable_redir)->key);
		smart_free(*freeable_redir);
	}
}

void	clear_command_list(void)
{
	t_command		*temp;
	t_command		*freeable;
	t_redirection	*redir;
	t_redirection	*freeable_redir;
	int				i;

	temp = g_data.command_head;
	while (temp != NULL)
	{
		freeable = temp;
		temp = temp->next;
		i = -1;
		while (++i < 4)
			cmd_clear(&redir, &freeable_redir, &freeable, i);
		smart_free_strs(freeable->command);
		smart_free(freeable);
	}
	g_data.command_head = NULL;
	g_data.command_tail = NULL;
}
