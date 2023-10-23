/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:17:34 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 03:12:17 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	run_pwd(void)
{
	char	*pwd;

	pwd = get_env("PWD");
	printf("%s\n", pwd);
	smart_free(pwd);
	return (1);
}

void	add_export(char	*arg)
{
	char	**splitted;

	splitted = ft_split(arg, '=');
	if (!splitted[1])
		add_environment(splitted[0], "");
	else
		add_environment(splitted[0], splitted[1]);
	smart_free_strs(splitted);
}

void	*unset_node(t_env *node)
{
	void	*tmp;

	smart_free(node->key);
	smart_free(node->value);
	tmp = node->next;
	free(node);
	return (tmp);
}

int	run_echo(t_command *cmd)
{
	bool	n;
	int		i;

	i = 1;
	n = false;
	while (cmd->command[i])
	{
		if (i == 1 && !ft_strncmp("-n", cmd->command[i], 2) && \
			check_echo_n(cmd->command[i]))
		{
			n = true;
			i++;
			continue ;
		}
		printf("%s", cmd->command[i]);
		if (cmd->command[i + 1])
			printf(" ");
		i++;
	}
	if (n == false)
		printf("\n");
	return (1);
}
