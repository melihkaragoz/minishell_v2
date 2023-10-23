/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 01:52:58 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 05:59:58 by mkaragoz         ###   ########.fr       */
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
	else if (is_full_space(rl))
	{
		smart_free(rl);
		return (1);
	}
	else if (is_full_space(rl))
	{
		smart_free(rl);
		return (1);
	}
	return (0);
}

void	ms_exit(void)
{
	printf("\033[A\n%sexit\n", "minihell$ ");
	exit(0);
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
		rl = readline("minihell$ ");
		if (rl == NULL)
			ms_exit();
		else if (rl_control(rl) == 1)
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
