#include "minishell.h"

extern t_data g_data;

static void env_resulation(void)
{
	t_parse_node *temp;
	int i;
	int quote_mode;

	temp = g_data.parse_head;
	while (temp != NULL)
	{
		i = 0;
		quote_mode = NO_Q;
		while (temp->word[0] != '\0' && temp->word[i])
		{
			if (quote_mode != SINGLE_Q)
			{
				if (temp->word[i] == '$')
				{
					dollar(&temp->word, i);
					i = 0;
					continue;
				}
			}
			if (temp->word[i] == '\'')
				quote_mode = SINGLE_Q;
			else if (temp->word[i] == '"')
				quote_mode = DOUBLE_Q;
			i++;
		}
		temp = temp->next;
	}
}

static void ms_set_quote_mode(int set, int *i)
{
	g_data.quote_mode = set;
	g_data.arg_mode = 1;
	if (g_data.line[(*i) + 1] == set)
	{
		(*i)++;
		g_data.quote_mode = 0;
	}
}

static void ms_set_arg_false(int i, int *j)
{
	if (i == 4 || i == 1 || i == 2 || i == 3)
		(*j)++;
	if (i == 4 || i == 1 || i == 2 || i == 0)
		g_data.arg_mode = false;
	if (i == 7 || i == 5)
		g_data.quote_mode = 0;
	if (i == 3)
		ms_set_quote_mode(g_data.line[*j], j);
	if (g_data.double_redirection == true && (i == 4))
	{
		(*j)++;
		g_data.double_redirection = false;
	}
}

static int ms_check_seperators(char *s)
{
	char seperators[4];
	int j;

	seperators[0] = '|';
	seperators[1] = '<';
	seperators[2] = '>';
	seperators[3] = 0;
	j = -1;
	while (seperators[++j])
		if (!ft_strncmp(s, &seperators[j], 1))
		{
			if (!ft_strncmp(s + 1, &seperators[j], 1))
				g_data.double_redirection = true;
			return (1);
		}
	return (0);
}

static int ms_end_of_word(int *i)
{
	if (ms_check_seperators(&g_data.line[(*i)]) && !g_data.quote_mode) // << >> || geldiyse burda geçiyor
		return (ms_set_arg_false(4, i), 1);
	while (g_data.quote_mode && g_data.line[(*i)] && (*i)++) // tırnak açıldıysa
	{
		if (g_data.line[(*i)] == g_data.quote_mode) // tırnak kapandıysa
		{
			if (g_data.line[(*i) + 1] && (ms_check_seperators(&g_data.line[(*i) + 1]) || g_data.line[(*i) + 1] == ' ')) // tırnak kapandıysa --- bir sonraki karakter seperator ise
				return (ms_set_arg_false(5, i), 1);
			else if (g_data.line[(*i) + 1] && (g_data.line[(*i) + 1] == 39 || g_data.line[(*i) + 1] == 34)) // tırnak kapandıysa --- bir sonraki karakter tırnaksa
				return (ms_set_arg_false(3, i), 0);
			else
				return (ms_set_arg_false(7, i), 0); // tırnak kapandıysa --- bir sonraki karakter karakter ise
		}
	}
	if (g_data.line[(*i)] && !g_data.quote_mode) // tırnak açık değilse
	{
		if (!ft_strncmp(&g_data.line[(*i)], "\'", 1) || !ft_strncmp(&g_data.line[(*i)], "\"", 1)) // tırnak açık değilse --- tırnaksa
			ms_set_quote_mode(g_data.line[(*i)], i);
		else if (!ft_strncmp(&g_data.line[(*i)], " ", 1)) // tırnak açık değilse --- boşluksa
			return (ms_set_arg_false(0, i), 1);
		else if (g_data.line[(*i) + 1] && ms_check_seperators(&g_data.line[(*i) + 1])) // tırnak açık değilse --- bir sonraki karakter seperator ise
			return (ms_set_arg_false(1, i), 1);
	}
	return (0);
}

static int check_missing_quotes(char *rl)
{
	int i;
	int quote;

	quote = 0;
	i = -1;
	while (rl[++i])
	{
		if (quote == 0 && (rl[i] == 34 || rl[i] == 39))
			quote = rl[i];
		else if (quote == rl[i])
			quote = 0;
	}
	return (!!quote);
}

static void seperate(char *rl, int *i)
{
	int f;

	g_data.line = ft_strdup(rl);
	while (g_data.line && g_data.line[*i])
	{
		while (g_data.line[*i] && g_data.line[*i] <= 32 && g_data.line[*i] >= 0) // bosluklari gec
			(*i)++;
		// space_jumper(g_data.line, i);
		f = (*i);
		while (g_data.line[*i] && (!ms_end_of_word(i) || g_data.arg_mode)) // ozel karakterleri gec
			(*i)++;
		add_parse_node(ft_substr(g_data.line, f, (*i) - f)); // node'un oluştuğu bölüm
		g_data.arg_mode = true;
	}
}

// static void seperate(char *rl)
// {
// 	int	i;
// 	int start;
// 	int sep;

// 	i = 0;
// 	while (rl[i])
// 	{
// 		sep = 0;
// 		space_jumper(rl, &i);
// 		start = i;
// 		while (rl[i] && sep == 0)
// 		{
// 			sep = is_sep(&rl[i]);
// 			if (sep == 0)
// 				i++;
// 		}
// 		if (start == i)
// 			i += sep;
// 		add_parse_node(ft_substr(rl, start, i - start));
// 	}
// }

// void	print_nodes()
// {
// 	t_parse_node *tmp = g_data.parse_head;
// 	while (tmp)
// 	{
// 		printf("word:  %s      ----->     type:  %d\n", tmp->word, tmp->type);
// 		tmp = tmp->next;
// 	}

// }

static char **alloc_cmd(t_parse_node *pnode)
{
	t_parse_node *tmp;
	char **ret;
	int i;

	i = 0;
	tmp = pnode;
	while (tmp && tmp->type != PIPPE)
	{
		i++;
		tmp = tmp->next;
	}
	ret = malloc(sizeof(char *) * (i + 1));
	ret[i] = NULL;
	return (ret);
}

// static void	prints_nodes()
// {
// 	t_parse_node *tmp = g_data.parse_head;
// 	while (tmp)
// 	{
// 		printf("word:  %s      ----->     type:  %d\n", tmp->word, tmp->type);
// 		tmp = tmp->next;
// 	}
// }

int set_commands(void)
{
	t_parse_node *tmp;
	t_command *cmd;
	t_redirection *redir;
	int i;

	tmp = g_data.parse_head;
	cmd = g_data.command_head;
	redir = NULL;
	while (tmp && tmp->word)
	{
		i = 0;
		cmd = create_command_node();
		cmd->command = alloc_cmd(tmp);
		while (tmp && tmp->type != PIPPE)
		{
			redir = if_redir(tmp);
			if (redir != NULL)
			{
				add_redirection_node(cmd, redir, redir->type);
				if (tmp->next->next)
					tmp = tmp->next->next;
				else
				{
					cmd->command[i] = NULL;
					add_command_node(cmd);
					return (1);
				}
			}
			cmd->command[i] = ft_strdup(tmp->word);
			tmp = tmp->next;
			i++;
		}
		cmd->command[i] = NULL;
		add_command_node(cmd);
		if (tmp && tmp->type == PIPPE)
			tmp = tmp->next;
	}
	return (1);
}

char *smart_substr(char *str, char *start, int len, void *freeable)
{
	char *tmp;

	tmp = ft_substr(str, start - str, len);
	free(freeable);
	return (tmp);
}

void trim(char **rl)
{
	char *st;
	char *fn;
	int i;
	int f;

	i = 0;
	f = 0;
	while ((*rl)[i])
	{
		if ((*rl)[i] > 32 && (*rl)[i] < 127)
		{
			fn = (*rl) + i;
			if (f == 0)
			{
				st = fn;
				f = 1;
			}
		}
		i++;
	}
	*rl = smart_substr(*rl, st, fn - st + 1, *rl);
}

int parse(char *rl)
{
	int i;

	i = 0;
	if (check_missing_quotes(rl))
	{
		perror("Missing quote!\n");
		return (1);
	}
	trim(&rl);
	seperate(rl, &i);

	// ################################################################ DEBUG
	t_parse_node *pn;
	pn = g_data.parse_head;
	printf("\n\tAFTER SEPERATE\n");
	while (pn)
	{
		printf("[PN]word: %s\ttype: %d\n", pn->word, pn->type);
		pn = pn->next;
	}
	printf("\n\n");
	// ################################################################ DEBUG

	env_resulation();
	pn = g_data.parse_head;
	printf("\n\tAFTER RESULATION\n");
	while (pn)
	{
		printf("[PN]word: %s\ttype: %d\n", pn->word, pn->type);
		pn = pn->next;
	}
	printf("\n\n");
	// ################################################################ DEBUG
	ms_set_nodes(&i);
	pn = g_data.parse_head;
	printf("\n\tAFTER SETTING NODES\n");
	while (pn)
	{
		printf("[PN]word: %s\ttype: %d\n", pn->word, pn->type);
		pn = pn->next;
	}
	printf("\n\n");
	// ################################################################ DEBUG
	// prints_nodes();
	set_commands();
	t_command *cn;
	cn = g_data.command_head;
	pn = g_data.parse_head;
	printf("\n\tAFTER SETTING COMMANDS\n");
	int c;
	while (cn)
	{
		c = -1;
		while (cn->command[++c])
			printf("[PN]word: %s\n", cn->command[c]);
		printf("\n");
		cn = cn->next;
	}
	printf("\n\n");
	// ################################################################ DEBUG

	print_commands();
	parse_access();
	return (0);
}

// int parse(char *rl)
// {
// 	seperate(rl);
// 	env_resulation();
// 	//tirnak sil
// if (check_commands() == 0)
// 		return 1;
// parse_commands();
// 	parse_access();
// 	return 0;
// }
