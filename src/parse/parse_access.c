#include "minishell.h"

extern t_data g_data;

static int cmd_access(char *cmd, t_command *node)
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

void parse_access()
{
	char	*cmd;
	t_command *tmp;
	
	tmp = g_data.command_head;
	while (tmp)
	{
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
