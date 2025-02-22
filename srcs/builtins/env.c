/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:48:58 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/15 17:15:33 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

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
}