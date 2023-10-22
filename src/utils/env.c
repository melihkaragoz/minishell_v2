/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaragoz <mkaragoz@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 02:32:56 by mkaragoz          #+#    #+#             */
/*   Updated: 2023/10/23 02:40:33 by mkaragoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_data	g_data;

static t_env	*create_env(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = NULL;
	return (new);
}

void	add_environment(char *key, char *value)
{
	t_env	*new;

	if (!check_and_update(key, value))
	{
		new = create_env(key, value);
		if (g_data.env_head == NULL)
		{
			g_data.env_head = new;
			g_data.env_tail = new;
		}
		else
		{
			g_data.env_tail->next = new;
			g_data.env_tail = g_data.env_tail->next;
		}
	}
}

void	set_env(char **envp)
{
	char	**splitted;
	int		i;

	i = 0;
	while (envp[i])
	{
		splitted = ft_split(envp[i], '=');
		add_environment(splitted[0], splitted[1]);
		smart_free_strs(splitted);
		i++;
	}
}

char	*get_env(char *key)
{
	t_env	*temp;

	temp = g_data.env_head;
	while (temp != NULL)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			break ;
		temp = temp->next;
	}
	if (temp == NULL)
		return (NULL);
	return (ft_strdup(temp->value));
}

char	**merge_env(void)
{
	char	**envp;
	t_env	*curr;
	int		i;

	i = 0;
	curr = g_data.env_head;
	while (curr != NULL)
	{
		i++;
		curr = curr->next;
	}
	envp = malloc(sizeof(char *) * (i + 1));
	i = 0;
	curr = g_data.env_head;
	while (curr != NULL)
	{
		envp[i] = smart_join(curr->key, "=", NULL);
		envp[i] = smart_join(envp[i], curr->value, envp[i]);
		i++;
		curr = curr->next;
	}
	envp[i] = NULL;
	return (envp);
}
