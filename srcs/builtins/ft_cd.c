/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:02:16 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/11 21:19:16 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd(t_token *current)
{
	static char 	*prev_path = NULL;
	char	*cur_path;
	char	*path;
	char	*new_path;

	cur_path = getcwd(NULL, 0);
	if (current->next)
	{
		printf("cd: too many arguments\n");
		printf("\nfrom built-ins\n");
		return ;
	}
	if (strcmp(current->value, "-") == 0)
	{
		if (!prev_path)
			printf("cd: OLDPWD not set\n");
		else
		{
			if (chdir(prev_path) == 0)
				printf("%s\n", prev_path);
			else
				perror("cd");
		}
		free(prev_path);
		prev_path = cur_path;
		printf("\nfrom built-ins\n");
		return ;
	}
	if (chdir(current->value) == 0)
	{
		printf("\nfrom built-ins\n");
		return ;
	}
	path = ft_strjoin(cur_path, "/");
	new_path = ft_strjoin(path, current->value);
	if (*current->value == '~')
		new_path = ft_strjoin(getenv("HOME"), current->value + 1);
	if (chdir(new_path) == -1)
		printf("cd: %s: No such file or directory\n", current->value);
	printf("\nfrom built-ins\n");
}

