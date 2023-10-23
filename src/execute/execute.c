/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:48:01 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:49:05 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

void	redirections(t_command *curr)
{
	if (curr->infile != NULL)
	{
		if (curr->infile->fd[0] == -1)
		{
			error_exit("No such file or directory", curr->infile->key, 1);
			exit(g_data.exit_status);
		}
		dup2(curr->infile->fd[0], STDIN_FILENO);
	}
	if (curr->outfile != NULL)
		dup2(curr->outfile->fd[0], STDOUT_FILENO);
}

void	duplicates(t_command *curr)
{
	if (curr->prev == NULL && curr->outfile == NULL)
		dup2(curr->pipe_fds[1], STDOUT_FILENO);
	else if (curr->next == NULL && curr->infile == NULL)
		dup2(curr->prev->pipe_fds[0], STDIN_FILENO);
	else if (curr->prev != NULL && curr->next != NULL)
	{
		if (curr->infile == NULL)
			dup2(curr->prev->pipe_fds[0], STDIN_FILENO);
		if (curr->outfile == NULL)
			dup2(curr->pipe_fds[1], STDOUT_FILENO);
	}
}

int	builtin_run(t_command *cmd)
{
	if (ft_strncmp(cmd->command[0], "echo", 5) == 0)
		return (run_echo(cmd));
	else if (ft_strncmp(cmd->command[0], "cd", 3) == 0)
		return (run_cd(cmd));
	else if (ft_strncmp(cmd->command[0], "pwd", 4) == 0)
		return (run_pwd());
	else if (ft_strncmp(cmd->command[0], "export", 7) == 0)
		return (run_export(cmd));
	else if (ft_strncmp(cmd->command[0], "unset", 6) == 0)
		return (run_unset(cmd));
	else if (ft_strncmp(cmd->command[0], "env", 4) == 0)
		return (run_env());
	else if (ft_strncmp(cmd->command[0], "exit", 5) == 0)
		run_exit(0);
	return (0);
}

void	exec(t_command *curr, int type, char **envp)
{
	heredoc(curr->redirection_heads[HEREDOC - 1]);
	if (is_builtin(curr->command[0]) && type != MULTI_COMMAND)
	{
		builtin_run(curr);
		return ;
	}
	if (fork() == 0)
	{
		redirections(curr);
		if (type == MULTI_COMMAND)
		{
			duplicates(curr);
			close_pipes();
		}
		if (is_builtin(curr->command[0]))
		{
			builtin_run(curr);
			exit(g_data.exit_status);
		}
		execve(curr->command[0], curr->command, envp);
		error_exit("command not found", curr->command[0], 127);
		exit(g_data.exit_status);
	}
}

void	execute(char **envp)
{
	t_command	*curr;
	int			status;

	curr = g_data.command_head;
	if (curr && curr->next == NULL)
		exec(curr, SINGLE_COMMAND, envp);
	else
	{
		open_pipes();
		while (curr != NULL)
		{
			exec(curr, MULTI_COMMAND, envp);
			curr = curr->next;
		}
	}
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_data.exit_status = WEXITSTATUS(status);
		close_pipes();
	}
	smart_free_strs(envp);
}
