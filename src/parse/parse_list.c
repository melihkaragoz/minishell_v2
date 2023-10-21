#include "minishell.h"

extern t_data g_data;

static t_parse_node *create_parse_node(char *s)
{
	t_parse_node *new;

	new = malloc(sizeof(t_parse_node));
	new->type = 0;
	new->word = s;
	new->next = NULL;
	return new;
}

void add_parse_node(char *s)
{
	t_parse_node *new;
	new = create_parse_node(s);
	if (g_data.parse_head == NULL && new)
	{
		new->prev = NULL;
		g_data.parse_head = new;
		g_data.parse_tail = new;
		printf(" -%s- node added to parse_head\n", g_data.parse_head->word);
	}
	else
	{
		new->prev = g_data.parse_tail;
		g_data.parse_tail->next = new;
		g_data.parse_tail = new;
		printf(" -%s- node added to next\n", g_data.parse_tail->word);
	}
}

void clear_parse_list(void)
{
	t_parse_node	*temp;
	t_parse_node	*freeable;

	temp = g_data.parse_head;
	while (temp != NULL)
	{
		freeable = temp;
		temp = temp->next;
		smart_free(freeable->word);
		smart_free(freeable);
	}
	g_data.parse_head = NULL;
	g_data.parse_tail = NULL;
}

// void print_list(void)
// {
// 	t_parse_node *temp;

// 	temp = g_data.parse_head;
// 	while (temp != NULL)
// 	{
// 		printf("%s\n", temp->word);
// 		temp = temp->next;
// 	}
// }
