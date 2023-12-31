/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:28:12 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:58:43 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	check_echo_n(char *keyword)
{
	int	i;

	i = 0;
	while (keyword[++i])
		if (keyword[i] != 'n')
			return (0);
	return (1);
}

void	update_env_tail(void)
{
	g_data.env_tail = g_data.env_head;
	while (g_data.env_tail->next && g_data.env_tail->next->key)
		g_data.env_tail = g_data.env_tail->next;
}

int	go_dir(char *path, char **new)
{
	char	*pwd;
	char	*oldpwd;

	pwd = getcwd(NULL, 0);
	oldpwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		smart_free(pwd);
		smart_free(oldpwd);
		return (0);
	}
	*new = getcwd(NULL, 0);
	add_environment("PWD", *new);
	add_environment("OLDPWD", oldpwd);
	smart_free(pwd);
	smart_free(oldpwd);
	return (1);
}

void	run_exit(int num)
{
	close_all_redirections();
	clear_parse_list();
	clear_command_list();
	exit(num);
}
