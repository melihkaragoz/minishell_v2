/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:44:02 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:44:51 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	is_full_space(char *rl)
{
	int	i;

	i = 0;
	while (rl && rl[i])
	{
		if (rl[i] != ' ' && rl[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	is_sep(char *s)
{
	int	i;
	int	len;

	i = 0;
	while (i < SEPERATORS_SIZE)
	{
		len = ft_strlen(g_data.seperators[i]);
		if (ft_strncmp(g_data.seperators[i], s, len) == 0)
			return (len);
		i++;
	}
	return (0);
}

void	space_jumper(char *s, int *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
}

int	ft_isval(int c)
{
	if (c < 127 && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9') || c == '_' || c == '.'))
		return (1);
	return (0);
}
