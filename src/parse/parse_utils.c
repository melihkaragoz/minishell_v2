/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:18:00 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:32:00 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	ms_node_check_builtin(char *content)
{
	if (!content)
		return (0);
	if (!ft_strncmp("echo", content, 5))
		return (1);
	else if (!ft_strncmp("pwd", content, 4))
		return (1);
	else if (!ft_strncmp("export", content, 7))
		return (1);
	else if (!ft_strncmp("unset", content, 6))
		return (1);
	else if (!ft_strncmp("env", content, 4))
		return (1);
	else if (!ft_strncmp("cd", content, 3))
		return (1);
	else if (!ft_strncmp("π", content, 3))
		return (1);
	return (0);
}

int	ms_node_check_redirection(char *content)
{
	if (!ft_strncmp("<<", content, 2))
		return (HEREDOC);
	else if (!ft_strncmp(">>", content, 2))
		return (APPEND);
	else if (!ft_strncmp("<", content, 1))
		return (INFILE);
	else if (!ft_strncmp(">", content, 1))
		return (OUTFILE);
	return (0);
}

char	*smart_substr(char *str, char *start, int len, void *freeable)
{
	char	*tmp;

	tmp = ft_substr(str, start - str, len);
	free(freeable);
	return (tmp);
}

void	trim(char **rl)
{
	char	*st;
	char	*fn;
	int		i;
	int		f;

	i = 0;
	f = 0;
	while ((*rl)[i])
	{
		if ((*rl)[i] > 32 && (*rl)[i] < 127)
		{
			fn = (*rl) + i;
			if (f == 0)
			{
				st = fn;
				f = 1;
			}
		}
		i++;
	}
	*rl = smart_substr(*rl, st, fn - st + 1, *rl);
}
