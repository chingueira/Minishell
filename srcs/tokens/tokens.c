/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 11:53:06 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/19 01:14:04 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

t_type	find_type(char *value)
{
	if (!ft_strcmp(value, ">"))
		return (GREATER);
	else if (!ft_strcmp(value, "<"))
		return (LESSER);
	else if (!ft_strcmp(value, ">>"))
		return (APPEND);
	else if (!ft_strcmp(value, "<<"))
		return (HERE_DOC);
	else if (!ft_strcmp(value, "|"))
		return (PIPE);
	else if (!ft_strcmp(value, "echo"))
		return (BUILTINS);
	else if (!ft_strcmp(value, "cd") || !ft_strcmp(value, "pwd"))
		return (BUILTINS);
	else if (!ft_strcmp(value, "export") || !ft_strcmp(value, "unset"))
		return (BUILTINS);
	else if (!ft_strcmp(value, "env") || !ft_strcmp(value, "exit"))
		return (BUILTINS);
	else if (value[0] == '\'')
		return (SINGLE_QUOTE);
	else if (value[0] == '\"')
		return (DOUBLE_QUOTE);
	// else if (ft_strchr(value, '/'))
	// 	return (PATH);
	else if (value[0] == '-')
		return (OPTION);
	else
		return (IDENTIFIER);
}

t_token	*tokenize_array(char **array)
{
	int		i;
	t_token	*new;
	t_token	*token;

	i = 0;
	token = NULL;
	while (array[i])
	{
		new = new_token(array[i], find_type(array[i]));
		ft_lstadd_back((t_list **)&token, (t_list *)new);
		i++;
	}
	return (token);
}

void	identify_tokens(t_token *tokens, char *path)
{
	t_token	*head;

	head = tokens;
	while (head)
	{
		if (head->type == IDENTIFIER)
		{
			if (is_command(head->value, path))
				head->type = COMMAND;
			else
				head->type = ARGUMENT;
		}
		head = head->next;
	}
	if (tokens->type == ARGUMENT)
		printf("%s: command not found\n", tokens->value);
}

