#include "minishell.h"

extern t_data g_data;

static void init_redirections(t_command *command)
{
	command->redirection_heads[0] = NULL;
	command->redirection_heads[1] = NULL;
	command->redirection_heads[2] = NULL;
	command->redirection_heads[3] = NULL;
	command->redirection_tails[0] = NULL;
	command->redirection_tails[1] = NULL;
	command->redirection_tails[2] = NULL;
	command->redirection_tails[3] = NULL;
}


t_command *create_command_node()
{
	t_command *new;

	new = malloc(sizeof(t_command));

	new->infile = NULL;
	new->outfile = NULL;
	new->next = NULL;
	new->prev = NULL;
	init_redirections(new);
	return new;
}

void add_command_node(t_command *new)
{
	if (g_data.command_head == NULL)
	{
		new->prev = NULL;
		g_data.command_head = new;
		g_data.command_tail = new;
	}
	else
	{
		new->prev = g_data.command_tail;
		g_data.command_tail->next = new;
		g_data.command_tail = new;
	}
}

void clear_command_list()
{
	t_command *temp;
	t_command *freeable;
	t_redirection *redir;
	t_redirection *freeable_redir;
	int i;

	temp = g_data.command_head;
	while (temp != NULL)
	{
		freeable = temp;
		temp = temp->next;
		i = 0;
		while (i < 4)
		{
			redir = freeable->redirection_heads[i];
			while (redir != NULL)
			{
				freeable_redir = redir;
				redir = redir->next;
				smart_free(freeable_redir->key);
				smart_free(freeable_redir);
			}
			i++;
		}
		smart_free_strs(freeable->command);
		smart_free(freeable);
	}
	g_data.command_head = NULL;
	g_data.command_tail = NULL;
}

void print_commands(void)
{
	t_command *temp;
	int i;

	temp = g_data.command_head;

	while(temp != NULL)
	{
		i = 0;
		printf("command\n");
		while (temp->command[i])
		{
			printf("\n##########\narg[%d]: %s ", i, temp->command[i]);
			printf("\nredirections: \n");
			int j = 0;
			while (j < 4)
			{
				t_redirection *redir = temp->redirection_heads[j];
				while (redir != NULL)
				{
					char *type = NULL;
					if (redir->type == 0)
						type = "infile";
					else if (redir->type == 1)
						type = "outfile";
					else if (redir->type == 2)
						type = "heredoc";
					else if (redir->type == 3)
						type = "append";
					printf("\ntype: %s key: %s ", type, redir->key);
					redir = redir->next;
				}
				if (redir != NULL)
					printf("\n");
				j++;
			}
			i++;
		}
		printf("\n");
		temp =  temp->next;
	}
}
