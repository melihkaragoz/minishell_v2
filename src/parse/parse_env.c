/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:12:32 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:14:11 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

char	*get_left(int pos, char **str)
{
	char	*rtn;
	int		start;
	int		len;

	len = 0;
	while (ft_isval((*str)[pos]) == 1)
		pos++;
	start = pos;
	while ((*str)[pos] != '\0')
	{
		pos++;
		len++;
	}
	rtn = ft_substr(*str, start, len);
	return (rtn);
}

void	add_left(char *replace, char **str, int pos)
{
	char	*left;

	left = get_left(pos, str);
	*str = smart_dup(replace, *str);
	if (left[0] != '\0')
		*str = smart_join(*str, left, *str);
	smart_free(left);
	smart_free(replace);
}

static void	put_env(char **word, int pos)
{
	char	*left;
	char	*key;
	char	*value;
	int		start;

	start = pos;
	while (ft_isval((*word)[pos]) == 1)
		pos++;
	left = get_left(start, word);
	key = ft_substr(*word, start, pos - start);
	value = get_env(key);
	smart_free(key);
	if (value == NULL)
	{
		*word = smart_dup(left, *word);
		return ;
	}
	*word = smart_dup(value, *word);
	if (left[0] != '\0')
		*word = smart_join(*word, left, *word);
	smart_free(value);
	smart_free(left);
}

void	dollar(char **word, int i)
{
	char	*start;

	start = ft_substr(*word, 0, i);
	if ((*word)[i + 1] == '$')
		add_left(ft_itoa(getpid()), word, i + 2);
	else if ((*word)[i + 1] == '?')
		add_left(ft_itoa(g_data.exit_status), word, i + 2);
	else if ((*word)[i + 1] == '0')
		add_left(ft_strdup("minishell"), word, i + 2);
	else
		put_env(word, i + 1);
	if (start[0] != '\0')
		*word = smart_join(start, *word, *word);
	smart_free(start);
}
