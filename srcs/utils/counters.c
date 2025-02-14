/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counters.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 23:08:34 by welepy            #+#    #+#             */
/*   Updated: 2025/02/14 23:09:32 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	number_of_commands(t_token *tokens)
{
	t_token	*tmp;
	int	number;

	number = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == COMMAND || is_builtin(tmp->type))
			number++;
		tmp = tmp->next;
	}
	return(number);
}

int	count_args(t_token *tmp_token)
{
	int	i;
	t_token	*token = tmp_token;

	i = 0;
	while (token && token->type != PIPE && token->type != COMMAND)
	{
		if (token->type == ARGUMENT || token->type == DOUBLE_QUOTE
			|| token->type == SINGLE_QUOTE)
			i++;
		token = token->next;
	}
	return (i);
}