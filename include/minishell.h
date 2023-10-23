/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 04:47:56 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:56:17 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include "libft.h"

# define SEPERATORS_SIZE 8
# define NO_Q 0
# define SINGLE_Q 1
# define DOUBLE_Q 2

# define OUTFILE 1
# define HEREDOC 2
# define APPEND 3
# define INFILE 4
# define PIPPE 5
# define BUILTIN 6

# define SINGLE_COMMAND 0
# define MULTI_COMMAND 1

# define MAIN 0
# define PIPE 1
# define HEREDOC 2

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redirection
{
	int						fd[2];
	char					*key;
	int						type;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	t_redirection		*infile;
	t_redirection		*outfile;
	int					pipe_fds[2];
	char				**command;
	struct s_command	*next;
	struct s_command	*prev;
	t_redirection		*redirection_heads[4];
	t_redirection		*redirection_tails[4];
}	t_command;

typedef struct s_parse_node
{
	char				*word;
	int					type;
	struct s_parse_node	*next;
	struct s_parse_node	*prev;
}	t_parse_node;

typedef struct s_data
{
	char			*line;
	int				quote_mode;
	bool			arg_mode;
	bool			double_redirection;
	int				command_count;
	int				pipe_count;
	t_parse_node	*parse_head;
	t_parse_node	*parse_tail;
	t_command		*command_head;
	t_command		*command_tail;
	t_env			*env_head;
	t_env			*env_tail;
	char			**path;
	char			*seperators[SEPERATORS_SIZE];
	int				exit_status;
	int				sig;
	int				quit_flag;
}	t_data;

int				ms_node_check_builtin(char *content);
int				ms_node_check_redirection(char *content);
void			ms_set_nodes(int *i);
int				is_sep(char *s);
int				ft_isval(int c);
void			space_jumper(char *s, int *i);
void			smart_free(void *addr);
char			*smart_join(char *s1, char *s2, void *freeable);
void			smart_free_strs(char **strs);
char			*smart_dup(char *s1, void *freeable);
int				is_redir(t_parse_node *temp);
t_redirection	*if_redir(t_parse_node *temp);
int				is_special_char(char c);
int				error_exit(char *message, char *str, int exit_status);
int				parse(char *rl);
void			add_parse_node(char *s);
void			clear_parse_list(void);
void			dollar(char **word, int i);
void			add_command_node(t_command *new);
void			parse_commands(void);
void			parse_access(void);
int				check_commands(void);
t_command		*create_command_node(void);
t_redirection	*create_redirection_node(char *key, int type);
void			add_redirection_node(t_command *command, t_redirection *new,
					int type);
void			close_all_redirections(void);
void			clear_command_list(void);
void			init_data(int argc, char *argv[], char *envp[]);
char			*get_env(char *key);
void			set_env(char **envp);
void			execute(char **envp);
void			close_pipes(void);
void			open_pipes(void);
void			heredoc(t_redirection *heredoc);
int				is_full_space(char *rl);
char			**merge_env(void);
void			sigint_handler(int signum);
void			sigquit_handler(int signum);
int				run_export(t_command *cmd);
int				run_unset(t_command *cmd);
int				run_env(void);
int				run_pwd(void);
int				check_echo_n(char *keyword);
void			update_env_tail(void);
int				check_and_update(char *key, char *value);
int				go_dir(char *path, char **new);
int				run_cd(t_command *cmd);
int				run_echo(t_command *cmd);
void			run_exit(int num);
int				builtin_run(t_command *cmd);
int				is_builtin(char	*cmd);
void			add_environment(char *key, char *value);
int				print_nodes(void);
char			*smart_substr(char *str, char *start, int len, void *freeable);
void			add_export(char	*arg);
void			*unset_node(t_env *node);
void			trim(char **rl);
int				set_commands(void);
int				set_commands_util(int *i, t_parse_node **tmp,
					t_redirection **redir, t_command **cmd);
int				check_missing_quotes(char *rl);
void			seperate(char *rl, int *i);
int				ms_end_of_word(int *i);
void			ms_set_quote_mode(int set, int *i);
int				ms_set_arg_false(int i, int *j);
int				ms_if_not_quote(int *i);
char			**alloc_cmd(t_parse_node *pnode);
int				ms_check_seperators(char *s);
#endif
