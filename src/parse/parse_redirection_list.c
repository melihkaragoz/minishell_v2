/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection_list.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:17:17 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 05:11:29 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

t_redirection	*create_redirection_node(char *key, int type)
{
	t_redirection	*new;

	new = malloc(sizeof(t_redirection));
	new->type = type;
	if (type == INFILE)
		new->fd[0] = open(key, O_RDONLY, 0777);
	else if (type == OUTFILE)
		new->fd[0] = open(key, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (type == HEREDOC)
		pipe(new->fd);
	else if (type == APPEND)
		new->fd[0] = open(key, O_WRONLY | O_CREAT | O_APPEND, 0777);
	new->key = ft_strdup(key);
	new->next = NULL;
	return (new);
}

void	add_redirection_node(t_command *command, t_redirection *new, int type)
{
	if (command->redirection_heads[type - 1] == NULL)
	{
		command->redirection_heads[type - 1] = new;
		command->redirection_tails[type - 1] = new;
	}
	else
	{
		command->redirection_tails[type - 1]->next = new;
		command->redirection_tails[type - 1] = new;
	}
	if (type == HEREDOC || type == INFILE)
		command->infile = new;
	else if (type == OUTFILE || type == APPEND)
		command->outfile = new;
	close_all_redirections();
}

void	close_all_redirections(void)
{
	t_command		*temp;
	t_redirection	*redir_temp;
	int				i;

	temp = g_data.command_head;
	while (temp != NULL)
	{
		i = 0;
		while (i < 4)
		{
			redir_temp = temp->redirection_heads[i];
			while (redir_temp != NULL)
			{
				close(redir_temp->fd[0]);
				close(redir_temp->fd[1]);
				redir_temp = redir_temp->next;
			}
			i++;
		}
		temp = temp->next;
	}
}

int	set_commands_util(int *i, t_parse_node **tmp,
								t_redirection **redir, t_command **cmd)
{
	while ((*tmp) && (*tmp)->type != PIPPE)
	{
		(*redir) = if_redir((*tmp));
		if ((*redir) != NULL)
		{
			add_redirection_node((*cmd), (*redir), (*redir)->type);
			close_all_redirections();
			if ((*tmp)->next->next)
			{
				(*tmp) = (*tmp)->next->next;
				continue ;
			}
			else
			{
				(*cmd)->command[*i] = NULL;
				add_command_node((*cmd));
				return (1);
			}
		}
		(*cmd)->command[*i] = ft_strdup((*tmp)->word);
		(*tmp) = (*tmp)->next;
		(*i)++;
	}
	return (0);
}

int	set_commands(void)
{
	t_parse_node	*tmp;
	t_command		*cmd;
	t_redirection	*redir;
	int				i;

	tmp = g_data.parse_head;
	cmd = g_data.command_head;
	redir = NULL;
	while (tmp && tmp->word)
	{
		i = 0;
		cmd = create_command_node();
		cmd->command = alloc_cmd(tmp);
		if (set_commands_util(&i, &tmp, &redir, &cmd))
			return (1);
		cmd->command[i] = NULL;
		add_command_node(cmd);
		if (tmp && tmp->type == PIPPE)
			tmp = tmp->next;
	}
	return (1);
}
