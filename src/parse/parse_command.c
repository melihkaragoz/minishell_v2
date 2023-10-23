/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:12:11 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:12:11 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

void	if_pipe_or_end(t_parse_node **start, t_parse_node **temp,
						int *i, t_command *new)
{
	char	**command;

	command = malloc(sizeof(char *) * (*i + 1));
	command[*i] = NULL;
	(*i) = 0;
	while (*start != NULL && ft_strncmp((*start)->word, "|", 1) != 0)
	{
		if (is_redir(*start) != -1)
		{
			(*start) = (*start)->next->next;
			continue ;
		}
		command[*i] = ft_strdup((*start)->word);
		(*start) = (*start)->next;
		(*i)++;
	}
	(*i) = 0;
	new->command = command;
	add_command_node(new);
	if (temp != NULL && (*temp) != NULL)
	{
		(*temp) = (*temp)->next;
		(*start) = (*temp);
	}
}

int	check_commands(void)
{
	t_parse_node	*temp;

	temp = g_data.parse_head;
	while (temp != NULL)
	{
		if (is_redir(temp) != -1)
			if (temp->next == NULL || is_special_char(temp->next->word[0]))
				return (error_exit("syntax error token`newline'", "", 258));
		if (ft_strncmp(temp->word, "|", 1) == 0)
			if (temp->next == NULL || temp->prev == NULL)
				return (error_exit("syntax error token `newline'", "", 258));
		temp = temp->next;
	}
	return (1);
}

void	iterate_parse_nodes(t_parse_node *temp, t_command *command,
								t_parse_node *start, t_redirection *redir)
{
	int	i;

	i = 0;
	while (temp != NULL)
	{
		redir = if_redir(temp);
		if (redir != NULL)
		{
			add_redirection_node(command, redir, redir->type);
			temp = temp->next->next;
		}
		else if (temp->type == PIPPE)
		{
			if_pipe_or_end(&start, &temp, &i, command);
			command = create_command_node();
		}
		else
		{
			i++;
			temp = temp->next;
		}
	}
	if_pipe_or_end(&start, &temp, &i, command);
}

void	parse_commands(void)
{
	t_redirection	*redir;
	t_command		*command;

	redir = NULL;
	command = create_command_node();
	iterate_parse_nodes(g_data.parse_head, command, g_data.parse_head, redir);
}
