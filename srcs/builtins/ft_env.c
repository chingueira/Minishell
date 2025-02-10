/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:16:55 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/29 09:03:26 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

void	ft_env(t_env *env, t_token *token)
{
	t_env	*tmp;

	tmp = env;
	if (token && (token->type == ARGUMENT || token->type == OPTION))
	{
		printf("env: this env does not support options or arguments\n");
		return ;
	}
	while (tmp)
	{
		printf("%s=", tmp->name);
		if (tmp->value)
			printf("%s", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
	printf("\nfrom built-ins\n");
}
