#include "minishell.h"

extern t_data g_data;

void heredoc(t_redirection *heredoc)
{
	char *rl;

	while (heredoc != NULL)
	{
		while (1)
		{
			rl = readline("> ");
			if (ft_strncmp(rl, heredoc->key, ft_strlen(heredoc->key) + 1) == 0)
			{
				smart_free(rl);
				break;
			}
			ft_putstr_fd(rl, heredoc->fd[1]);
			ft_putstr_fd("\n", heredoc->fd[1]);
			smart_free(rl);
		}
		close(heredoc->fd[1]);
		heredoc = heredoc->next;
	}
}
