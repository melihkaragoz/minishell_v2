/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:17:34 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:19:15 by mkaragoz         ###   ########.fr       */
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
