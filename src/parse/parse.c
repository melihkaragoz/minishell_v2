/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 03:42:31 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 05:16:42 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static void	env_resulation(void)
{
	t_parse_node	*temp;
	int				i;
	int				quote_mode;

	temp = g_data.parse_head;
	while (temp != NULL)
	{
		i = 0;
		quote_mode = NO_Q;
		while (temp->word[0] != '\0' && temp->word[i])
		{
			if (quote_mode != SINGLE_Q && temp->word[i] == '$')
			{
				dollar(&temp->word, i);
				i = 0;
				continue ;
			}
			if (temp->word[i] == '\'')
				quote_mode = SINGLE_Q;
			else if (temp->word[i] == '"')
				quote_mode = DOUBLE_Q;
			i++;
		}
		temp = temp->next;
	}
}

void	ms_set_quote_mode(int set, int *i)
{
	g_data.quote_mode = set;
	g_data.arg_mode = 1;
	if (g_data.line[(*i) + 1] == set)
	{
		(*i)++;
		g_data.quote_mode = 0;
	}
}

int	ms_set_arg_false(int i, int *j)
{
	if (i == 4 || i == 1 || i == 2 || i == 3)
		(*j)++;
	if (i == 4 || i == 1 || i == 2 || i == 0)
		g_data.arg_mode = false;
	if (i == 7 || i == 5)
		g_data.quote_mode = 0;
	if (i == 3)
		ms_set_quote_mode(g_data.line[*j], j);
	if (g_data.double_redirection == true && (i == 4))
	{
		(*j)++;
		g_data.double_redirection = false;
	}
	return (1);
}

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

int	parse(char *rl)
{
	int	i;

	i = 0;
	if (check_missing_quotes(rl))
	{
		perror("Missing quote!\n");
		free(rl);
		return (1);
	}
	trim(&rl);
	seperate(rl, &i);
	env_resulation();
	ms_set_nodes(&i);
	set_commands();
	parse_access();
	free(g_data.line);
	return (0);
}
