/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 20:43:34 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/28 11:10:58 by mchingi          ###   ########.fr       */
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
