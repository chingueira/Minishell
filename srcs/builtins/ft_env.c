/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:16:55 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/23 14:43:08 by welepy           ###   ########.fr       */
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
		g_exit_status = 1;
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
	g_exit_status = 0;
}
