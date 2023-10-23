/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:04:07 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 05:04:31 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static int	cmd_access(char *cmd, t_command *node)
{
	int		i;
	char	*tmp;
	char	**paths;
	char	*path;

	i = -1;
	path = get_env("PATH");
	if (path == NULL)
		return (error_exit("No such file or directory", cmd, 127));
	paths = ft_split(path, ':');
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		tmp = smart_join(tmp, cmd, tmp);
		if (access(tmp, X_OK) == 0)
		{
			node->command[0] = smart_dup(tmp, node->command[0]);
			smart_free(tmp);
			break ;
		}
		smart_free(tmp);
	}
	smart_free_strs(paths);
	smart_free(path);
	return (0);
}

void	parse_access(void)
{
	char		*cmd;
	t_command	*tmp;

	tmp = g_data.command_head;
	while (tmp)
	{
		if (is_builtin(tmp->command[0]))
		{
			tmp = tmp->next;
			continue ;
		}
		cmd = tmp->command[0];
		if (access(cmd, X_OK) == 0)
		{
			tmp = tmp->next;
			continue ;
		}
		cmd_access(cmd, tmp);
		tmp = tmp->next;
	}
}

int	ms_check_seperators(char *s)
{
	char	seperators[4];
	int		j;

	seperators[0] = '|';
	seperators[1] = '<';
	seperators[2] = '>';
	seperators[3] = 0;
	j = -1;
	while (seperators[++j])
	{
		if (!ft_strncmp(s, &seperators[j], 1))
		{
			if (!ft_strncmp(s + 1, &seperators[j], 1))
				g_data.double_redirection = true;
			return (1);
		}
	}
	return (0);
}

int	ms_if_not_quote(int *i)
{
	if (!ft_strncmp(&g_data.line[(*i)], "\'", 1) || \
		!ft_strncmp(&g_data.line[(*i)], "\"", 1))
		ms_set_quote_mode(g_data.line[(*i)], i);
	else if (!ft_strncmp(&g_data.line[(*i)], " ", 1))
		return (ms_set_arg_false(0, i), 1);
	else if (g_data.line[(*i) + 1] && \
		ms_check_seperators(&g_data.line[(*i) + 1]))
		return (ms_set_arg_false(1, i), 1);
	return (0);
}

char	**alloc_cmd(t_parse_node *pnode)
{
	t_parse_node	*tmp;
	char			**ret;
	int				i;

	i = 0;
	tmp = pnode;
	while (tmp && tmp->type != PIPPE)
	{
		i++;
		tmp = tmp->next;
	}
	ret = malloc(sizeof(char *) * (i + 1));
	ret[i] = NULL;
	return (ret);
}
