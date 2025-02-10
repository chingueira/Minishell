/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 20:43:34 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/10 12:21:00 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	is_builtin_or_command(t_type type)
{
	return (type == COMMAND || type == ECHO || type == CD || type == PWD ||
		type == EXPORT || type == UNSET || type == ENV);
}

void	token_sequence(t_token *tokens)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (is_builtin_or_command(head->type) && head->next && is_builtin_or_command(head->next->type))
		{
			while (head->next && (is_builtin_or_command(head->next->type)))
			{
				head->next->type = ARGUMENT;
				head = head->next;
			}
		}
		head = head->next;
	}
}

char	*remove_quotes(char *str)
{
	size_t len;
	if (str == NULL || strlen(str) < 2)
		return (str);
	len = strlen(str);
	if (str[0] == '"' && str[len - 1] == '"')
	{
		str++;
		str[len - 2] = '\0';
	}
	return (str);
}

char	**clean_args(char **args)
{
	int		i;
	char	**cleaned_args;

	i = 0;
	while (args[i])
		i++;
	cleaned_args = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cleaned_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		cleaned_args[i] = remove_quotes(args[i]);
		i++;
	}
	cleaned_args[i] = NULL;
	return (cleaned_args);
}

char	*clean_string(char *str)
{
	int 	tab[2];
	char	*cleaned_str;

	tab[0] = 0;
	tab[1] = 0;
	while (str[tab[0]])
	{
		if (str[tab[0]] != '"' || str[tab[0]] != '\'')
			tab[1]++;
		tab[0]++;
	}
	cleaned_str = (char *)malloc(sizeof(char) * (tab[1] + 1));
	if (!cleaned_str)
		return (NULL);
	tab[0] = 0;
	tab[1] = 0;
	while (str[tab[0]])
	{
		if (str[tab[0]] == '\"' || str[tab[0]] == '\'')
			tab[0]++;
		cleaned_str[tab[1]] = str[tab[0]];
		tab[0]++;
		tab[1]++;
	}
	cleaned_str[tab[0]] = '\0';
	return (cleaned_str);
}
