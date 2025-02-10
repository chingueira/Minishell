/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:45:03 by mchingi           #+#    #+#             */
/*   Updated: 2025/01/29 09:03:56 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h" 

void	ft_pwd(t_token *token)
{
	char	*pwd;

	if (token && (token->type == ARGUMENT || token->type == OPTION))
	{
		printf("pwd: this version does not supports options or arguments\n");
		return ;
	}
	pwd = malloc(PATH_MAX);
	getcwd(pwd, PATH_MAX);
	printf("%s\n", pwd);
	free(pwd);
	printf("\nfrom built-ins\n");
}
