#include "minishell.h"

extern t_data g_data;

t_redirection *create_redirection_node(char *key, int type)
{
	t_redirection *new;

	new = malloc(sizeof(t_redirection));
	new->type = type;
	if (type == INFILE)
		new->fd[0] = open(key, O_RDONLY, 0777);
	else if (type == OUTFILE)
		new->fd[0] = open(key, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (type == HEREDOC)
		pipe(new->fd);
	else if (type == APPEND)
		new->fd[0] = open(key, O_WRONLY | O_CREAT | O_APPEND, 0777);
	new->key = ft_strdup(key);
	new->next = NULL;
	return new;
}

void add_redirection_node(t_command *command, t_redirection *new, int type)
{
	if (command->redirection_heads[type - 1] == NULL)
	{
		command->redirection_heads[type - 1] = new;
		command->redirection_tails[type - 1] = new;
	}
	else
	{
		command->redirection_tails[type - 1]->next = new;
		command->redirection_tails[type - 1] = new;
	}
	if (type == HEREDOC || type == INFILE)
		command->infile = new;
	else if (type == OUTFILE || type == APPEND)
		command->outfile = new;
	close_all_redirections();
}

void close_all_redirections()
{
	t_command *temp;
	t_redirection *redir_temp;
	int				i;

	temp = g_data.command_head;
	while (temp != NULL)
	{
		i = 0;
		while (i < 4)
		{
			redir_temp = temp->redirection_heads[i];
			while (redir_temp != NULL)
			{
				close(redir_temp->fd[0]);
				close(redir_temp->fd[1]);
				redir_temp = redir_temp->next;
			}
			i++;
		}
		temp = temp->next;
	}
}
