/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:15:25 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 05:05:20 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static void	ms_node_remove_char(t_parse_node *tmp, int i)
{
	char	*tmp_str;
	char	*tmp_str_extra;
	int		len;

	if (tmp->word && *tmp->word)
	{
		len = ft_strlen(tmp->word);
		tmp_str = ft_substr(tmp->word, 0, i);
		tmp_str_extra = ft_substr(tmp->word, i + 1, len - i);
		free(tmp->word);
		tmp->word = ft_strjoin(tmp_str, tmp_str_extra);
		free(tmp_str);
		free(tmp_str_extra);
	}
}

static void	ms_remove_quotes(t_parse_node *tmp, int *i)
{
	int	start_pos_quo;
	int	quo;

	quo = 0;
	*i = 0;
	while (tmp->word[(*i)])
	{
		if (quo && tmp->word[(*i)] == quo)
		{
			quo = 0;
			ms_node_remove_char(tmp, start_pos_quo);
			ms_node_remove_char(tmp, (*i) - 1);
			(*i) -= 2;
		}
		else if (!quo && (tmp->word[(*i)] == '\'' || tmp->word[(*i)] == '\"'))
		{
			quo = tmp->word[(*i)];
			start_pos_quo = (*i);
		}
		(*i)++;
	}
}

void	ms_set_nodes(int *i)
{
	t_parse_node	*tmp;

	tmp = g_data.parse_head;
	while (tmp)
	{
		tmp->type = 0;
		if (ms_node_check_redirection(tmp->word))
			tmp->type = ms_node_check_redirection(tmp->word);
		else if (ft_strncmp("|", tmp->word, 2) == 0)
			tmp->type = PIPPE;
		ms_remove_quotes(tmp, i);
		if (ms_node_check_builtin(tmp->word))
			tmp->type = BUILTIN;
		tmp = tmp->next;
	}
}

int	ms_end_of_word(int *i)
{
	if (ms_check_seperators(&g_data.line[(*i)]) && !g_data.quote_mode)
		return (ms_set_arg_false(4, i), 1);
	while (g_data.quote_mode && g_data.line[(*i)] && (*i)++)
	{
		if (g_data.line[(*i)] == g_data.quote_mode)
		{
			if (g_data.line[(*i) + 1] && (g_data.line[(*i) + 1] == ' ' || \
				ms_check_seperators(&g_data.line[(*i) + 1])))
				return (ms_set_arg_false(5, i), 1);
			else if (g_data.line[(*i) + 1] && (g_data.line[(*i) + 1] == 39 || \
				g_data.line[(*i) + 1] == 34))
				return (ms_set_arg_false(3, i), 0);
			else
				return (ms_set_arg_false(7, i), 0);
		}
	}
	if (g_data.line[(*i)] && !g_data.quote_mode)
		return (ms_if_not_quote(i));
	return (0);
}

int	check_missing_quotes(char *rl)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (rl[++i])
	{
		if (quote == 0 && (rl[i] == 34 || rl[i] == 39))
			quote = rl[i];
		else if (quote == rl[i])
			quote = 0;
	}
	return (!!quote);
}
