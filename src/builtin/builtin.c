#include "minishell.h"

extern t_data g_data;

int	run_env(void)
{
	t_env *env;

	env = g_data.env_head;
	while (env && env->value && !is_full_space(env->value))
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (1);
}

int	run_export(t_command *cmd)
{
	char	**splitted;
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
	{
		splitted = ft_split(arg, '=');
		if (!splitted[1])
			add_environment(splitted[0], "");
		else
			add_environment(splitted[0], splitted[1]);
		smart_free_strs(splitted);
	}
	return (1);
}

int	run_pwd(void)
{
	char *pwd;

	pwd = get_env("PWD");
	printf("%s\n", pwd);
	smart_free(pwd);
	return (1);
}

int	run_echo(t_command *cmd)
{
	bool n;
	int i;

	i = 1;
	n = false;
	while (cmd->command[i])
	{
		if (i == 1 && !ft_strncmp("-n", cmd->command[i], 2) && check_echo_n(cmd->command[i]))
		{
			n = true;
			i++;
			continue;
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

int run_cd(t_command *cmd)
{
	int		res;
	char	*home;
	char	*curr;
	char	*tmp;

	res = -1;
	home = get_env("HOME");
	curr = get_env("PWD");
	if (cmd->command[1] == NULL)
	{
		res = go_dir(home);
		if (res == 0)
		{
			smart_free(home);
			return (0);
		}
		tmp = home;
	}
	else
	{
		res = go_dir(cmd->command[1]);
		if (res == 0)
		{
			printf("cd: %s: No such file or directory\n", cmd->command[1]);
			smart_free(home);
			return (0);
		}
		tmp = cmd->command[1];
	}
	add_environment("PWD", tmp);
	add_environment("OLDPWD", curr);
	// smart_free(tmp);
	smart_free(curr);
	smart_free(home);
	return (1);
}

int	run_unset(t_command *cmd)
{
	t_env *prev;
	t_env *env;

	char	*s;
	env = g_data.env_head;
	prev = g_data.env_head;
	s = cmd->command[1];
	while (s && env && env->value)
	{
		if (ft_strlen(env->key) == ft_strlen(s))
		{
			if (!ft_strncmp(s, env->key, ft_strlen(env->key)))
			{
				if (env == g_data.env_head)
					g_data.env_head = g_data.env_head->next;
				else if (env == g_data.env_tail)
					prev->next = NULL;
				else
					prev->next = env->next;
				update_env_tail();
				return (1);
			}
		}
		prev = env;
		env = env->next;
	}
	return (1);
}
