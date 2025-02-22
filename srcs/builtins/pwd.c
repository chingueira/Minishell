/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:49:13 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/15 17:15:10 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

void	ft_pwd(t_token *current)
{
	char	*pwd;
	t_token		*token;

	token = current->next;
	if (token && (token->type == ARGUMENT || token->type == OPTION))
	{
		printf("pwd: this version does not supports options or arguments\n");
		return ;
	}
	pwd = malloc(PATH_MAX);
	getcwd(pwd, PATH_MAX);
	printf("%s\n", pwd);
	free(pwd);
}