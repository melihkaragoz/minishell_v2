/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 01:52:58 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 04:58:35 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	g_data;

int	rl_control(char *rl)
{
	if (g_data.sig == 1)
	{
		g_data.sig = 0;
		smart_free(rl);
		return (1);
	}
	if (is_full_space(rl))
	{
		smart_free(rl);
		return (1);
	}
	if (rl == NULL)
		return (1);
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*rl;
	int		stdot;
	int		stdit;

	init_data(argc, argv, envp);
	while (1)
	{
		stdot = dup(1);
		stdit = dup(0);
		rl = readline("segmentation fault$ ");
		if (rl_control(rl) == 1)
			continue ;
		add_history(rl);
		if (parse(rl) == 0)
			execute(merge_env());
		close_all_redirections();
		clear_parse_list();
		clear_command_list();
		dup2(stdot, 1);
		dup2(stdit, 0);
	}
	return (0);
}
