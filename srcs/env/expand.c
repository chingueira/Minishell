/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 20:37:07 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/14 22:32:25 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char *process_expansion(char *str, t_env *env);

static char	*get_env_value(t_env *env, char *name)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void expand(char ***matrix, t_env *env)
{
	int y = 0;
	char *expanded;
	char *value;

	while ((*matrix)[y])
	{
		if ((*matrix)[y][0] == '$')
		{
			value = get_env_value(env, &(*matrix)[y][1]);
			free((*matrix)[y]);
			(*matrix)[y] = ft_strdup(value ? value : "");
		}
		else if (ft_strchr((*matrix)[y], '$'))
		{
			expanded = process_expansion((*matrix)[y], env);
			free((*matrix)[y]);
			(*matrix)[y] = expanded;
		}
		y++;
	}
}

static char	*process_expansion_helper(t_env *env, char *result, char *temp)
{
	char *value;

	value = get_env_value(env, temp);
	free(temp);
	if (value)
		result = ft_strjoin_free(result, value);
	else
		result = ft_strjoin_free(result, "");
	return (result);
}

static char *process_expansion(char *str, t_env *env)
{
	char *result;
	char *start;
	char *end;
	char *temp;

	start = str;
	result = ft_strdup("");
	while ((start = ft_strchr(start, '$')))
	{
		end = start;
		if (end > str)
		{
			temp = ft_substr(str, 0, end - str);
			result = ft_strjoin_free(result, temp);
		}
		start++;
		end = start;
		while (*end && (ft_isalnum(*end) || *end == '_'))
			end++;
		temp = ft_substr(start, 0, end - start);
		result = process_expansion_helper(env, result, temp);
		str = end;
	}
	result = ft_strjoin_free(result, str);
	return (result);
}
