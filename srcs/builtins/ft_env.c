/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:16:55 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/21 16:35:09 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

void	ft_env(t_env *env, t_token *token, bool declare)
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
		if (declare)
			printf("declare -x ");
		printf("%s=", tmp->name);
		if (tmp->value)
			printf("%s", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
	printf("\nfrom built-ins\n");
}
