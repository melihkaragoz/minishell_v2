/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:41:12 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:41:13 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static void	init_seperators(void)
{
	g_data.seperators[0] = "|";
	g_data.seperators[1] = " ";
	g_data.seperators[2] = "\t";
	g_data.seperators[3] = "<<";
	g_data.seperators[4] = ">>";
	g_data.seperators[5] = "<";
	g_data.seperators[6] = ">";
	g_data.seperators[7] = "\0";
}

void	init_data(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	g_data.quote_mode = 0;
	g_data.parse_head = NULL;
	g_data.parse_tail = NULL;
	g_data.command_head = NULL;
	g_data.command_tail = NULL;
	g_data.env_head = NULL;
	g_data.env_tail = NULL;
	g_data.exit_status = 0;
	g_data.sig = MAIN;
	g_data.quit_flag = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	set_env(envp);
	init_seperators();
}

int	check_and_update(char *key, char *value)
{
	t_env	*tmp;

	tmp = g_data.env_head;
	while (tmp != NULL)
	{
		if (!ft_strncmp(tmp->key, key, ft_strlen(tmp->key) + 1))
		{
			tmp->value = smart_dup(value, tmp->value);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
