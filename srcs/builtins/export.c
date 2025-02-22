/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:49:08 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/22 16:03:12 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"


static	t_env	*arg_to_env(t_token *token)
{
	char	**temp;
	t_env	*current_env;

	temp = ft_split(token->value, '=');
	current_env = safe_malloc(sizeof(t_env));
	current_env->name = ft_strdup(temp[0]);
	if (temp[1])
		current_env->value = ft_strdup(remove_quotes(temp[1]));
	else if (token->next && (token->next->type == DOUBLE_QUOTE || \
				token->next->type == SINGLE_QUOTE))
		current_env->value = ft_strdup(token->next->value);
	else
		current_env->value = NULL;
	current_env->next = NULL;
	free_matrix(temp);
	return (current_env);
}

t_env	*last_env(t_env *env)
{
	if (env == NULL)
		return (NULL);
	while (env->next != NULL)
		env = env->next;
	return (env);
}

void	add_env(t_env **env, t_env *new)
{
	t_env	*t;

	if (*env)
	{
		t = last_env(*env);
		t->next = new;
	}
	else
		*env = new;
}

void	ft_export(t_env *env, t_token *token)
{
	t_env	*temp;
	t_token	*head;

	head = token;
	while (head && (head->type == ARGUMENT))
	{
		temp = arg_to_env(head);
		ft_unset(env, head);
		add_env(&env, temp);
		head = head->next;
	}
}