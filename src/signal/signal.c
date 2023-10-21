#include "minishell.h"

extern t_data g_data;

void sigint_handler(int signum)
{
	(void)signum;
	g_data.sig = 1;
	write(1, "\033[A", 3);
	ioctl(0, TIOCSTI, "\n");
}

void sigquit_handler(int signum)
{
	(void)signum;
	printf("Quit: 3\n");
}