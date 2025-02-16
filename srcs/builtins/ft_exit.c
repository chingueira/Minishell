/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:32:38 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/16 16:34:24 by welepy           ###   ########.fr       */
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
	printf("\nfrom built-ins\n");
	clean_and_exit(shell);
}
