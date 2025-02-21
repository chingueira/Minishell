/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 20:43:34 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/21 16:17:35 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	option_helper(t_token *head)
{
	char	*tmp;

	tmp = ft_strdup(head->value);
	ft_free(&head->value);
	head->value = ft_strdup(remove_quotes(tmp));
	head->type = OPTION;
	ft_free(&tmp);
}

void	id_quotes(t_token *tokens)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (head->type == SINGLE_QUOTE || head->type == DOUBLE_QUOTE)
		{
			if (ft_strncmp(head->value + 1, "echo", ft_strlen(head->value) - 2) == 0)
				head->type = ECHO;
			else if (ft_strncmp(head->value + 1, "exit", ft_strlen(head->value) - 2) == 0)
				head->type = EXIT;
			else if (ft_strncmp(head->value + 1, "cd", ft_strlen(head->value) - 2) == 0)
				head->type = CD;
			else if (ft_strncmp(head->value + 1, "env", ft_strlen(head->value) - 2) == 0)
				head->type = ENV;
			else if (ft_strncmp(head->value + 1, "export", ft_strlen(head->value) - 2) == 0)
				head->type = EXPORT;
			else if (ft_strncmp(head->value + 1, "unset", ft_strlen(head->value) - 2) == 0)
				head->type = UNSET;
			else if (ft_strncmp(head->value + 1, "pwd", ft_strlen(head->value) - 2) == 0)
				head->type = PWD;
			else if ((head->value[1] == '-'))
				option_helper(head);
		}
		head = head->next;
	}
	head = tokens;
	if (head->type == SINGLE_QUOTE || head->type == DOUBLE_QUOTE || head->type == ARGUMENT)
		head->type = COMMAND;
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
	head = tokens;
	id_quotes(head);
}

char	*remove_quotes(char *str)
{
	size_t len;
	if (str == NULL || strlen(str) < 2)
		return (str);
	len = ft_strlen(str);
	if ((str[0] == '"' || str[0] == '\'') && (str[len - 1] == '"' || str[len - 1] == '\''))
	{
		str++;
		str[len - 2] = '\0';
	}
	return (str);
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

void	error_message(char *str)
{
	ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}

/*char	**clean_args(char **args)
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
}*/