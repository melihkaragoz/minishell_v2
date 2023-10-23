/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:05:15 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:58:39 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

int	run_export(t_command *cmd)
{
	t_env	*tmp;
	char	*arg;

	arg = cmd->command[1];
	if (!arg)
	{
		tmp = g_data.env_head;
		while (tmp && tmp->value)
		{
			printf("declare -x %s", tmp->key);
			if (tmp->value && !is_full_space(tmp->value))
				printf("=\"%s\"", tmp->value);
			printf("\n");
			tmp = tmp->next;
		}
	}
	else
		add_export(arg);
	return (1);
}

int	run_env(void)
{
	t_env	*env;

	env = g_data.env_head;
	while (env && env->value && !is_full_space(env->value))
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (1);
}

int	cd_exec(t_command **cmd, char **home, char **tmp, char **new)
{
	int	res;

	res = -1;
	if ((*cmd)->command[1] == NULL)
	{
		res = go_dir(*home, new);
		if (res == 0)
		{
			smart_free(*home);
			return (0);
		}
		*tmp = *home;
	}
	else
	{
		res = go_dir((*cmd)->command[1], new);
		if (res == 0)
		{
			printf("cd: %s: No such file or directory\n", (*cmd)->command[1]);
			smart_free(*home);
			return (0);
		}
		*tmp = (*cmd)->command[1];
	}
	return (1);
}

int	run_cd(t_command *cmd)
{
	char	*home;
	char	*curr;
	char	*tmp;
	char	*new;
	int		res;

	res = -1;
	home = get_env("HOME");
	curr = get_env("PWD");
	res = cd_exec(&cmd, &home, &tmp, &new);
	if (!res)
		return (0);
	add_environment("PWD", new);
	add_environment("OLDPWD", curr);
	smart_free(new);
	smart_free(curr);
	smart_free(home);
	return (1);
}

int	run_unset(t_command *cmd)
{
	t_env	*prev;
	t_env	*env;
	char	*s;

	env = g_data.env_head;
	prev = g_data.env_head;
	s = cmd->command[1];
	while (s && env && env->value)
	{
		if (ft_strlen(env->key) == ft_strlen(s) && \
			!ft_strncmp(s, env->key, ft_strlen(env->key)))
		{
			if (env == g_data.env_head)
				g_data.env_head = unset_node(env);
			else
				prev->next = unset_node(env);
			update_env_tail();
			return (1);
		}
		prev = env;
		env = env->next;
	}
	return (1);
}
