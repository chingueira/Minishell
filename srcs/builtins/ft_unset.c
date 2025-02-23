/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:33:18 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/23 15:13:18 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_env(t_env **head, const char *name)
{
	t_env	*current;
	t_env	*previous;

	current = *head;
	previous = NULL;
	while (current)
	{
		if (strcmp(current->name, name) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*head = current->next;
			if (current->name)
				free(current->name);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	ft_unset(t_env *env, t_token *token)
{
	t_token	*temp;

	if (!token)
		return ;
	temp = token;
	while (temp && temp->type == ARGUMENT)
	{
		remove_env(&env, temp->value);
		temp = temp->next;
	}
	g_exit_status = 0;
	printf("\nfrom built-ins\n");
}
