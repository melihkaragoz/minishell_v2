#include "minishell.h"

extern t_data g_data;

int ms_node_check_builtin(char *content)
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
	// else if (!ft_strncmp("exit", content, 5))
	// 	ms_exit ("exit",0);
	else if (!ft_strncmp("cd", content, 3))
		return (1);
	else if (!ft_strncmp("π", content, 3))
		return (1);
	return (0);
}

int ms_node_check_redirection(char *content)
{
	if (!ft_strncmp("<<", content, 2))
		return (5);
	else if (!ft_strncmp("<", content, 1))
		return (3);
	else if (!ft_strncmp(">>", content, 2))
		return (6);
	else if (!ft_strncmp(">", content, 1))
		return (4);
	else if (!ft_strncmp("||", content, 2))
		return (7);
	else if (!ft_strncmp("&&", content, 2))
		return (8);
	return (0);
}