#include "minishell.h"

extern t_data g_data;

static void ms_node_remove_char(t_parse_node *tmp, int i) // birinci parametre olarak verilen stringden ikinci parametre indexindeki karakteri siler.
{
	char *tmp_str;
	char *tmp_str_extra;
	int len;

	if (tmp->word && *tmp->word)
	{
		len = ft_strlen(tmp->word);
		tmp_str = ft_substr(tmp->word, 0, i);
		tmp_str_extra = ft_substr(tmp->word, i + 1, len - i);
		free(tmp->word);
		tmp->word = ft_strjoin(tmp_str, tmp_str_extra);
		free(tmp_str);
		free(tmp_str_extra);
	}
}

static void ms_remove_quotes(t_parse_node *tmp, int *i)
{
	int quo;
	int start_pos_quo;

	quo = 0;
    *i = 0;
	while (tmp->word[(*i)])
	{
		if (quo && tmp->word[(*i)] == quo)
		{
			quo = 0;
			ms_node_remove_char(tmp, start_pos_quo);
			ms_node_remove_char(tmp, (*i) - 1);
			(*i) -= 2;
		}
		else if (!quo && (tmp->word[(*i)] == '\'' || tmp->word[(*i)] == '\"'))
		{
			quo = tmp->word[(*i)];
			start_pos_quo = (*i);
		}
		(*i)++;
	}
}

void ms_set_nodes(int *i)
{
    t_parse_node *tmp;

    tmp = g_data.parse_head;
	while (tmp) // node'ları tek tek gezen döngü
	{
		tmp->type = 0;								  // undefined
		if (ms_node_check_redirection(tmp->word)) // redirection
			tmp->type = ms_node_check_redirection(tmp->word);
		else if (!ft_strncmp("|", tmp->word, 2)) // pipe
			tmp->type = 2;
		ms_remove_quotes(tmp, i);
		if (ms_node_check_builtin(tmp->word)) // built-in
			tmp->type = 1;
		tmp = tmp->next;
	}
}