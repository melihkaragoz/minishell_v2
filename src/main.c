#include "minishell.h"

t_data g_data;

int rl_control(char *rl)
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

void gadget(void)
{
	system("leaks minishell");
	printf("devam mi abi ? ");
	getchar();
}

int main(int argc, char *argv[], char *envp[])
{
	char *rl;

	init_data(argc, argv, envp);
	while (1)
	{
		rl = readline("segmentation fault$ ");
		if (rl_control(rl) == 1)
			continue;
		add_history(rl);
		if (parse(rl) == 0)
			execute(merge_env());
		close_all_redirections();
		clear_parse_list();
		clear_command_list();
		//free(rl);
		//system("leaks minishell");
	}
	return 0;
}
