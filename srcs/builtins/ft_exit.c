/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:32:38 by mchingi           #+#    #+#             */
/*   Updated: 2025/01/29 09:03:44 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

// Still to do: clean the leaks

void	ft_exit(t_shell *shell, t_token *token)
{
	if (token && (token->type == ARGUMENT || token->type == OPTION))
	{
		printf("exit: this versions does not supports options arguments\n");
		return ;
	}
	shell->flag = false;
	printf("\nfrom built-ins\n");
	exit (0);
}
