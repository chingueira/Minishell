/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifiers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 22:35:04 by welepy            #+#    #+#             */
/*   Updated: 2025/02/14 23:18:34 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_builtin(t_type type)
{
	return (type == ECHO || type == CD || type == PWD ||
		type == EXPORT || type == UNSET || type == ENV || type == EXIT);
}

bool	is_builtin_or_command(t_type type)
{
	return (type == COMMAND || is_builtin(type));
}

bool	is_operator(t_type type)
{
	return (type == DOUBLE_PIPE || type == PIPE ||
			type == DOUBLE_AND || type == STAR);
}

bool	is_redirection(t_type type)
{
	return (type == LESSER || type == GREATER ||
			type == DOUBLE_LESSER || type == DOUBLE_GREATER);
}

bool	is_argument(t_type type)
{
	return (type == ARGUMENT || type == DOUBLE_QUOTE || type == SINGLE_QUOTE);
}