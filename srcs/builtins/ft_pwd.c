/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:45:03 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/23 15:11:41 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

void	ft_pwd(t_token *token)
{
	char	*pwd;

	if (token && (token->type == ARGUMENT || token->type == OPTION))
	{
		printf("pwd: this version does not support options or arguments\n");
		g_exit_status = 1;
		return ;
	}
	pwd = safe_malloc(PATH_MAX);
	if (getcwd(pwd, PATH_MAX) == NULL)
	{
		perror("getcwd");
		free(pwd);
		g_exit_status = 1;
		return;
	}
	else
	{
		printf("%s\n", pwd);
		free(pwd);
		g_exit_status = 0;
	}
	printf("\nfrom built-ins\n");
}
