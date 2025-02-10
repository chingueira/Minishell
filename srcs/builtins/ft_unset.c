/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 17:33:18 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/29 09:04:06 by mchingi          ###   ########.fr       */
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
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	temp = token;
	while (temp && temp->type == ARGUMENT)
	{
		remove_env(&env, temp->value);
		temp = temp->next;
	}
	printf("\nfrom built-ins\n");
}
