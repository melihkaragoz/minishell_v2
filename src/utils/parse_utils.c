/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:41:32 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:42:05 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	error_exit(char *message, char *str, int exit_status)
{
	printf("minishell: %s: %s\n", str, message);
	g_data.exit_status = exit_status;
	return (0);
}

int	is_special_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

t_redirection	*if_redir(t_parse_node *temp)
{
	if (temp->type == HEREDOC)
		return (create_redirection_node(temp->next->word, HEREDOC));
	else if (temp->type == APPEND)
		return (create_redirection_node(temp->next->word, APPEND));
	else if (temp->type == INFILE)
		return (create_redirection_node(temp->next->word, INFILE));
	else if (temp->type == OUTFILE)
		return (create_redirection_node(temp->next->word, OUTFILE));
	return (NULL);
}

int	is_redir(t_parse_node *temp)
{
	if (ft_strncmp(temp->word, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(temp->word, ">>", 2) == 0)
		return (APPEND);
	else if (ft_strncmp(temp->word, "<", 1) == 0)
		return (INFILE);
	else if (ft_strncmp(temp->word, ">", 1) == 0)
		return (OUTFILE);
	return (-1);
}
